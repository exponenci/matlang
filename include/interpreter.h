#pragma once

#include "matrix.h"
#include "parser.h"
#include "dispatcher.h"

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <vector>

#ifndef MATLANG_INTERPRETER_H
#define MATLANG_INTERPRETER_H


class Interpreter {
private:
    Dispatcher operation_holder_ = Dispatcher::Instance();
    std::ostream& out_;

public:
    explicit Interpreter(std::ostream& out = std::cout) : out_(out) {
        operation_holder_.SetCommand("print", std::make_shared<PrintCommand>(out_));
    };

private:
    std::shared_ptr<Object> Simplify(std::shared_ptr<Object> &);

public:
    void Run(const std::string &);
    void Run();
};

#endif //MATLANG_INTERPRETER_H
