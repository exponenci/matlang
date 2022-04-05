#pragma once

#ifndef MATLANG_EXPRESSION_H
#define MATLANG_EXPRESSION_H

#include <stack>

#include "object.h"
#include "matrix.h"
#include "integer.h"

class Expression : public Object {
private:
    std::list<sptrObj> args_;

public:
    Expression() : Object(object_type::ExpressionT) {
    }

    explicit Expression(std::list<sptrObj> &&data) : Object(object_type::ExpressionT), args_(std::move(data)) {
    }

    void AddArg(sptrObj &&arg);

    size_t GetSize() const;

    std::list<sptrObj> &GetArgs() const;

    std::string GetString() override;

private:
    static int Priority(const std::shared_ptr<Object>& sptr) {
        if (IsSymbolEqual(sptr, "+")  || IsSymbolEqual(sptr, "-")) {
            return 1;
        }
        if (IsSymbolEqual(sptr, "*") || IsSymbolEqual(sptr, "/")) {
            return 2;
        }
        return 0;
    }

    static bool IsOperation(const std::shared_ptr<Object>& sptr) {
        if (!Is<Symbol>(sptr)) {
            return false;
        }
        std::string value = As<Symbol>(sptr)->GetString();
        return value == "+" || value == "-" || value == "*" || value == "/";
    }

    static bool IsSymbolEqual(const std::shared_ptr<Object>& sptr, std::string_view sv) {
        return Is<Symbol>(sptr) && As<Symbol>(sptr)->GetString() == sv;
    }

public:
    void Infix2Postfix() {
        std::list<std::shared_ptr<Object>> postfix;
        std::stack<std::shared_ptr<Object>> s;
        for (auto &arg: args_) {
            if (IsSymbolEqual(arg, "(")) { // if opening bracket then push the stack
                s.push(arg);
            } else if (IsSymbolEqual(arg, ")")) {            // if closing bracket encounted then keep popping from stack until
                while (!IsSymbolEqual(s.top(), "(")) {
                    postfix.push_back(s.top());
                    s.pop();
                }
                s.pop();
            } else if (IsOperation(arg)) {
                while (!s.empty() && Priority(arg) <= Priority(s.top())) {
                    postfix.push_back(s.top());
                    s.pop();
                }
                s.push(arg);
            } else if (Is<Symbol>(arg) || Is<Integer>(arg)) {
                postfix.push_back(arg);
            }
        }
        while (!s.empty()) {
            postfix.push_back(s.top());
            s.pop();
        }
        args_ = std::move(postfix);
    }
};

#endif //MATLANG_EXPRESSION_H
