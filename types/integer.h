#pragma once

#ifndef MATLANG_INTEGER_H
#define MATLANG_INTEGER_H

#include "object.h"

class Integer : public Object {
private:
    int64_t value_;

public:
    Integer(int64_t value) : Object(object_type::IntegerT), value_(value) {
    }

    int64_t Get() const;

    void Set(int64_t value);

    std::string GetString() override;

    Integer operator+(const Integer &other) const;

    Integer &operator+=(const Integer &other);

    Integer operator-(const Integer &other) const;

    Integer &operator-=(const Integer &other);

    Integer operator*(const Integer &other) const;

    Integer &operator*=(const Integer &other);

    Integer operator/(const Integer &other) const;

    Integer &operator/=(const Integer &other);
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Integer &value);


#endif //MATLANG_INTEGER_H
