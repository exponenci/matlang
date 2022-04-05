#include "matrix.h"

template<typename T>
bool ConstMatrixIter<T>::operator==(const ConstMatrixIter<T> &other) const {
    return curr_l_ == other.curr_l_ && curr_c_ == other.curr_c_;
}

template<typename T>
bool ConstMatrixIter<T>::operator!=(const ConstMatrixIter<T> &other) const {
    return !(*this == other);
}

template<typename T>
const T &ConstMatrixIter<T>::operator*() const {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

template<typename T>
ConstMatrixIter<T> &ConstMatrixIter<T>::operator++() {
    if (++curr_c_ == matrix_ptr_->columns_) {
        curr_c_ = 0;
        ++curr_l_;
    }
    return *this;
}

template<typename T>
const ConstMatrixIter<T> ConstMatrixIter<T>::operator++(int) {
    const ConstMatrixIter<T> copy_of(*this);
    ++*this;
    return copy_of;
}

template<typename T>
bool MatrixIter<T>::operator==(const MatrixIter<T> &other) const {
    return curr_l_ == other.curr_l_ && curr_c_ == other.curr_c_;
}

template<typename T>
bool MatrixIter<T>::operator!=(const MatrixIter<T> &other) const {
    return !(*this == other);
}

template<typename T>
const T &MatrixIter<T>::operator*() const {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

template<typename T>
T &MatrixIter<T>::operator*() {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

template<typename T>
MatrixIter<T> &MatrixIter<T>::operator++() {
    if (++curr_c_ == matrix_ptr_->columns_) {
        curr_c_ = 0;
        ++curr_l_;
    }
    return *this;
}

template<typename T>
const MatrixIter<T> MatrixIter<T>::operator++(int) {
    MatrixIter<T> copy_of(*this);
    ++(*this);
    return copy_of;
}


template<typename T>
[[nodiscard]] std::pair<size_t, size_t> Matrix<T>::size() const {
    return {lines_, columns_};
}

template<typename T>
MatrixIter<T> Matrix<T>::begin() {
    return MatrixIter<T>(this);
}

template<typename T>
MatrixIter<T> Matrix<T>::end() {
    return MatrixIter<T>(this, lines_);
}

template<typename T>
ConstMatrixIter<T> Matrix<T>::begin() const {
    return ConstMatrixIter<T>(this);
}

template<typename T>
ConstMatrixIter<T> Matrix<T>::end() const {
    return ConstMatrixIter<T>(this, lines_);
}

template<typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &rhs) {
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] += rhs.matrix_[curr_l][curr_c];
        }
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs) const {
    Matrix<T> sum(*this);
    sum += rhs;
    return sum;
}

template<typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &rhs) {
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] -= rhs.matrix_[curr_l][curr_c];
        }
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &rhs) const {
    Matrix<T> sum(*this);
    sum -= rhs;
    return sum;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &other) const {
    auto[other_lines, other_columns] = other.size();
    Matrix<T> multiply(lines_, other_columns);
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < other_columns; ++curr_c) {
            for (size_t curr_ind = 0; curr_ind < columns_; ++curr_ind) {
                multiply.matrix_[curr_l][curr_c] +=
                        matrix_[curr_l][curr_ind] * other.matrix_[curr_ind][curr_c];
            }
        }
    }
    return multiply;
}

template<typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix &other) {
    *this = *this * other;
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::Transposed() const {
    Matrix<T> new_matrix(columns_, lines_);
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            new_matrix.matrix_[curr_c][curr_l] = matrix_[curr_l][curr_c];
        }
    }
    return new_matrix;
}

template<typename T>
Matrix<T> &Matrix<T>::Transpose() {
    *this = this->Transposed();
    return *this;
}

template<typename T>
std::vector<std::vector<T>> &Matrix<T>::GetArray() const {
    return matrix_;
}

template<typename T>
std::ostream &Matrix<T>::PrintOut(std::ostream &out) const {
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        if (curr_l != 0) {
            out << '\n';
        }
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            if (curr_c != 0) {
                out << '\t';
            }
            out << matrix_[curr_l][curr_c];
        }
    }
    return out;
}

template<typename T>
std::string Matrix<T>::GetString() {
    return "<matrix object>";
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> &m) {
    return m.PrintOut(out);
}

template<typename T, typename Ty>
Matrix<T> operator*(const Ty &scalar, const Matrix<T> &matrix) {
    return matrix * scalar;
}
