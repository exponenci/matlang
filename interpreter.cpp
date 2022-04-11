#include "interpreter.h"


void Interpreter::Run(const std::string &expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};

    std::string result;
    result.reserve(100);

    std::list<std::shared_ptr<Object>> parsed_script = ReadScript(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("no whole line has been read;");
    }
    for (auto &line: parsed_script) {
        line = Simplify(line); // make some evaluations, remove `ExpressionObject`s
    }
}

std::shared_ptr<Object> Interpreter::Simplify(std::shared_ptr<Object> &object) {
    if (Is<CommandObject>(object)) {
        std::shared_ptr<Object> command_name = As<CommandObject>(object)->GetCommand();
        std::list<std::shared_ptr<Object>> &args = As<CommandObject>(object)->GetArgs();
        if (Is<Symbol>(command_name)) {
            if (As<Symbol>(command_name)->GetString() == "init" && Is<Symbol>(args.front())) {
                args.back() = Simplify(args.back());
            } else {
                for (auto &arg: args) {
                    arg = Simplify(arg);
                }
            }
        } else {
            throw SyntaxError("Interpreter: invalid command name\n");
        }
        return operation_holder_.Invoke(As<Symbol>(command_name)->GetString(), args); // run command(*args)
    } else if (Is<Matrix>(object)) {
        for (auto &ptr: *As<Matrix>(object)) {
            ptr = Simplify(ptr);
        }
        return object;
    } else if (Is<Expression>(object)) {
        std::list<std::shared_ptr<Object>> &args = As<Expression>(object)->GetArgs();
        for (auto &arg: args) {
            arg = Simplify(arg);
        }
        As<Expression>(object)->Infix2Postfix();
        return operation_holder_.Eval(args); // evaluates: Integer{5} Integer{3} Integer{8} Symbol{*} Symbol{-}
    } else if (Is<Rational>(object)) {
        return object;
    } else if (Is<Symbol>(object)) {
        std::string symbol = As<Symbol>(object)->GetString();
        if (operation_holder_.IsRegisteredSymbol(symbol)) {
            return object;
        }
        std::shared_ptr<Object> variable = operation_holder_.At(symbol);
        if (!variable) { // is there such variable
            throw SyntaxError("Interpreter: unknown symbol was given\n");
        }
        return variable;
    }
    throw SyntaxError("Interpreter: unknown type was given\n");
}
