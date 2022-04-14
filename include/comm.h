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
        MatrixLinearTransform,
        Arithmetic
//        Initialize, // already done separately
    };

    enum LTCmdMode {
        rref = 0b1001,
        to_diag = 0b0011,
        to_triangle = 0b0101,
        inv = 0b1000,
        det = 0b0110,
        rank = 0b0000,
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
private:
    std::ostream &out_;
public:
    PrintCommand(std::ostream &);

    sptrObj Run(std::list<sptrObj> &) override;
};

class TransposeCommand : public BaseCommand {
public:
    TransposeCommand();

    sptrObj Run(std::list<sptrObj> &) override;
};


class LinearTransformationCommand : public BaseCommand {
private:
    int mode_;

public:
    LinearTransformationCommand(int);

    void MakeTransform(std::vector<std::vector<sptrObj>> &, size_t, size_t) const;

    sptrObj Run(std::list<sptrObj> &) override;
};
