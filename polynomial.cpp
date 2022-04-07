#include "types/include/polynomial.h"

template<typename T>
void Polynomial<T>::Update() {
    auto it = coefficients_.end();
    while (coefficients_.size() > 1 && (--it)->second == T{0}) {
        coefficients_.erase(it);
    }
}

template<typename T>
typename std::map<size_t, T>::const_iterator Polynomial<T>::begin() const {
    return coefficients_.begin();
}

template<typename T>
typename std::map<size_t, T>::const_iterator Polynomial<T>::end() const {
    return coefficients_.end();
}

template<typename T>
bool Polynomial<T>::operator==(const Polynomial& other) const {
    return coefficients_ == other.coefficients_;
}

template<typename T>
bool Polynomial<T>::operator!=(const Polynomial& other) const {
    return !(*this == other);
}

template<typename T>
T Polynomial<T>::operator[](size_t ind) const {
    if (static_cast<int>(ind) > Degree() || Degree() == -1) {
        return T{0};
    }
    if (coefficients_.find(ind) != coefficients_.end()) {
        return coefficients_.at(ind);
    }
    return T{0};
}

template<typename T>
[[nodiscard]] int Polynomial<T>::Degree() const {
    auto it = coefficients_.end();
    if (it == coefficients_.begin()) {
        return -1;
    }
    --it;
    if (it->first != 0) {
        return it->first;
    }
    if (coefficients_.find(0) != coefficients_.end()) {
        return 0;
    }
    return -1;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& rhs) {
    std::map<size_t, T> new_coefficients;
    for (const auto& [key_lhs, value_lhs] : coefficients_) {
        for (const auto& [key_rhs, value_rhs] : rhs.coefficients_) {
            if (new_coefficients[key_lhs + key_rhs] + value_lhs * value_rhs != T{0}) {
                new_coefficients[key_lhs + key_rhs] += value_lhs * value_rhs;
            } else {
                new_coefficients.erase(key_lhs + key_rhs);
            }
        }
    }
    coefficients_ = std::move(new_coefficients);
    Update();
    return *this;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& rhs) {
    size_t deg;
    size_t cmp1 = Degree() + 1, cmp2 = rhs.Degree() + 1;
    for (deg = 0; deg != std::min(cmp1, cmp2); ++deg) {
        auto rhs_it = rhs.coefficients_.find(deg);
        if (rhs_it != rhs.coefficients_.end()) {
            if (coefficients_[deg] + rhs[deg] != T{0}) {
                coefficients_[deg] += rhs.coefficients_.at(deg);
            } else {
                coefficients_.erase(deg);
            }
        }
    }
    if (cmp1 < cmp2) {
        for (; deg != cmp2; ++deg) {
            auto rhs_it = rhs.coefficients_.find(deg);
            if (rhs_it != rhs.coefficients_.end()) {
                if (coefficients_[deg] + rhs[deg] != T{0}) {
                    coefficients_[deg] += rhs[deg];
                } else {
                    coefficients_.erase(deg);
                }
            }
        }
    }
    Update();
    return *this;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& rhs) {
    *this += rhs * -1;
    return *this;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    Polynomial<T> multiply(lhs);
    multiply *= rhs;
    return multiply;
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    Polynomial sum(lhs);
    sum += rhs;
    return sum;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    Polynomial minus(lhs);
    minus -= rhs;
    return minus;
}

template<typename T>
T Polynomial<T>::operator()(const T& value) const {
    if (Degree() == -1) {
        return T{0};
    }
    size_t deg = static_cast<size_t>(Degree());
    T result{0};
    for (; deg != 0; --deg) {
        result *= value;
        if (coefficients_.find(deg) != coefficients_.end()) {
            result += coefficients_.at(deg);
        }
    }
    result *= value;
    if (coefficients_.find(deg) != coefficients_.end()) {
        result += coefficients_.at(deg);
    }
    return result;
}

template<typename T>
Polynomial<T> Polynomial<T>::operator&(const Polynomial<T>& other) const {
    Polynomial<T> composition(T{0});
    Polynomial<T> degree_holder{T{1}};
    for (size_t ind = 0; ind < static_cast<size_t>(Degree() + 1); ++ind) {
        if (coefficients_.find(ind) != coefficients_.end()) {
            composition += coefficients_.at(ind) * degree_holder;
        }
        degree_holder *= other;
    }
    composition.Update();
    return composition;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    if (lhs.Degree() < rhs.Degree() || lhs.Degree() == -1) {
        return Polynomial<T>{T{0}};
    }
    int new_degree = lhs.Degree() - rhs.Degree() + 1;
    Polynomial<T> initial(lhs);
    Polynomial<T> division;
    std::vector<T> partial_division;
    while (true) {
        if (new_degree <= 0 || initial.Degree() == -1) {
            division.Update();
            return division;
        }
        partial_division.resize(new_degree);
        T multiplicative = (--initial.coefficients_.end())->second /
                           (--rhs.coefficients_.end())->second;
        if (initial.Degree() == rhs.Degree()) {
            division += multiplicative;
            division.Update();
            return division;
        }
        partial_division.back() = multiplicative;
        for (int i = initial.Degree(); i >= new_degree - 1; --i) {
            if (initial[i] - multiplicative * rhs[i - new_degree + 1] != T{0}) {
                initial.coefficients_[i] -= multiplicative * rhs[i - new_degree + 1];
            } else {
                initial.coefficients_.erase(i);
            }
        }
        division += Polynomial<T>(partial_division);
        initial.Update();
        new_degree = initial.Degree() - rhs.Degree() + 1;
    }
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    return lhs - ((lhs / rhs) * rhs);
}

template<typename T>
Polynomial<T> operator,(const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
    if (lhs.Degree() < 0) {
        if (rhs.Degree() > -1) {
            return rhs / rhs[rhs.Degree()];
        }
        return rhs;
    }
    return (rhs % lhs, lhs);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
    if (p.Degree() == -1) {
        out << T(0);
        return out;
    }
    if (p.Degree() == 0) {
        out << p[0];
        return out;
    }
    size_t degree = static_cast<size_t>(p.Degree());
    if (p[degree] == T{1}) {
        out << "x";
    } else if (p[degree] == T{-1}) {
        out << "-x";
    } else if (p[degree] != T{0}) {
        out << p[degree] << "*x";
    }
    if (degree != 1) {
        out << '^' << degree;
    }
    --degree;
    for (; degree >= 1; --degree) {
        if (p[degree] == T{0}) {
            continue;
        }
        if (p[degree] == T{1}) {
            out << "+x";
        } else if (p[degree] == T{-1}) {
            out << "-x";
        } else if (p[degree] > T{0}) {
            out << '+' << p[degree] << "*x";
        } else {
            out << p[degree] << "*x";
        }
        if (degree != 1) {
            out << '^' << degree;
        }
    }
    if (p[degree] != T{0}) {
        if (p[degree] > T{0}) {
            out << '+' << p[degree];
        } else {
            out << p[degree];
        }
    }
    return out;
}
