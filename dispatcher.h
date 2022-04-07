#pragma  once

#include "object.h"
#include "matrix.h"
#include "integer.h"
#include "expression.h"
#include "comm.h"


#include <stack>
#include <map>
#include <memory>
#include <string>


#ifndef MATLANG_DISPATCHER_H
#define MATLANG_DISPATCHER_H


class Dispatcher;

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

    static bool IsArithmeticOperation(const std::shared_ptr<Object> &sptr) {
        if (!Is<Symbol>(sptr)) {
            return false;
        }
        std::string value = As<Symbol>(sptr)->GetString();
        return value == "+" || value == "-" || value == "*" || value == "/";
    }

public:
    static Dispatcher &Instance();

    [[nodiscard]] std::shared_ptr<Object> At(const std::string &variable) const;

    void InitObject(const std::string &, std::shared_ptr<Object>);

    std::shared_ptr<Object> Eval(std::list<std::shared_ptr<Object>> &args) {
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

    std::shared_ptr<Object> Invoke(const std::string &command, std::list<std::shared_ptr<Object>> &args) {
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

    bool IsSystemSymbol(const std::string&);
};


#endif //MATLANG_DISPATCHER_H
