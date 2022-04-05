#include "integer.h"

int64_t Integer::Get() const {
    return value_;
}

void Integer::Set(int64_t value) {
    value_ = value;
}

std::string Integer::GetString() {
    return "<integer object: " + std::to_string(value_) + " >";
}

Integer Integer::operator+(const Integer &other) const {
    return {value_ + other.value_};
}

Integer &Integer::operator+=(const Integer &other) {
    value_ += other.value_;
    return *this;
}

Integer Integer::operator-(const Integer &other) const {
    return {value_ - other.value_};
}

Integer &Integer::operator-=(const Integer &other) {
    value_ -= other.value_;
    return *this;
}

Integer Integer::operator*(const Integer &other) const {
    return {value_ * other.value_};
}

Integer &Integer::operator*=(const Integer &other) {
    value_ *= other.value_;
    return *this;
}

Integer Integer::operator/(const Integer &other) const {
    return {value_ / other.value_};
}

Integer &Integer::operator/=(const Integer &other) {
    value_ /= other.value_;
    return *this;
}

std::ostream &operator<<(std::ostream &out, const Integer &value) {
    out << value;
    return out;
}
