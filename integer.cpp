#include "integer.h"

Integer::Integer(int64_t value)
        : Evaluable(object_type::IntegerT),
          value_(value) {
}

Integer &Integer::operator=(int64_t value) {
    value_ = value;
    return *this;
}

int64_t Integer::GetValue() const {
    return value_;
}

void Integer::SetValue(int64_t value) {
    value_ = value;
}

std::string Integer::GetString() {
    return std::to_string(value_);
}

std::shared_ptr<Evaluable> Integer::operator+(const std::shared_ptr<Evaluable> &other) const {
    if (!Is<Integer>(other)) {
        throw SyntaxError("invalid argument: <integer> + <$invalid$>");
    }
    return std::make_shared<Integer>(value_ + As<Integer>(other)->value_);
}

std::shared_ptr<Evaluable> Integer::operator-(const std::shared_ptr<Evaluable> &other) const {
    if (!Is<Integer>(other)) {
        throw SyntaxError("invalid argument: <integer> - <$invalid$>");
    }
    return std::make_shared<Integer>(value_ - As<Integer>(other)->value_);
}

std::shared_ptr<Evaluable> Integer::operator*(const std::shared_ptr<Evaluable> &other) const {
    if (!Is<Integer>(other)) {
        throw SyntaxError("invalid argument: <integer> * <$invalid$>");
    }
    return std::make_shared<Integer>(value_ * As<Integer>(other)->value_);
}

std::shared_ptr<Evaluable> Integer::operator/(const std::shared_ptr<Evaluable> &other) const {
    if (!Is<Integer>(other)) {
        throw SyntaxError("invalid argument: <integer> / <$invalid$>");
    }
    return std::make_shared<Integer>(value_ / As<Integer>(other)->value_);
}

std::ostream &operator<<(std::ostream &out, const Integer &value) {
    out << value;
    return out;
}
