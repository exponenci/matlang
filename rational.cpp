#include "types/include/rational.h"

int GCD(int a, int b) {
    if (b == 0) {
        return a;
    }
    return GCD(b, a % b);
}

std::pair<int, int> Simplify(int a, int b) {
    int divider = 1;
    if (b > 0) {
        if (a > 0) {
            divider = GCD(a, b);
        } else {
            divider = GCD(-a, b);
        }
    } else if (b < 0) {
        if (a > 0) {
            divider = GCD(a, -b);
        } else {
            divider = GCD(-a, -b);
        }
    }
    return {a / divider, b / divider};
}


int Rational::Numerator() const {
    return numerator_;
}

int Rational::Denominator() const {
    if (numerator_ == 0) {
        return 1;
    }
    return denominator_;
}

Rational Rational::operator+() const {
    return Rational(numerator_, denominator_);
}

Rational Rational::operator-() const {
    return Rational(-numerator_, denominator_);
}

Rational& Rational::operator++() {
    numerator_ += denominator_;
    return *this;
}

Rational Rational::operator++(int) {
    Rational return_value(*this);
    numerator_ += denominator_;
    return return_value;
}

Rational& Rational::operator--() {
    numerator_ -= denominator_;
    return *this;
}

Rational Rational::operator--(int) {
    Rational return_value(*this);
    numerator_ -= denominator_;
    return return_value;
}

bool Rational::operator==(const Rational& other) const {
    return (numerator_ == other.numerator_ &&
            denominator_ == other.denominator_);
}

bool Rational::operator!=(const Rational& other) const {
    return !(*this == other);
}

void Rational::Update() {
    auto [f, s] = Simplify(numerator_, denominator_);
    if (s < 0) {
        numerator_ = -f;
        denominator_ = -s;
    } else {
        numerator_ = f;
        denominator_ = s;
    }
}

Rational& operator+=(Rational& one, const Rational& other) {
    one.numerator_ = one.numerator_ * other.denominator_ + other.numerator_ * one.denominator_;
    one.denominator_ *= other.denominator_;
    one.Update();
    return one;
}

Rational& operator-=(Rational& one, const Rational& other) {
    one.numerator_ = one.numerator_ * other.denominator_ - other.numerator_ * one.denominator_;
    one.denominator_ *= other.denominator_;
    one.Update();
    return one;
}

Rational& operator*=(Rational& one, const Rational& other) {
    one.numerator_ *= other.numerator_;
    one.denominator_ *= other.denominator_;
    one.Update();
    return one;
}

Rational& operator/=(Rational& one, const Rational& other) {
    one.numerator_ *= other.denominator_;
    one.denominator_ *= other.numerator_;
    one.Update();
    return one;
}

Rational operator+(const Rational& one, const Rational& other) {
    Rational result(one);
    result += other;
    return result;
}

Rational operator-(const Rational& one, const Rational& other) {
    Rational result(one);
    result -= other;
    return result;
}

Rational operator*(const Rational& one, const Rational& other) {
    Rational result(one);
    result *= other;
    return result;
}

Rational operator/(const Rational& one, const Rational& other) {
    Rational result(one);
    result /= other;
    return result;
}