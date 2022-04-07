#include "dispatcher.h"
#include "error.h"


Dispatcher::Dispatcher() {
    registers_ = {
            {"+",         std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Matrix>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(lhs) + As<Evaluable>(rhs);
                        } else if (Is<Integer>(lhs) && Is<Integer>(rhs)) {
                            return *As<Integer>(lhs) + As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for summation\n");
                    })},
            {"-",         std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Matrix>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(lhs) - As<Evaluable>(rhs);
                        } else if (Is<Integer>(lhs) && Is<Integer>(rhs)) {
                            return *As<Integer>(lhs) - As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for subtraction\n");
                    })},
            {"*",         std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Integer>(lhs) && Is<Integer>(rhs)) {
                            return *As<Integer>(lhs) * As<Evaluable>(rhs);
                        } else if (Is<Matrix>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(lhs) * As<Evaluable>(rhs);
                        } else if (Is<Matrix>(lhs) && Is<Integer>(rhs)) {
                            return *As<Matrix>(lhs) * As<Evaluable>(rhs);
                        } else if (Is<Integer>(lhs) && Is<Matrix>(rhs)) {
                            return *As<Matrix>(rhs) * As<Evaluable>(lhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for multiply\n");
                    })},
            {"/",         std::make_shared<ArithmeticCommand>(
                    [&](const sptrObj &lhs, const sptrObj &rhs) -> sptrObj {
                        if (Is<Integer>(lhs) && Is<Integer>(rhs)) {
                            return *As<Integer>(lhs) / As<Evaluable>(rhs);
                        } else if (Is<Matrix>(lhs)) {
                            return *As<Matrix>(lhs) / As<Evaluable>(rhs);
                        }
                        throw RuntimeError("Dispatcher: invalid operands for division\n");
                    })},
            {"print",     std::make_shared<PrintCommand>()},
            {"transpose", std::make_shared<TransposeCommand>()}
    };
}

Dispatcher &Dispatcher::Instance() {
    if (!instance) {
        instance = new Dispatcher{};
        deleter.Init(instance);
    }
    return *instance;
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

Dispatcher *Dispatcher::instance = nullptr;
Deleter Dispatcher::deleter;

void Deleter::Init(Dispatcher *held_dp) {
    held_dp_ = held_dp;
}

Deleter::~Deleter() noexcept {
    delete held_dp_;
    held_dp_ = nullptr;
}

bool Dispatcher::IsSystemSymbol(const std::string& regname) {
    return registers_.contains(regname) || regname == ")" || regname == "(";
}