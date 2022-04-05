#pragma once

#ifndef MATLANG_RATIONAL_H
#define MATLANG_RATIONAL_H

#include <vector>

int GCD(int a, int b);

std::pair<int, int> Simplify(int a, int b);

class Rational {
private:
    int numerator_, denominator_;

public:
    Rational() : numerator_(0), denominator_(1) {}

    Rational(int num) : numerator_(num), denominator_(1) {}

    explicit Rational(int num, int denom) : numerator_(num), denominator_(denom) {
        this->Update();
    }

    [[nodiscard]] int Numerator() const;
    [[nodiscard]] int Denominator() const;

    friend Rational operator+(const Rational&, const Rational&);
    friend Rational operator-(const Rational&, const Rational&);
    friend Rational operator*(const Rational&, const Rational&);
    friend Rational operator/(const Rational&, const Rational&);

    friend Rational& operator+=(Rational&, const Rational&);
    friend Rational& operator-=(Rational&, const Rational&);
    friend Rational& operator*=(Rational&, const Rational&);
    friend Rational& operator/=(Rational&, const Rational&);

    Rational operator+() const;
    Rational operator-() const;

    Rational& operator++();
    Rational operator++(int);
    Rational& operator--();
    Rational operator--(int);

    bool operator==(const Rational& other) const;
    bool operator!=(const Rational& other) const;
    void Update();
};

#endif //MATLANG_RATIONAL_H
