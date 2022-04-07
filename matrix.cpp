#include "matrix.h"

// CONST_MATRIX_ITER

bool ConstMatrixIter::operator==(const ConstMatrixIter &other) const {
    return curr_l_ == other.curr_l_ && curr_c_ == other.curr_c_;
}

bool ConstMatrixIter::operator!=(const ConstMatrixIter &other) const {
    return !(*this == other);
}

const sptrObj &ConstMatrixIter::operator*() const {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

ConstMatrixIter &ConstMatrixIter::operator++() {
    if (++curr_c_ == matrix_ptr_->columns_) {
        curr_c_ = 0;
        ++curr_l_;
    }
    return *this;
}

ConstMatrixIter ConstMatrixIter::operator++(int) {
    const ConstMatrixIter copy_of(*this);
    ++*this;
    return copy_of;
}

//MATRIX_ITER
bool MatrixIter::operator==(const MatrixIter &other) const {
    return curr_l_ == other.curr_l_ && curr_c_ == other.curr_c_;
}

bool MatrixIter::operator!=(const MatrixIter &other) const {
    return !(*this == other);
}

const sptrObj &MatrixIter::operator*() const {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

sptrObj &MatrixIter::operator*() {
    return matrix_ptr_->matrix_[curr_l_][curr_c_];
}

MatrixIter &MatrixIter::operator++() {
    if (++curr_c_ == matrix_ptr_->columns_) {
        curr_c_ = 0;
        ++curr_l_;
    }
    return *this;
}

MatrixIter MatrixIter::operator++(int) {
    MatrixIter copy_of(*this);
    ++(*this);
    return copy_of;
}


// MATRIX

[[nodiscard]] std::pair<size_t, size_t> Matrix::size() const {
    return {lines_, columns_};
}

MatrixIter Matrix::begin() {
    return MatrixIter(this);
}

MatrixIter Matrix::end() {
    return MatrixIter(this, lines_);
}

ConstMatrixIter Matrix::begin() const {
    return ConstMatrixIter(this);
}

ConstMatrixIter Matrix::end() const {
    return ConstMatrixIter(this, lines_);
}

void Matrix::operator+=(const Matrix &rhs) {
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] =
                    *As<Evaluable>(matrix_[curr_l][curr_c]) + As<Evaluable>(rhs.matrix_[curr_l][curr_c]);
        }
    }
}

void Matrix::operator-=(const Matrix &rhs) {
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] =
                    *As<Evaluable>(matrix_[curr_l][curr_c]) - As<Evaluable>(rhs.matrix_[curr_l][curr_c]);
        }
    }
}

void Matrix::operator*=(const Matrix &other) {
    auto[other_lines, other_columns] = other.size();
    if (columns_ != other_lines) {
        throw SyntaxError("Matrix::operator*=: invalid matrices sizes");
    }
    std::vector<std::vector<sptrObj>> new_data;
    new_data.resize(lines_);
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        new_data[curr_l].resize(other_columns);
        for (size_t curr_c = 0; curr_c < other_columns; ++curr_c) {
            new_data[curr_l][curr_c] = std::make_shared<Integer>(0);
            for (size_t curr_ind = 0; curr_ind < columns_; ++curr_ind) {
                new_data[curr_l][curr_c] = *As<Evaluable>(new_data[curr_l][curr_c]) + As<Evaluable>(
                        *As<Evaluable>(matrix_[curr_l][curr_ind]) * As<Evaluable>(other.matrix_[curr_ind][curr_c]));
            }
        }
    }
    matrix_ = std::move(new_data);
    columns_ = other_columns;
}

std::shared_ptr<Evaluable> Matrix::operator+(const std::shared_ptr<Evaluable> &rhs) const {
    if (!Is<Matrix>(rhs)) {
        throw RuntimeError("Matrix::operator+: invalid operand type");
    }
    Matrix sum(*this);
    sum += *As<Matrix>(rhs);
    return std::make_shared<Matrix>(std::move(sum));
}

std::shared_ptr<Evaluable> Matrix::operator-(const std::shared_ptr<Evaluable> &rhs) const {
    if (!Is<Matrix>(rhs)) {
        throw RuntimeError("Matrix::operator-: invalid operand type");
    }
    Matrix sub(*this);
    sub -= *As<Matrix>(rhs);
    return std::make_shared<Matrix>(std::move(sub));
}

std::shared_ptr<Evaluable> Matrix::operator*(const std::shared_ptr<Evaluable> &other) const {
    Matrix multiply(*this);
    if (Is<Matrix>(other)) {
        multiply *= *As<Matrix>(other);
    } else if (Is<Integer>(other)) {
        multiply *= *As<Integer>(other);
    } else {
        throw RuntimeError("Matrix::operator*: invalid operand type");
    }
    return std::make_shared<Matrix>(std::move(multiply));
}
std::shared_ptr<Evaluable> Matrix::operator/(const std::shared_ptr<Evaluable> &other) const {
    Matrix division(*this);
    if (Is<Integer>(other)) {
        division /= *As<Integer>(other);
    } else {
        throw RuntimeError("Matrix::operator/: invalid operand type");
    }
    return std::make_shared<Matrix>(std::move(division));
}

template<typename Ty>
void Matrix::operator/=(const Ty &scalar) requires std::is_base_of_v<Evaluable, Ty> { // Ty = Integer
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] =
                    scalar / As<Evaluable>(matrix_[curr_l][curr_c]); // can be extended to Rational
        }
    }
}

template<typename Ty>
void Matrix::operator*=(const Ty &scalar) requires std::is_base_of_v<Evaluable, Ty> { // Ty = Integer
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            matrix_[curr_l][curr_c] =
                    scalar * As<Evaluable>(matrix_[curr_l][curr_c]); // can be extended to Rational
        }
    }
}

std::shared_ptr<Evaluable> Matrix::Transposed() const {
    Matrix new_matrix(columns_, lines_);
    for (size_t curr_l = 0; curr_l < lines_; ++curr_l) {
        for (size_t curr_c = 0; curr_c < columns_; ++curr_c) {
            new_matrix.matrix_[curr_c][curr_l] = matrix_[curr_l][curr_c];
        }
    }
    return std::make_shared<Matrix>(std::move(new_matrix));
}

void Matrix::Transpose() {
    *this = *As<Matrix>(this->Transposed());
}

std::vector<std::vector<sptrObj>> &Matrix::GetArray() {
    return matrix_;
}

std::ostream &Matrix::PrintOut(std::ostream &out) const {
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

std::ostream &operator<<(std::ostream &out, const Matrix &m) {
    return m.PrintOut(out);
}

template<typename Ty>
std::shared_ptr<Evaluable> operator*(const Ty &scalar, const Matrix &matrix) {
    return matrix * scalar;
}
