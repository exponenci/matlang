#pragma once

#include "object.h"
#include "matrix.h"
#include "integer.h"
#include "expression.h"
#include "comm.h"

#include <stack>
#include <map>
#include <memory>
#include <string>
#include <functional>


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
    ArithmeticCommand(std::function<sptrObj(sptrObj, sptrObj)> &&f)
            : BaseCommand(cmd::cmd_type::Arithmetic),
              func_(std::move(f)) {
    }

    sptrObj Run(std::list<sptrObj> &args) override;
};


class PrintCommand : public BaseCommand {
public:
    PrintCommand()
            : BaseCommand(cmd::cmd_type::Print) {
    }

    sptrObj Run(std::list<sptrObj> &args) override;
};

class TransposeCommand : public BaseCommand {
public:
    TransposeCommand()
            : BaseCommand(cmd::cmd_type::Transpose) {
    }

    sptrObj Run(std::list<sptrObj> &args) override;
};
