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
public:
    PrintCommand();

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
    LinearTransformationCommand(int mode) : BaseCommand(cmd::MatrixLinearTransform), mode_(mode) {}

    void MakeTransform(std::vector<std::vector<sptrObj>> &data, size_t rows_count, size_t columns_count) const {
        size_t curr_row = 0;
        std::shared_ptr<Evaluable> div, mul_cf;
        while (curr_row < rows_count) {
            for (size_t i = mode_ == cmd::to_triangle ? curr_row + 1 : 0; i < rows_count; ++i) {
                if (curr_row >= columns_count) {
                    continue;
                }
                if (As<Rational>(data[curr_row][curr_row])->Numerator() == 0) {
                    bool non_zero_not_found = true;
                    for (size_t k = curr_row + 1; k < rows_count; ++k) {
                        if (As<Rational>(data[k][curr_row])->Numerator() != 0) {
                            std::swap(data[k], data[curr_row]);
                            non_zero_not_found = false;
                            break;
                        }
                    }
                    if (non_zero_not_found) {
                        continue;
                    }
                }
                div = As<Evaluable>(data[curr_row][curr_row]);
                mul_cf = *As<Evaluable>(data[i][curr_row]) / div;
                for (size_t j = 0;
                     j < columns_count; ++j) { // should run beginning from curr_row + 1 if want triang view
                    if (i == curr_row && (mode_ & cmd::inv)) { // should ignore, if want diag only
                        data[i][j] = *As<Evaluable>(data[i][j]) / div; // make 1 leading
                    } else if (i != curr_row) {
                        data[i][j] =
                                *As<Evaluable>(data[i][j]) -
                                (*As<Evaluable>(data[curr_row][j]) * mul_cf); // make zero all others
                    }
                }
            }
            ++curr_row;
        }
    }

    sptrObj Run(std::list<sptrObj> &args) override {
        if (args.size() != 1) {
            throw RuntimeError("LinearTransformationCommand::Run: expected 1 argument\n");
        }
        if (!Is<Matrix>(args.front())) {
            throw RuntimeError("LinearTransformationCommand::Run: expected matrix as argument\n");
        }
        std::vector<std::vector<sptrObj>> data = As<Matrix>(args.front())->DeepCopy();
        size_t rows_count = data.size(), columns_count = data[0].size();
        if (mode_ == cmd::inv) {
            if (rows_count != columns_count) {
                throw RuntimeError("LinearTransformationCommand::Run: (inv) only square matrix can be inverse\n");
            }
            for (size_t i = 0; i < rows_count; ++i) {
                for (size_t j = 0; j < columns_count; ++j) {
                    data[i].push_back(std::make_shared<Rational>(i == j));
                }
            }
            columns_count <<= 1;
        } else if (mode_ == cmd::det && rows_count != columns_count) {
            throw RuntimeError("LinearTransformationCommand::Run: (det) det is only for square matrices\n");
        }
        MakeTransform(data, rows_count, columns_count);
        if (mode_ & cmd::rref) {
            return std::make_shared<Matrix>(std::move(data));
        }
        if (mode_ == cmd::inv) {
            std::vector<std::vector<sptrObj>> inv_result;
            inv_result.resize(rows_count);
            for (size_t i = 0; i < rows_count; ++i) {
                if (As<Rational>(data[i][i])->Numerator() == 0) {
                    throw RuntimeError(
                            "LinearTransformationCommand::Run: inverse of matrix with det = 0 was requested\n");
                }
                inv_result[i].reserve(rows_count);
                for (size_t j = 0; j < rows_count; ++j) {
                    inv_result[i].push_back(data[i][rows_count + j]);
                }
            }
            return std::make_shared<Matrix>(inv_result);
        }
        if (mode_ == cmd::det) {
            std::shared_ptr<Rational> determinant = std::make_shared<Rational>(1);
            for (size_t i = 0; i < rows_count; ++i) {
                determinant = As<Rational>(*determinant * As<Evaluable>(data[i][i]));
            }
            return determinant;
        }
        // rank
        size_t rank = std::min(rows_count, columns_count);
        for (size_t i = 0; i < std::min(rows_count, columns_count); ++i) {
            if (As<Rational>(data[i][i])->Numerator() == 0) {
                --rank;
            }
        }
        return std::make_shared<Rational>(rank);
    }
};
