#include "types/include/mcomplex.h"

double Complex::Re() const {
    return real_;
}

double Complex::Im() const {
    return imag_;
}

Complex Complex::operator+() const {
    return Complex{real_, imag_};
}

Complex Complex::operator-() const {
    return Complex{-real_, -imag_};
}

bool Complex::operator==(const Complex& other) const {
    return real_ == other.real_ && imag_ == other.imag_;
}

bool Complex::operator!=(const Complex& other) const {
    return real_ != other.real_ || imag_ != other.imag_;
}

[[nodiscard]] double Complex::Abs() const {
    return std::sqrt(real_ * real_ + imag_ * imag_);
}

Complex operator+(const Complex& one, const Complex& other) {
    return Complex{one.real_ + other.real_, one.imag_ + other.imag_};
}

Complex operator-(const Complex& one, const Complex& other) {
    return Complex{one.real_ - other.real_, one.imag_ - other.imag_};
}

Complex operator*(const Complex& one, const Complex& other) {
    return Complex{one.real_ * other.real_ - one.imag_ * other.imag_,
                   one.real_ * other.imag_ + one.imag_ * other.real_};
}

Complex operator/(const Complex& one, const Complex& other) {
    double divider = other.real_ * other.real_ + other.imag_ * other.imag_;
    return Complex{(one.real_ * other.real_ + one.imag_ * other.imag_) / divider,
                   (one.imag_ * other.real_ - one.real_ * other.imag_) / divider};
}
