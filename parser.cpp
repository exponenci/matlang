#include "parser.h"

std::list<sptrObj> ReadScript(Tokenizer *tokenizer) {
    std::list<sptrObj> result;
    while (!tokenizer->IsEnd()) {
        sptrObj matlangobj = Read(tokenizer);
        if (!Is<NoneObject>(matlangobj)) {
            result.push_back(matlangobj);
        }
    }
    return result;
}

sptrObj Read(Tokenizer *tokenizer, size_t depth) {
    if (tokenizer->IsEnd()) {
        return nullptr;
    }
    sptrObj object;
    Token curr_token = tokenizer->GetToken();
    if (const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token)) {
        if (symbol_token_ptr->name_ == "let") { // we are initializing variable
            if (depth != 0) {
                throw SyntaxError("Read: let must begin from a new command line\n");
            }
            object = std::make_shared<CommandObject>();
            As<CommandObject>(object)->SetCommand(std::make_shared<Symbol>("init"));
            tokenizer->Next(); // after this tokenizer->GetToken() is expected to return
            // shared_ptr on Symbol object (containing variable name)
            sptrObj varname = Read(tokenizer, depth + 1); // read variable name with a depth > 0
            if (Is<Symbol>(varname)) {
                As<CommandObject>(object)->AddArg(std::move(varname));  // TODO perhaps we don't need to keep
                // it in Object type and simply move in std::string
            } else {
                throw SyntaxError("Read: variable name to be initialized is not a acceptable\n");
            }
            if (!ExpectRead(tokenizer, "=")) {
                throw SyntaxError("Read: invalid variable declaration (assignment sign was expected)\n");
            }
            // here we get list of objects which would specify initializing object
            std::list<sptrObj> expr = ReadExpression(tokenizer);
            if (expr.size() == 1) {
                // for size 1 - we simply keep it as it is
                As<CommandObject>(object)->AddArg(std::move(expr.front()));
            } else if (expr.size() > 1) {
                // otherwise, it is expression to be calculated
                As<CommandObject>(object)->AddArg(std::make_shared<Expression>(std::move(expr)));
            } else {
                throw SyntaxError("Read: object to be initialized was expected, nothing was received\n");
            }
            // at this point tokenizer->GetToken() is expected to return semicolon
        } else { // we are reading Symbol
            object = std::make_shared<Symbol>(symbol_token_ptr->name_);
            tokenizer->Next();
            curr_token = tokenizer->GetToken();
            if (const SymbolToken *token_ptr = std::get_if<SymbolToken>(&curr_token)) {
                if (token_ptr->name_ == "(") { // if reading symbol is a function call
                    sptrObj cmd_obj = std::make_shared<CommandObject>();
                    As<CommandObject>(cmd_obj)->SetCommand(object);
                    object = cmd_obj;
                    ReadCommandArgs(tokenizer, cmd_obj, depth);
                }
                // func(arg1, arg2)_
                //                 ^ <- tokenizer->GetToken()
                // TODO imho we always are on depth == 0 when we are calling Read to read function
                if (depth == 0) {
                    // if it is on new command line
                    // tokenizer->GetToken() is expected to return semicolon
                    curr_token = tokenizer->GetToken();
                    if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
                        return object;
                    } else {
                        throw SyntaxError("invalid function call (semicolon was forgotten)\n");
                    }
                }
            }
            // either `func(arg1, arg2)_` or `string_`
            //                         ^            ^
            return object;
        }
    } else if (const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token)) {
        // TODO imho we will never reach this code; so check it out
        if (depth > 0) {
            object = std::make_shared<Integer>(constant_token_ptr->value_);
        } else {
            throw SyntaxError("Read: invalid command line beginning (with integers)\n");
        }
    } else if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
        object = std::make_shared<NoneObject>(); // TODO should we return nullptr instead?
    } else { // if it is brackets token []
        throw SyntaxError("Read: invalid command line beginning (with brackets)\n");
    }
    tokenizer->Next();
    return object;
}

bool ExpectRead(Tokenizer *tokenizer, std::string_view sv) {
    if (tokenizer->IsEnd()) {
        return false;
    }
    sptrObj object;
    Token curr_token = tokenizer->GetToken();
    if (const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token)) {
        if (symbol_token_ptr->name_ == sv) {
            tokenizer->Next();
            return true;
        }
    }
    return false;
}

sptrObj ReadCommandArgs(Tokenizer *tokenizer, sptrObj object, size_t depth) { // TODO remove depth argument
    // calling if tokenizer->GetToken() returns SymbolToken("(")
    // at begin:
    // function(args)_
    //         ^
    // at end:
    // function(args)_
    //               ^
    Token curr_token = tokenizer->GetToken();
    SymbolToken *sym_ptr = std::get_if<SymbolToken>(&curr_token);
    if (!sym_ptr || !ExpectRead(tokenizer, "(")) {
        throw SyntaxError("ReadCommandArgs: invalid function call (opening bracket was expected)\n");
    }
    while (true) { // reading args of function
        curr_token = tokenizer->GetToken(); // first arg of function
        const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
        const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token);
        const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token);
        if (bracket_token_ptr && *bracket_token_ptr == BracketToken::CLOSE) {
            // TODO is it real `throw error: print([]])` ??
            throw SyntaxError("ReadCommandArgs: invalid function argument (closing array branch was not expected)\n");
        } else if (symbol_token_ptr || constant_token_ptr || bracket_token_ptr) {
            // if it is symbol (ex: variable) or integer or matrix (or, as planned for future, vector)
            if (symbol_token_ptr && symbol_token_ptr->name_ == ")") { // function arguments end
                // TODO think: we are here only if function was called without arguments, like func()
                // actually, we can remove this block, because ReadExpression handles these situation too
                // TODO: remove this block
                tokenizer->Next();
                break;
            }
            bool is_last_arg; // if it will be true then we received last argument of func and should break loop
            std::list<sptrObj> data = ReadExpression(tokenizer, true, &is_last_arg);
            if (data.size() == 1) {
                As<CommandObject>(object)->AddArg(std::move(data.front()));
            } else if (data.size() > 1) {
                As<CommandObject>(object)->AddArg(std::make_shared<Expression>(std::move(data)));
            } else { // if there is no expression received
                throw SyntaxError("ReadCommandArgs: invalid function call (invalid arguments pack)\n");
            }
            // here tokenizer->GetToken() is expected to return:
            // func(arg1_expr, arg2_expr, arg2_expr)
            //                 ^                    ^    <-- one of these 2 positions
            if (is_last_arg) {
                break;
            }
            continue;
        } else if (const SemicolonToken *semicolon_token = std::get_if<SemicolonToken>(&curr_token)) {
            break;
        } else {
            throw SyntaxError("invalid syntax with unknown symbol (in function args read)\n");
        }
        tokenizer->Next();
    }
    /* no need for this, because this situation is already being handled in calling funcion
    curr_token = tokenizer->GetToken();
    const SemicolonToken *semi_token_ptr = std::get_if<SemicolonToken>(&curr_token);
    if (depth == 0 && !semi_token_ptr) {
        throw SyntaxError("invalid function call (semicolon was forgotten 2)\n");
    }
    """*/
    return object;
}


std::list<sptrObj> ReadExpression(Tokenizer *tokenizer, bool inner_expr, bool *is_last_arg) {
    // TODO what is idea of inner_expr argument???
    // at calling moment:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //  ^                     ^                     ^
    // after working:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //              ^                     ^                     ^
    // in all cases returns list{Symbol("val"), Symbol("+"), Number(1), Symbol("*"), Symbol("A")}
    std::list<sptrObj> objects;
    Token curr_token;
    size_t open_brackets_count = 0; // we are going to count arithmetic brackets () to handle arithmetic expressions
    while (true) {
        if (tokenizer->IsEnd()) {
            break; // throw error?
        }
        curr_token = tokenizer->GetToken();
        const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token);
        const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
        if (symbol_token_ptr || constant_token_ptr) {
            if (inner_expr && symbol_token_ptr && (symbol_token_ptr->name_ == "," || symbol_token_ptr->name_ == ")") &&
                open_brackets_count == 0) {
                // what is inner_expr?
                if (is_last_arg) { // if we have pointer to be filled
                    *is_last_arg = (symbol_token_ptr->name_ == ")");
                }
                tokenizer->Next();
                break;
            }
            sptrObj symb_obj;
            if (symbol_token_ptr) { // f(a +
                symb_obj = std::make_shared<Symbol>(symbol_token_ptr->name_);
                if (symbol_token_ptr->name_ == ")") {
                    // TODO something wrong is here ...
                    --open_brackets_count; // f(3 * (1 + 2))
                    if (inner_expr && open_brackets_count == 0) {
                        if (is_last_arg) {
                            *is_last_arg = (symbol_token_ptr->name_ == ")");
                        }
                        tokenizer->Next();
                        break;
                    }
                } else if (symbol_token_ptr->name_ == "(") {
                    ++open_brackets_count;
                }
            } else {
                symb_obj = std::make_shared<Integer>(constant_token_ptr->value_);
            }
            tokenizer->Next(); // a +
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
        } else {
            throw SyntaxError("invalid syntax with unknown symbol (in expression read)\n");
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
    return std::make_shared<Matrix>(std::move(objects));
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
