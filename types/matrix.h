#pragma once

#ifndef MATLANG_MATRIX_H
#define MATLANG_MATRIX_H

#include <iostream>
#include <vector>

#include "object.h"

template<typename T>
class Matrix;


template<typename T>
class ConstMatrixIter {
private:
    const Matrix<T> *matrix_ptr_;
    size_t curr_l_, curr_c_;

public:
    explicit ConstMatrixIter(const Matrix<T> *ptr, size_t line = 0, size_t colm = 0) :
            matrix_ptr_(ptr),
            curr_l_(line),
            curr_c_(colm) {}

    bool operator==(const ConstMatrixIter<T> &other) const;

    bool operator!=(const ConstMatrixIter<T> &other) const;

    const T &operator*() const;

    ConstMatrixIter<T> &operator++();

    const ConstMatrixIter<T> operator++(int);
};


template<typename T>
class MatrixIter {
private:
    Matrix<T> *matrix_ptr_;
    size_t curr_l_, curr_c_;

public:
    explicit MatrixIter(Matrix<T> *ptr, size_t line = 0, size_t colm = 0) :
            matrix_ptr_(ptr),
            curr_l_(line),
            curr_c_(colm) {}

    bool operator==(const MatrixIter<T> &other) const;

    bool operator!=(const MatrixIter<T> &other) const;

    const T &operator*() const;

    T &operator*();

    MatrixIter<T> &operator++();

    const MatrixIter<T> operator++(int);
};


template<class T = sptrObj>
class Matrix : public Object {
private:
    friend class MatrixIter<T>;

    friend class ConstMatrixIter<T>;

    std::vector<std::vector<T>> matrix_ = {};
    size_t lines_{}, columns_{};

public:
    explicit Matrix(const std::vector<std::vector<T>> &table = {})
            : Object(object_type::MatrixT),
              matrix_(table),
              lines_(table.size()) {
        columns_ = !table.empty() ? table[0].size() : 0;
    }

    explicit Matrix(std::vector<std::vector<T>> &&value)
            : Object(object_type::MatrixT),
              matrix_(std::move(value)) {
        lines_ = matrix_.size();
        columns_ = !matrix_.empty() ? matrix_[0].size() : 0;
    }

    explicit Matrix(size_t l, size_t c) : Object(object_type::MatrixT), lines_(l), columns_(c) {
        matrix_.resize(l);
        for (size_t i = 0; i < l; ++i) {
            matrix_[i].resize(c);
        }
    }

    [[nodiscard]] std::pair<size_t, size_t> size() const;

    MatrixIter<T> begin();

    MatrixIter<T> end();

    ConstMatrixIter<T> begin() const;

    ConstMatrixIter<T> end() const;

    Matrix<T> &operator+=(const Matrix<T> &rhs);

    Matrix<T> operator+(const Matrix<T> &rhs) const;

    Matrix<T> &operator-=(const Matrix<T> &rhs);

    Matrix<T> operator-(const Matrix<T> &rhs) const;

    Matrix<T> operator*(const Matrix &other) const;

    Matrix<T> &operator*=(const Matrix &other);

    template<typename Ty>
    Matrix<T> &operator*=(const Ty &scalar) {
        for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
            for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
                matrix_[curr_l][curr_c] *= scalar;
            }
        }
        return *this;
    }

    template<typename Ty>
    Matrix<T> operator*(const Ty &scalar) const {
        Matrix<T> scalar_multiply(*this);
        scalar_multiply *= scalar;
        return scalar_multiply;
    }

    Matrix<T> Transposed() const;

    Matrix<T> &Transpose();

    std::vector<std::vector<T>> &GetArray() const;

    std::ostream &PrintOut(std::ostream &out) const;

    std::string GetString() override;
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &m);

template<typename T, typename Ty>
Matrix<T> operator*(const Ty &scalar, const Matrix<T> &matrix);

#endif // MATLANG_MATRIX_H