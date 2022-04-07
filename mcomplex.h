#pragma once

#ifndef MATLANG_MCOMPLEX_H
#define MATLANG_MCOMPLEX_H

#include <cmath>

class Complex {
private:
    double real_, imag_;

public:
    Complex(const double& re = 0, const double& im = 0) : real_(re), imag_(im) {}

    [[nodiscard]] double Re() const;
    [[nodiscard]] double Im() const;

    friend Complex operator+(const Complex&, const Complex&);
    friend Complex operator-(const Complex&, const Complex&);
    friend Complex operator*(const Complex&, const Complex&);
    friend Complex operator/(const Complex&, const Complex&);

    Complex operator+() const;
    Complex operator-() const;

    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    [[nodiscard]] double Abs() const;
};

#endif //MATLANG_MCOMPLEX_H
