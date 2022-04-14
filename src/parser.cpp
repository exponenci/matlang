#include "parser.h"

constexpr bool IsSpecialSymbol(std::string_view sv) {
    constexpr std::string_view specials = "!&()*+,-./:;<=>[]^{|}~";
    return specials.find(sv) != std::string_view::npos;
}

std::list<sptrObj> ReadScript(Tokenizer *tokenizer) {
    std::list<sptrObj> result;
    while (!tokenizer->IsEnd()) {
        sptrObj line_obj = Read(tokenizer);
        Token curr_token = tokenizer->GetToken();
        if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
            if (!Is<NoneObject>(line_obj)) {
                result.push_back(line_obj);
            }
        } else {
            throw SyntaxError("ReadScript: invalid function call (semicolon was forgotten)\n");
        }
        tokenizer->Next();
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
            object = std::make_shared<CommandObject>();
            As<CommandObject>(object)->SetCommand(std::make_shared<Symbol>("init"));
            tokenizer->Next(); // after this tokenizer->GetToken() is expected to return
            curr_token = tokenizer->GetToken();
            symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
            if (!symbol_token_ptr) {
                throw SyntaxError("Read: variable name to be initialized is not a acceptable\n");
            }
            As<CommandObject>(object)->AddArg(std::move(std::make_shared<Symbol>(symbol_token_ptr->name_)));
            tokenizer->Next();
            if (!ExpectRead(tokenizer, "=")) {
                throw SyntaxError("Read: invalid variable declaration (assignment sign was expected)\n");
            }
            As<CommandObject>(object)->AddArg(ReadExpression(tokenizer));
        } else { // we are reading Symbol
            object = std::make_shared<Symbol>(symbol_token_ptr->name_);
            tokenizer->Next();
            curr_token = tokenizer->GetToken();
            if (const SymbolToken *token_ptr = std::get_if<SymbolToken>(&curr_token)) {
                if (token_ptr->name_ == "(") { // if reading symbol is a function call
                    sptrObj cmd_obj = std::make_shared<CommandObject>();
                    As<CommandObject>(cmd_obj)->SetCommand(object);
                    object = cmd_obj;
                    ReadCommandArgs(tokenizer, cmd_obj);
                } else {
                    throw SyntaxError("Read: invalid command line beginning (function call was expected)\n");
                }
            } else {
                throw SyntaxError(
                        "Read: invalid command line beginning (function call was expected, unknown symbol was received)\n");
            }
            // func(arg1, arg2)_
            //                 ^ <- tokenizer->GetToken()
        }
    } else if (const SemicolonToken *semicolon_token_ptr = std::get_if<SemicolonToken>(&curr_token)) {
        object = std::make_shared<NoneObject>(); // TODO should we return nullptr instead?
    } else { // if it is brackets or constant token
        throw SyntaxError("Read: invalid command line beginning (with brackets or constant)\n");
    }
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

sptrObj ReadCommandArgs(Tokenizer *tokenizer, sptrObj object) {
    // calling if tokenizer->GetToken() returns SymbolToken("(")
    // at begin:
    // function(args)_
    //         ^
    // at end:
    // function(args)_
    //               ^
    Token curr_token = tokenizer->GetToken();
    if (!std::get_if<SymbolToken>(&curr_token) || !ExpectRead(tokenizer, "(")) {
        throw SyntaxError("ReadCommandArgs: invalid function call (opening bracket was expected)\n");
    }
    while (true) { // reading args of function
        curr_token = tokenizer->GetToken(); // first arg of function
        const SymbolToken *symbol_token_ptr = std::get_if<SymbolToken>(&curr_token);
        const ConstantToken *constant_token_ptr = std::get_if<ConstantToken>(&curr_token);
        const BracketToken *bracket_token_ptr = std::get_if<BracketToken>(&curr_token);
        if (bracket_token_ptr && *bracket_token_ptr == BracketToken::CLOSE) {
            throw SyntaxError("ReadCommandArgs: invalid function argument (closing array branch was not expected)\n");
        } else if (symbol_token_ptr || constant_token_ptr || bracket_token_ptr) {
            bool is_last_arg; // if it will be true then we received last argument of func and should break loop
            As<CommandObject>(object)->AddArg(ReadExpression(tokenizer, &is_last_arg));
            // here tokenizer->GetToken() is expected to return:
            // func(arg1_expr, arg2_expr, arg2_expr)
            //                 ^                    ^    <-- one of these 2 positions
            if (is_last_arg) {
                break;
            }
            continue;
        } else {
            // example of script when we reach this code: `func(arg1; arg2);`
            throw SyntaxError("ReadCommandArgs: invalid syntax with unknown symbol\n");
        }
    }
    return object;
}


sptrObj ReadExpression(Tokenizer *tokenizer, bool *is_last_arg) {
    // inner_expr - is param to make function know when to end reading;
    // if it is true, it is expected to end after `)` or `,` or `]`, else it must stop reading at `;`

    // at calling moment:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //  ^                     ^                     ^
    // after working:
    // val + 1 * A, _        val + 1 * A) _        val + 1 * A] _
    //              ^                     ^                     ^
    // in all cases returns list{Symbol("val"), Symbol("+"), Number(1), Symbol("*"), Symbol("A")}
    std::list<sptrObj> objects;
    Token curr_token;
    ConstantToken *const_tptr;
    SymbolToken *symbol_tptr;
    BracketToken *bracket_tptr;
    sptrObj curr_object;
    size_t open_brackets_count = 0; // we are going to count arithmetic brackets () to handle arithmetic expressions
    bool is_first_token = true; // to handle first in opening circle branch if it is some difficult expression
    while (true) {
        if (tokenizer->IsEnd()) {
            break; // throw error?
        }
        curr_token = tokenizer->GetToken();
        if ((symbol_tptr = std::get_if<SymbolToken>(&curr_token))) {
            if (is_last_arg && (symbol_tptr->name_ == "," || symbol_tptr->name_ == ")") &&
                open_brackets_count == 0) {
                *is_last_arg = (symbol_tptr->name_ == ")");
                tokenizer->Next();
                break;
            }
            if (symbol_tptr->name_ == ")") {
                --open_brackets_count;
            } else if (is_first_token && symbol_tptr->name_ == "(") {
                ++open_brackets_count;
            }
            curr_object = std::make_shared<Symbol>(symbol_tptr->name_);
            tokenizer->Next();
            curr_token = tokenizer->GetToken();
            if ((symbol_tptr = std::get_if<SymbolToken>(&curr_token))) {
                if (symbol_tptr->name_ == "(") { // checking situation of when function is called
                    if (IsSpecialSymbol(As<Symbol>(curr_object)->GetString())) {
                        // so we are in some non-function-calling expression
                        ++open_brackets_count;
                    } else {
                        // function call is expected
                        std::shared_ptr<Object> cmd_obj = std::make_shared<CommandObject>();
                        As<CommandObject>(cmd_obj)->SetCommand(curr_object);
                        curr_object = cmd_obj;
                        ReadCommandArgs(tokenizer, cmd_obj);
                    }
                }
            }
            objects.push_back(curr_object);
        } else if ((const_tptr = std::get_if<ConstantToken>(&curr_token))) {
            objects.push_back(std::make_shared<Rational>(const_tptr->value_));
            tokenizer->Next();
        } else if ((bracket_tptr = std::get_if<BracketToken>(&curr_token))) {
            if (*bracket_tptr == BracketToken::OPEN) {
                /* from   [..., ...]_  to   [..., ...]_
                 *        ^                           ^
                 */
                objects.push_back(ReadMatrix(tokenizer));
            } else if (is_last_arg) { // if it is expression in matrix/vector
                *is_last_arg = true;
                tokenizer->Next();
                break;
            } else {
                throw SyntaxError("ReadExpression: expression is expected to end with a semicolon, "
                                  "closing square bracket was received\n");
            }
        } else if (std::get_if<SemicolonToken>(&curr_token)) {
            if (!is_last_arg) {
                break;
            } else {
                // if ReadExpression was called from inner expression, but suddenly received unexpected token
                throw SyntaxError("ReadExpression: semicolon was received unexpectedly\n");
            }
        } else {
            // even if we handle all 4 existing tokens, there could be some problems if new tokens will be created
            throw SyntaxError("ReadExpression: invalid token in expression\n");
        }
        is_first_token = false;
    }

    if (objects.size() == 1) {
        return objects.front();
    } else if (objects.size() > 1) {
        return std::make_shared<Expression>(std::move(objects));
    } else {
        throw SyntaxError("ReadExpression: object to be initialized was expected, nothing was received\n");
    }
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
                throw SyntaxError("ReadMatrix: invalid mat init (in outer vectors)\n");
            }
        } else {
            throw SyntaxError("ReadMatrix: invalid mat init (in outer vectors)\n");
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
                throw SyntaxError("ReadLine: invalid mat init (in inner vectors)\n"); // throw error mat A = [[1, []]]
            } else {
                tokenizer->Next();
                break; // breaks when we are at the closing bracket of vector
            }
        } else {
            bool is_last;
            objects.push_back(ReadExpression(tokenizer, &is_last));
            if (is_last) {
                break;
            }
        }
    }
    return objects;
}
