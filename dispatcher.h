#pragma  once
#ifndef MATLANG_DISPATCHER_H
#define MATLANG_DISPATCHER_H

#include "types/object.h"

#include <stack>
#include <functional>
#include <map>
#include <memory>
#include <string>


namespace cmd {
    enum cmd_type {
        Print,
        Transpose,
        Det,
        Inv,
        Rank,
        ToTriangle,
        ToDiag,
        Rrref,
        Init,
        Arithmetic
    };
}

class Dispatcher;

class BaseCommand {
private:
    cmd::cmd_type type_;

public:
    BaseCommand(cmd::cmd_type, Dispatcher *dp);

    virtual std::shared_ptr<Object> Run(std::list<std::shared_ptr<Object>> &&) = 0;

    virtual ~BaseCommand() = default;

    template<class T>
    std::shared_ptr<T> CastTo(std::shared_ptr<Object> &, bool = true);

    Dispatcher *dp_;
};

class ArithmeticCommand : public BaseCommand {
private:
    typedef std::shared_ptr<Object> sptrObj;
    std::function<sptrObj(sptrObj, sptrObj)> func_;

public:
    ArithmeticCommand(Dispatcher* dp, std::function<sptrObj(sptrObj, sptrObj)>&& f)
            : BaseCommand(cmd::cmd_type::Arithmetic, dp),
              func_(std::move(f)) {
    }

    std::shared_ptr<Object> Run(std::list<std::shared_ptr<Object>>&& args) override;
};


class Deleter {
private:
    friend Dispatcher;

    Dispatcher *held_dp_;

public:
    void Init(Dispatcher *held_dp);

    ~Deleter() noexcept;
};

class Dispatcher {
private:
    friend Deleter;

    static Dispatcher *instance;
    static Deleter deleter;

    std::map<std::string, std::shared_ptr<BaseCommand>> registers_;  // standard functions

    std::map<std::string, std::shared_ptr<Object>> variables_;  // user variables

    Dispatcher();

public:
    std::shared_ptr<Object> RunCommandLine(std::shared_ptr<Object>);

    static Dispatcher &Instance();

    std::shared_ptr<BaseCommand> operator()(const std::string &);

    [[nodiscard]] std::shared_ptr<Object> At(const std::string& variable) const;

    void Init(const std::string&, std::shared_ptr<Object>);

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


    std::shared_ptr<Object> Eval(std::list<std::shared_ptr<Object>>& args) {
        std::stack<std::shared_ptr<Object>> stack;
        for (auto &arg : args) {
            if (IsOperation(arg)){
                std::shared_ptr<Object> value_1 = stack.top();
                stack.pop();
                std::shared_ptr<Object> value_2 = stack.top();
                stack.pop();
                stack.push(Invoke(As<Symbol>(arg)->GetString(), value_2, value_1));
            } else {
                stack.push(arg);
            }
        }
        return stack.top();
    }

    std::shared_ptr<Object> Invoke(const std::string& command, std::list<std::shared_ptr<Object>>& args) {
        // TODO
        return nullptr;
    }
    std::shared_ptr<Object> Invoke(const std::string& command, std::shared_ptr<Object>& arg1, std::shared_ptr<Object>& arg2) {
        // TODO
        return nullptr;
    }

};


#endif //MATLANG_DISPATCHER_H
