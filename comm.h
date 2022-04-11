#pragma once

#include "object.h"
#include "rational.h"
#include "matrix.h"
#include "expression.h"

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <string>


namespace cmd {
    enum cmd_type {
        Print,
        Transpose,
//        Det,
//        Inv,
//        Rank,
//        ToTriangle,
//        ToDiag,
//        Rrref,
//        Initialize, // already done separately
        Arithmetic
    };
}


class BaseCommand {
private:
    cmd::cmd_type type_;

public:
    BaseCommand(cmd::cmd_type type) : type_(type) {}

    virtual sptrObj Run(std::list<sptrObj> &) = 0;

    virtual ~BaseCommand() = default;
};

class ArithmeticCommand : public BaseCommand {
private:
    std::function<sptrObj(sptrObj, sptrObj)> func_;

public:
    ArithmeticCommand(std::function<sptrObj(sptrObj, sptrObj)> &&);

    sptrObj Run(std::list<sptrObj> &) override;
};


class PrintCommand : public BaseCommand {
public:
    PrintCommand();

    sptrObj Run(std::list<sptrObj> &) override;
};

class TransposeCommand : public BaseCommand {
public:
    TransposeCommand();

    sptrObj Run(std::list<sptrObj> &) override;
};
