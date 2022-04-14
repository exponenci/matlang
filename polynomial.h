#pragma once

#ifndef MATLANG_POLYNOMIAL_H
#define MATLANG_POLYNOMIAL_H

#include <iostream>
#include <map>
#include <vector>

template<typename T>
class Polynomial {
private:
    std::map<size_t, T> coefficients_;

    void Update();

public:
    Polynomial(const std::vector<T>& coeffs) {
        size_t degree = 0;
        for (auto it = coeffs.begin(); it != coeffs.end(); ++it) {
            if (*it != T{0}) {
                coefficients_[degree] = *it;
            }
            ++degree;
        }
    }

    Polynomial(const T& zero_coef = T()) {
        if (zero_coef != T{0}) {
            coefficients_[0] = zero_coef;
        }
    }

    template<typename It>
    explicit Polynomial(It begin, It end) {
        size_t degree = 0;
        for (auto it = begin; it != end; ++it) {
            if (*it != T{0}) {
                coefficients_[degree] = *it;
            }
            ++degree;
        }
    }

    typename std::map<size_t, T>::const_iterator begin() const;
    typename std::map<size_t, T>::const_iterator end() const;

    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

    T operator[](size_t ind) const;
    [[nodiscard]] int Degree() const;

    Polynomial<T>& operator*=(const Polynomial<T>& rhs);
    Polynomial<T>& operator+=(const Polynomial<T>& rhs);
    Polynomial<T>& operator-=(const Polynomial<T>& rhs);

    template<class Ty>
    friend Polynomial<Ty> operator*(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);
    template<class Ty>
    friend Polynomial<Ty> operator+(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);
    template<class Ty>
    friend Polynomial<Ty> operator-(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);

    T operator()(const T& value) const;
    Polynomial<T> operator&(const Polynomial<T>& other) const;

    template<class Ty>
    friend Polynomial<Ty> operator/(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);
    template<class Ty>
    friend Polynomial<Ty> operator%(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);
    template<class Ty>
    friend Polynomial<Ty> operator,(const Polynomial<Ty>& lhs, const Polynomial<Ty>& rhs);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p);

#endif //MATLANG_POLYNOMIAL_H
