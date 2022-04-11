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

public:
    Interpreter() = default;

private:
    std::shared_ptr<Object> Simplify(std::shared_ptr<Object> &);

public:
    void Run(const std::string &);
};

#endif //MATLANG_INTERPRETER_H
