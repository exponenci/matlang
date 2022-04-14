#include "dispatcher.h"


Dispatcher::Dispatcher() {
    registers_ = {
            {"+",           std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Matrix>(lhs)) {
                            return *As<Matrix>(lhs) + As<Evaluable>(rhs);
                        } else if (Is<Rational>(lhs) && Is<Rational>(rhs)) {
                            return *As<Rational>(lhs) + As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for summation\n");
                    })},
            {"-",           std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Matrix>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(lhs) - As<Evaluable>(rhs);
                        } else if (Is<Rational>(lhs) && Is<Rational>(rhs)) {
                            return *As<Rational>(lhs) - As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for subtraction\n");
                    })},
            {"*",           std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Rational>(lhs) && Is<Rational>(rhs)) {
                            return *As<Rational>(lhs) * As<Evaluable>(rhs);
                        } else if (Is<Matrix>(lhs)) {
                            return *As<Matrix>(lhs) * As<Evaluable>(rhs);
                        } else if (Is<Rational>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(rhs) * As<Evaluable>(lhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for multiply\n");
                    })},
            {"/",           std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Rational>(lhs) && Is<Rational>(rhs)) {
                            return *As<Rational>(lhs) / As<Evaluable>(rhs);
                        } else if (Is<Matrix>(lhs)) {
                            return *As<Matrix>(lhs) / As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for division\n");
                    })},
            {"print",       std::make_shared<PrintCommand>()},
            {"transpose",   std::make_shared<TransposeCommand>()},
            {"rref",        std::make_shared<LinearTransformationCommand>(cmd::rref)},
            {"to_diag",     std::make_shared<LinearTransformationCommand>(cmd::to_diag)},
            {"to_triangle", std::make_shared<LinearTransformationCommand>(cmd::to_triangle)},
            {"inv",         std::make_shared<LinearTransformationCommand>(cmd::inv)},
            {"det",         std::make_shared<LinearTransformationCommand>(cmd::det)},
            {"rank",        std::make_shared<LinearTransformationCommand>(cmd::rank)},
    };
}


bool Dispatcher::IsArithmeticOperation(const std::shared_ptr<Object> &sptr) {
    if (!Is<Symbol>(sptr)) {
        return false;
    }
    std::string value = As<Symbol>(sptr)->GetString();
    return value == "+" || value == "-" || value == "*" || value == "/";
}

Dispatcher &Dispatcher::Instance() {
    if (!instance) {
        instance = new Dispatcher{};
        deleter.Init(instance);
    }
    return *instance;
}

bool Dispatcher::IsRegisteredSymbol(const std::string &regname) {
    return instance->registers_.contains(regname) || regname == ")" || regname == "(";
}

std::shared_ptr<Object> Dispatcher::At(const std::string &varname) const {
    if (!instance->variables_.contains(varname)) {
        return nullptr;
    }
    return instance->variables_.at(varname);
}

void Dispatcher::InitObject(const std::string &varname, std::shared_ptr<Object> sptr) {
    if (instance->registers_.contains(varname)) {
        throw NameError("Dispatcher: invalid name for object initializing (this string is reserved by language)\n");
    }
    instance->variables_[varname] = std::move(sptr);
}

std::shared_ptr<Object> Dispatcher::Eval(std::list<std::shared_ptr<Object>> &args) {
    std::stack<std::shared_ptr<Object>> stack;
    for (auto &arg: args) {
        if (IsArithmeticOperation(arg)) {
            std::shared_ptr<Object> value_1 = stack.top();
            stack.pop();
            std::shared_ptr<Object> value_2 = stack.top();
            stack.pop();
            std::list<std::shared_ptr<Object>> args_list{value_2, value_1};
            stack.push(Invoke(As<Symbol>(arg)->GetString(), args_list));
        } else {
            stack.push(arg);
        }
    }
    return stack.top();
}

std::shared_ptr<Object> Dispatcher::Invoke(const std::string &command, std::list<std::shared_ptr<Object>> &args) {
    if (command == "init") {
        if (args.size() != 2) {
            throw RuntimeError("Dispatcher: invalid arguments were provided for initialization\n");
        }
        InitObject(As<Symbol>(args.front())->GetString(), args.back());
        return instance->variables_.at(As<Symbol>(args.front())->GetString());
    }
    if (!instance->registers_.contains(command)) {
        throw NameError("Dispatcher: function not found\n");
    }
    return instance->registers_.at(command)->Run(args);
}


Dispatcher *Dispatcher::instance = nullptr;
Deleter Dispatcher::deleter;

void Deleter::Init(Dispatcher *held_dp) {
    held_dp_ = held_dp;
}

Deleter::~Deleter() noexcept {
    delete held_dp_;
    held_dp_ = nullptr;
}

