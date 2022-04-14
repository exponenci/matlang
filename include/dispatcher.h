#pragma  once

#include "error.h"
#include "object.h"
#include "rational.h"
#include "matrix.h"
#include "expression.h"
#include "comm.h"

#include <map>
#include <memory>
#include <stack>
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

    static bool IsArithmeticOperation(const std::shared_ptr<Object> &);

public:
    static Dispatcher &Instance();

    bool IsRegisteredSymbol(const std::string &);

    [[nodiscard]] std::shared_ptr<Object> At(const std::string &) const;

    void InitObject(const std::string &, std::shared_ptr<Object>);

    std::shared_ptr<Object> Eval(std::list<std::shared_ptr<Object>> &);

    std::shared_ptr<Object> Invoke(const std::string &, std::list<std::shared_ptr<Object>> &);

    void SetCommand(const std::string&, std::shared_ptr<BaseCommand>);
};


#endif //MATLANG_DISPATCHER_H
