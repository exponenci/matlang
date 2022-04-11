#pragma once

#include "object.h"

#ifndef MATLANG_INTEGER_H
#define MATLANG_INTEGER_H


class Integer : public Evaluable {
private:
    int64_t value_;

public:
    Integer(int64_t);

    Integer &operator=(int64_t);

    int64_t GetValue() const;

    void SetValue(int64_t);

    std::string GetString() override;

    std::shared_ptr<Evaluable> operator+(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator-(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator*(const std::shared_ptr<Evaluable> &) const override;

    std::shared_ptr<Evaluable> operator/(const std::shared_ptr<Evaluable> &) const override;
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const Integer &value);

#endif //MATLANG_INTEGER_H
