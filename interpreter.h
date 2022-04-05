#pragma once

#ifndef MATLANG_INTERPRETER_H
#define MATLANG_INTERPRETER_H


#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <vector>

#include "dispatcher.h"
#include "parser.h"

class Interpreter {
private:
    Dispatcher operation_holder_ = Dispatcher::Instance();

public:
    Interpreter() = default;

private:
    std::shared_ptr<Object> Simplify(std::shared_ptr<Object>& object) {
        if (Is<CommandObject>(object)) {
            std::shared_ptr<Object> command_name = As<CommandObject>(object)->GetCommand();
            std::list<std::shared_ptr<Object>>& args = As<CommandObject>(object)->GetArgs();
            if (Is<Symbol>(command_name)) {
                for (auto& arg : args) {
                    arg = Simplify(arg);
                }
            } else {
                throw SyntaxError("Interpreter: invalid command name\n");
            }
            return operation_holder_.Invoke(As<Symbol>(command_name)->GetString(), args); // run command(*args)
        } else if (Is<Matrix<>>(object)) {
            for (auto &ptr : *As<Matrix<>>(object)) {
                ptr = Simplify(ptr);
            }
            return object;
        } else if (Is<Expression>(object)) {
            std::list<std::shared_ptr<Object>>& args = As<Expression>(object)->GetArgs();
            for (auto& arg : args) {
                arg = Simplify(arg);
            }
            As<Expression>(object)->Infix2Postfix();
            return operation_holder_.Eval(args); // evaluates: Integer{5} Integer{3} Integer{8} Symbol{*} Symbol{-}
        } else if (Is<Integer>(object)) {
            return object;
        } else if (Is<Symbol>(object)) {
            std::string symbol = As<Symbol>(object)->GetString();
            std::shared_ptr<Object> variable = operation_holder_.At(symbol);
            if (!variable) { // is there such variable
                throw SyntaxError("Interpreter: unknown type was given\n");
            }
            return variable;
        }
        throw SyntaxError("Interpreter: unknown type was given\n");
    }
public:
    std::string Run(const std::string&);
};

#endif //MATLANG_INTERPRETER_H
