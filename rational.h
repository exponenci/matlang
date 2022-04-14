#pragma once

#include "object.h"
#include "integer.h"

#ifndef MATLANG_RATIONAL_H
#define MATLANG_RATIONAL_H


int64_t GCD(int64_t, int64_t);

std::pair<int64_t, int64_t> Simplify(int64_t, int64_t);

class Rational : public Evaluable {
private:
    std::shared_ptr<Integer> numerator_, denominator_;

public:
    Rational();

    Rational(int64_t);

    explicit Rational(int64_t, int64_t);

    Rational(const std::shared_ptr<Evaluable>&);

    std::shared_ptr<Evaluable> operator+(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator-(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator*(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator/(const std::shared_ptr<Evaluable> &) const override;

    std::string GetString() override;

    [[nodiscard]] int64_t Numerator() const;

    [[nodiscard]] int64_t Denominator() const;

    std::shared_ptr<Evaluable> operator+() const;

    std::shared_ptr<Evaluable> operator-() const;

    void Update();
};

#endif //MATLANG_RATIONAL_H
