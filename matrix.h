#pragma once

#include <iostream>
#include <vector>

#include "object.h"
#include "integer.h"
#include "error.h"

//#ifndef MATLANG_MATRIX_H
//#define MATLANG_MATRIX_H

class Matrix;


class ConstMatrixIter {
private:
    const Matrix *matrix_ptr_;
    size_t curr_l_, curr_c_;

public:
    explicit ConstMatrixIter(const Matrix *ptr, size_t line = 0, size_t colm = 0) :
            matrix_ptr_(ptr),
            curr_l_(line),
            curr_c_(colm) {}

    bool operator==(const ConstMatrixIter &other) const;

    bool operator!=(const ConstMatrixIter &other) const;

    const sptrObj &operator*() const;

    ConstMatrixIter &operator++();

    ConstMatrixIter operator++(int);
};


class MatrixIter {
private:
    Matrix *matrix_ptr_;
    size_t curr_l_, curr_c_;

public:
    explicit MatrixIter(Matrix *ptr, size_t line = 0, size_t colm = 0) :
            matrix_ptr_(ptr),
            curr_l_(line),
            curr_c_(colm) {}

    bool operator==(const MatrixIter &other) const;

    bool operator!=(const MatrixIter &other) const;

    const sptrObj &operator*() const;

    sptrObj &operator*();

    MatrixIter &operator++();

    MatrixIter operator++(int);
};


class Matrix : public Evaluable {
private:
    friend class MatrixIter;

    friend class ConstMatrixIter;

    std::vector<std::vector<sptrObj>> matrix_ = {};
    size_t lines_{}, columns_{};

    void ThrowIfNotValidMatrix() {
        for (size_t i = 0; i < lines_; ++i) {
            if (matrix_[i].size() != columns_) {
                throw SyntaxError("Matrix: invalid matrix given (count of elements in lines are not equal)\n");
            }
        }
    }

public:
    explicit Matrix(const std::vector<std::vector<sptrObj>> &table = {})
            : Evaluable(object_type::MatrixT),
              matrix_(table),
              lines_(matrix_.size()) {
        columns_ = !matrix_.empty() ? matrix_[0].size() : 0;
        ThrowIfNotValidMatrix();
    }

    explicit Matrix(std::vector<std::vector<sptrObj>> &&value)
            : Evaluable(object_type::MatrixT),
              matrix_(std::move(value)),
              lines_(matrix_.size()) {
        columns_ = !matrix_.empty() ? matrix_[0].size() : 0;
        ThrowIfNotValidMatrix();
    }

    explicit Matrix(size_t l, size_t c) : Evaluable(object_type::MatrixT), lines_(l), columns_(c) {
        matrix_.resize(l);
        for (size_t i = 0; i < l; ++i) {
            matrix_[i].resize(c);
        }
    }

    [[nodiscard]] std::pair<size_t, size_t> size() const;

    MatrixIter begin();

    MatrixIter end();

    ConstMatrixIter begin() const;

    ConstMatrixIter end() const;

private:
    void operator+=(const Matrix &);

    void operator-=(const Matrix &);

    void operator*=(const Matrix &);

    template<typename Ty>
    void operator*=(const Ty &scalar) requires std::is_base_of_v<Evaluable, Ty>;

    template<typename Ty>
    void operator/=(const Ty &scalar) requires std::is_base_of_v<Evaluable, Ty>;

public:
    std::shared_ptr<Evaluable> operator+(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator-(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator*(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator/(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> Transposed() const;

    void Transpose();

    std::vector<std::vector<sptrObj>> &GetArray();

    std::ostream &PrintOut(std::ostream &out) const;

    std::string GetString() override {
        std::string result;
        result.reserve((lines_ + 1) * (columns_ + 1));
        for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
            if (curr_l != 0) {
                result += '\n';
            }
            for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
                if (curr_c != 0) {
                    result += '\t';
                }
                result += matrix_[curr_l][curr_c]->GetString();
            }
        }
        return result;
    }
};

std::ostream &operator<<(std::ostream &out, const Matrix &m);

template<typename T, typename Ty>
std::shared_ptr<Evaluable> operator*(const Ty &scalar, const Matrix &matrix);


//#endif // MATLANG_MATRIX_H

