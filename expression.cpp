#include "expression.h"


Expression::Expression()
        : Object(object_type::ExpressionT) {
}

Expression::Expression(std::list<sptrObj> &&data)
        : Object(object_type::ExpressionT),
          args_(std::move(data)) {
}

int Expression::Priority(const std::shared_ptr<Object> &sptr) {
    if (IsSymbolEqual(sptr, "+") || IsSymbolEqual(sptr, "-")) {
        return 1;
    }
    if (IsSymbolEqual(sptr, "*") || IsSymbolEqual(sptr, "/")) {
        return 2;
    }
    return 0;
}

bool Expression::IsOperation(const std::shared_ptr<Object> &sptr) {
    if (!Is<Symbol>(sptr)) {
        return false;
    }
    std::string value = As<Symbol>(sptr)->GetString();
    return value == "+" || value == "-" || value == "*" || value == "/";
}

bool Expression::IsSymbolEqual(const std::shared_ptr<Object> &sptr, std::string_view sv) {
    return Is<Symbol>(sptr) && As<Symbol>(sptr)->GetString() == sv;
}

std::string Expression::GetString() {
    return "<expression object>";
}

size_t Expression::GetSize() const {
    return args_.size();
}

void Expression::AddArg(sptrObj &&arg) {
    args_.push_back(std::move(arg));
}

std::list<sptrObj> &Expression::GetArgs() {
    return args_;
}

void Expression::FormatInfix() {
    if (IsSymbolEqual(args_.front(), "+") || IsSymbolEqual(args_.front(), "-")) {
        args_.push_front(std::make_shared<Rational>(0));
    }
    for (auto it = args_.begin(); it != args_.end(); ++it) {
        auto floating_it = it;
        if (IsSymbolEqual(*it, "(") && IsOperation(*++floating_it) && Is<Evaluable>(*++floating_it)) {
            if (IsSymbolEqual(*--floating_it, "+") || IsSymbolEqual(*floating_it, "-")) {
                args_.insert(floating_it, std::make_shared<Rational>(0));
            } else {
                throw RuntimeError("Format: invalid operation was received\n");
            }
        }
    }
}

void Expression::Infix2Postfix() {
    FormatInfix();
    std::list<std::shared_ptr<Object>> postfix;
    std::stack<std::shared_ptr<Object>> s;
    for (auto &arg: args_) {
        if (IsSymbolEqual(arg, "(")) { // if opening bracket then push the stack
            s.push(arg);
        } else if (IsSymbolEqual(arg, ")")) {       // if closing bracket encounted then keep popping from stack until
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
        } else if (Is<Symbol>(arg) || Is<Evaluable>(arg)) {
            postfix.push_back(arg);
        }
    }
    while (!s.empty()) {
        postfix.push_back(s.top());
        s.pop();
    }
    args_ = std::move(postfix);
}
