#include "comm.h"

ArithmeticCommand::ArithmeticCommand(std::function<sptrObj(sptrObj, sptrObj)> &&f)
        : BaseCommand(cmd::cmd_type::Arithmetic),
          func_(std::move(f)) {
}

sptrObj ArithmeticCommand::Run(std::list<sptrObj> &args) {
    if (args.size() != 2) {
        throw RuntimeError("ArithmeticCommand: invalid number of arguments for operation\n");
    }
    return func_(args.front(), args.back());
}

PrintCommand::PrintCommand(std::ostream& out)
        : BaseCommand(cmd::cmd_type::Print), out_(out) {}

sptrObj PrintCommand::Run(std::list<sptrObj> &args) {
    for (auto &arg: args) {
        out_ << arg->GetString() << "\n";
    }
    return std::make_shared<NoneObject>();
}

TransposeCommand::TransposeCommand()
        : BaseCommand(cmd::cmd_type::Transpose) {}

sptrObj TransposeCommand::Run(std::list<sptrObj> &args) {
    if (args.size() != 1) {
        throw RuntimeError("TransposeCommand: invalid number of arguments to transpose\n");
    }
    if (Is<Matrix>(args.front())) {
        return As<Matrix>(args.front())->Transposed();
    }
    throw RuntimeError("TransposeCommand: invalid value was provided to transpose\n");
}


LinearTransformationCommand::LinearTransformationCommand(int mode)
        : BaseCommand(cmd::MatrixLinearTransform),
          mode_(mode) {}

void LinearTransformationCommand::MakeTransform(std::vector<std::vector<sptrObj>> &data, size_t rows_count,
                                                size_t columns_count) const {
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

sptrObj LinearTransformationCommand::Run(std::list<sptrObj> &args) {
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
    if (mode_ == cmd::rref) {
        return std::make_shared<Matrix>(std::move(data));
    }
    if ((mode_ & cmd::rref) == 1) {
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
