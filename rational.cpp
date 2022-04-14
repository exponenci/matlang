#include "rational.h"

int64_t GCD(int64_t a, int64_t b) {
    if (b == 0) {
        return a;
    }
    return GCD(b, a % b);
}

std::pair<int64_t, int64_t> Simplify(int64_t a, int64_t b) {
    int64_t divider = 1;
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

Rational::Rational()
        : Evaluable(object_type::RationalT),
          numerator_(std::make_shared<Integer>(0)),
          denominator_(std::make_shared<Integer>(1)) {}

Rational::Rational(int64_t num)
        : Evaluable(object_type::RationalT),
          numerator_(std::make_shared<Integer>(num)),
          denominator_(std::make_shared<Integer>(1)) {}

Rational::Rational(int64_t num, int64_t denom)
        : Evaluable(object_type::IntegerT),
          numerator_(std::make_shared<Integer>(num)),
          denominator_(std::make_shared<Integer>(denom)) {
    if (denom == 0) {
        throw RuntimeError("Rational::Rational: zero-division error\n");
    }
    this->Update();
}

Rational::Rational(const std::shared_ptr<Evaluable>& num)
        : Evaluable(object_type::RationalT) {
    if (Is<Rational>(num)) {
        numerator_ = As<Rational>(num)->numerator_;
        denominator_ = As<Rational>(num)->denominator_;
    } else {
        throw RuntimeError("Rational::Rational: invalid constructor from Evaluable pointer\n");
    }
    if (denominator_->GetValue() == 0) {
        throw RuntimeError("Rational::Rational: zero-division error\n");
    }
}

std::shared_ptr<Evaluable> Rational::operator+(const std::shared_ptr<Evaluable> &rhs) const {
    Rational result(Numerator(), Denominator()), other(rhs);
    result.numerator_->SetValue(Numerator() * other.Denominator() + other.Numerator() * Denominator());
    result.denominator_->SetValue(Denominator() * other.Denominator());
    result.Update();
    return std::make_shared<Rational>(std::move(result));
}

std::shared_ptr<Evaluable> Rational::operator-(const std::shared_ptr<Evaluable> &rhs) const {
    Rational result(Numerator(), Denominator()), other(rhs);
    result.numerator_->SetValue(Numerator() * other.Denominator() - other.Numerator() * Denominator());
    result.denominator_->SetValue(Denominator() * other.Denominator());
    result.Update();
    return std::make_shared<Rational>(std::move(result));
}

std::shared_ptr<Evaluable> Rational::operator*(const std::shared_ptr<Evaluable> &rhs) const {
    Rational result(Numerator(), Denominator()), other(rhs);
    result.numerator_->SetValue(Numerator() * other.Numerator());
    result.denominator_->SetValue(Denominator() * other.Denominator());
    result.Update();
    return std::make_shared<Rational>(std::move(result));
}

std::shared_ptr<Evaluable> Rational::operator/(const std::shared_ptr<Evaluable> &rhs) const {
    Rational result(Numerator(), Denominator()), other(rhs);
    result.numerator_->SetValue(Numerator() * other.Denominator());
    result.denominator_->SetValue(Denominator() * other.Numerator());
    result.Update();
    return std::make_shared<Rational>(std::move(result));
}

std::string Rational::GetString() {
    if (Denominator() == 1) {
        return numerator_->GetString();
    }
    return numerator_->GetString() + "/" + denominator_->GetString();
}

int64_t Rational::Numerator() const {
    return numerator_->GetValue();
}

int64_t Rational::Denominator() const {
    if (numerator_->GetValue() == 0) {
        return 1;
    }
    return denominator_->GetValue();
}

std::shared_ptr<Evaluable> Rational::operator+() const {
    return std::make_shared<Rational>(Numerator(), Denominator());
}

std::shared_ptr<Evaluable> Rational::operator-() const {
    return std::make_shared<Rational>(-Numerator(), Denominator());
}

void Rational::Update() {
    auto[f, s] = Simplify(Numerator(), Denominator());
    if (s < 0) {
        numerator_->SetValue(-f);
        denominator_->SetValue(-s);
    } else {
        numerator_->SetValue(f);
        denominator_->SetValue(s);
    }
}
