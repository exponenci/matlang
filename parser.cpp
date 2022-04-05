#include "parser.h"


std::list<std::shared_ptr<Object>> ReadScript(Tokenizer *tokenizer) {
    std::list<std::shared_ptr<Object>> result;
    while (!tokenizer->IsEnd()) {
        result.push_back(Read(tokenizer));
    }
    return result;
}

std::shared_ptr<Object> Read(Tokenizer *tokenizer, size_t depth) {
    /* */
    if (tokenizer->IsEnd()) {
        return nullptr;
    }
    std::shared_ptr<Object> object;
    Token curr_token = tokenizer->GetToken();
    if (const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token)) {
        if (symbol_token_ptr->name_ == "mat") { // we are initializing some variable matrix
            if (depth != 0) {
                throw SyntaxError("invalid mat init (you must init mat in separate line)\n");
            }
            object = std::make_shared<CommandObject>();
            As<CommandObject>(object)->SetCommand(std::make_shared<Symbol>("init"));
            tokenizer->Next();
            std::shared_ptr<Object> varname = Read(tokenizer, depth + 1);
            if (Is<Symbol>(varname)) {
                As<CommandObject>(object)->AddArg(std::move(varname));
            } else {
                throw SyntaxError("invalid mat init (invalid variable name)\n");
            }
            if (!ExpectRead(tokenizer, "=")) {
                throw SyntaxError("invalid mat init (you must declare assignment explicitly)\n");
            }
            std::list<std::shared_ptr<Object>> expr = ReadExpression(tokenizer);
            if (expr.size() == 1) {
                As<CommandObject>(object)->AddArg(std::move(expr.front()));
            } else if (expr.size() > 1) {
                As<CommandObject>(object)->AddArg(std::make_shared<Expression>(std::move(expr)));
            } else {
                throw SyntaxError("invalid mat init (you must assign variable something)\n");
            }
        } else { // we are reading symbol
            object = std::make_shared<Symbol>(symbol_token_ptr->name_);
            tokenizer->Next();
            curr_token = tokenizer->GetToken();
            if (const SymbolToken *token_ptr = std::get_if<SymbolToken>(&curr_token)) {
                if (token_ptr->name_ == "(") {
                    std::shared_ptr<Object> cmd_obj = std::make_shared<CommandObject>();
                    As<CommandObject>(cmd_obj)->SetCommand(object);
                    object = cmd_obj;
                    ReadCommandArgs(tokenizer, cmd_obj, depth);
                }
            }
            if (depth == 0) {
                tokenizer->Next();
                curr_token = tokenizer->GetToken();
                if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
                    return object;
                } else {
                    throw SyntaxError("invalid function call (semicolon was forgotten)\n");
                }
            }
            return object;
        }
    } else if (const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token)) {
        if (depth > 0) {
            object = std::make_shared<Integer>(constant_token_ptr->value_);
        } else {
            throw SyntaxError("invalid string beginning\n");
        }
    } else if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
        object = std::make_shared<NoneObject>();
    } else {
        throw SyntaxError("invalid string beginning\n");
    }
    tokenizer->Next();
    return object;
}

bool ExpectRead(Tokenizer *tokenizer, std::string_view sv) {
    if (tokenizer->IsEnd()) {
        return false;
    }
    std::shared_ptr<Object> object;
    Token curr_token;
    curr_token = tokenizer->GetToken();
    if (const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token)) {
        if (symbol_token_ptr->name_ == sv) {
            tokenizer->Next();
            return true;
        }
    }
    return false;
}

std::shared_ptr<Object> ReadCommandArgs(Tokenizer *tokenizer, std::shared_ptr<Object> object, size_t depth) {
    // calling if tokenizer->GetToken() returns SymbolToken("(")
    Token curr_token = tokenizer->GetToken();
    if (const SymbolToken *syptr = std::get_if<SymbolToken>(&curr_token)) {
        if (!ExpectRead(tokenizer, "(")) {
            throw SyntaxError("invalid function call (opening bracket)\n");
        }
        while (true) { // reading args of function
            curr_token = tokenizer->GetToken();
            const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
            const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token);
            const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token);
            if (bracket_token_ptr && *bracket_token_ptr == BracketToken::CLOSE) {
                throw SyntaxError("invalid function argument\n"); // throw error: print([]])
            } else if (symbol_token_ptr || constant_token_ptr || bracket_token_ptr) {
                if (symbol_token_ptr && symbol_token_ptr->name_ == ")") { // function arguments end
                    tokenizer->Next();
                    break;
                }
                bool is_last_arg;
                auto data = ReadExpression(tokenizer, true, &is_last_arg);
                if (data.size() == 1) {
                    As<CommandObject>(object)->AddArg(std::move(data.front()));
                } else if (data.size() > 1) {
                    As<CommandObject>(object)->AddArg(std::make_shared<Expression>(std::move(data)));
                } else {
                    throw SyntaxError("invalid function call (invalid arguments)\n");
                }
                if (is_last_arg) {
                    break;
                }
                continue;
            } else if (const SemicolonToken *semicolon_token = std::get_if<SemicolonToken>(&curr_token)) {
                break;
            } else { // Division symbol
                throw SyntaxError("invalid syntax with division symbol (in function args read)\n");
            }
            tokenizer->Next();
        }
        curr_token = tokenizer->GetToken();
        const SemicolonToken *semi_token_ptr = std::get_if<SemicolonToken>(&curr_token);
        if (depth == 0 && !semi_token_ptr) {
            throw SyntaxError("invalid function call (semicolon was forgotten 2)\n");
        }
    } else {
        throw SyntaxError("invalid function call (opening bracket expected)\n");
    }
    return object;
}


std::list<std::shared_ptr<Object>> ReadExpression(Tokenizer *tokenizer, bool inner_expr, bool *is_last_arg) {
    // at calling moment:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //  ^                     ^                     ^
    // after working:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //              ^                     ^                     ^
    // in all cases returns list{Symbol("val"), Symbol("+"), Number(1), Symbol("*"), Symbol("A")}
    std::list<std::shared_ptr<Object>> objects;
    Token curr_token;
    size_t open_brackets_count = 0;
    while (true) {
        if (tokenizer->IsEnd()) {
            break; // throw error?
        }
        curr_token = tokenizer->GetToken();
        const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token);
        const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
        if (symbol_token_ptr || constant_token_ptr) {
            if (inner_expr && symbol_token_ptr && (symbol_token_ptr->name_ == "," || symbol_token_ptr->name_ == ")") && open_brackets_count == 0) {
                if (is_last_arg) {
                    *is_last_arg = (symbol_token_ptr->name_ == ")");
                }
                tokenizer->Next();
                break;
            }
            std::shared_ptr<Object> symb_obj;
            if (symbol_token_ptr) {
                symb_obj = std::make_shared<Symbol>(symbol_token_ptr->name_);
                if (symbol_token_ptr->name_ == ")") {
                    --open_brackets_count;
                }
            } else {
                symb_obj = std::make_shared<Integer>(constant_token_ptr->value_);
            }
            tokenizer->Next();
            curr_token = tokenizer->GetToken();
            if (const SymbolToken *token_ptr = std::get_if<SymbolToken>(&curr_token)) {
                if (symbol_token_ptr && token_ptr->name_ == "(") {
                    if (As<Symbol>(symb_obj)->GetString() != "*" && As<Symbol>(symb_obj)->GetString() != "/" &&
                        As<Symbol>(symb_obj)->GetString() != "+" && As<Symbol>(symb_obj)->GetString() != "-") {
                        std::shared_ptr<Object> cmd_obj = std::make_shared<CommandObject>();
                        As<CommandObject>(cmd_obj)->SetCommand(symb_obj);
                        ReadCommandArgs(tokenizer, cmd_obj, 1);
                        objects.push_back(cmd_obj);
                        continue;
                    } else {
                        ++open_brackets_count;
                    }
                }
            }
            objects.push_back(symb_obj);
            continue;
        } else if (const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token)) {
            if (*bracket_token_ptr == BracketToken::OPEN) {
                objects.push_back(ReadMatrix(tokenizer));
                continue;
            } else {
                if (is_last_arg) {
                    *is_last_arg = true;
                }
                tokenizer->Next();
                break;
            }
        } else if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
            break;
        } else { // Division symbol
            throw SyntaxError("invalid syntax with division symbol (in expression read)\n");
        }
        tokenizer->Next();
    }
    return objects;
}


std::shared_ptr<Object> ReadMatrix(Tokenizer *tokenizer) {
    // when get arg:
    // [[a, b, c]] _
    // ^
    // when returns:
    // [[a, b, c]] _
    //             ^
    // we must call ReadMatrix at the moment, when tokenizer->CurrToken() returns FIRST opening bracket `[`
    // function returns shared ptr to Matrix Object,
    // tokenizer at the returning moment returns SECOND closing bracket `]`
    std::vector<std::vector<std::shared_ptr<Object>>> objects;
    Token curr_token;
    tokenizer->Next();
    while (true) {
        if (tokenizer->IsEnd()) {
            break; // throw error?
        }
        curr_token = tokenizer->GetToken();
        if (const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token)) {
            if (*bracket_token_ptr == BracketToken::OPEN) {
                objects.push_back(ReadLine(tokenizer));
            } else {
                tokenizer->Next();
                break;
            }
        } else if (const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token)) {
            if (symbol_token_ptr->name_ == ",") {
                tokenizer->Next();
                continue;
            } else {
                throw SyntaxError("invalid mat init (in outer vectors)\n");
            }
        } else {
            throw SyntaxError("invalid mat init (in outer vectors)\n");
        }
    }
    return std::make_shared<Matrix<>>(std::move(objects));
}


std::vector<std::shared_ptr<Object>> ReadLine(Tokenizer *tokenizer) {
    // we must call ReadLine at the moment, when tokenizer->CurrToken() returns FIRST opening bracket `[`
    // after function is done, tokenizer->Next() already returned closing bracket `]`
    // when get:
    // [a, b, c] _
    // ^
    // after:
    // [a, b, c] _
    //           ^
    std::vector<std::shared_ptr<Object>> objects;
    Token curr_token;
    tokenizer->Next(); // was [, now we expect some integer or expression
    while (true) {
        if (tokenizer->IsEnd()) {
            break; // throw error?
        }
        curr_token = tokenizer->GetToken();
        if (const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token)) {
            if (*bracket_token_ptr == BracketToken::OPEN) {
                throw SyntaxError("invalid mat init (in inner vectors)\n"); // throw error mat A = [[1, []]]
            } else {
                tokenizer->Next();
                break; // breaks when we are at the closing bracket of vector
            }
        } else {
            bool is_last;
            std::list<std::shared_ptr<Object>> data = ReadExpression(tokenizer, true, &is_last);
            if (data.size() == 1) {
                objects.push_back(data.front());
            } else if (data.size() > 1) {
                objects.push_back(std::make_shared<Expression>(std::move(data)));
            } else {
                throw SyntaxError("Error read line");
            }
            if (is_last) {
                break;
            }
        }
    }
    return objects;
}
