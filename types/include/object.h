#pragma once

#include <memory>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "error.h"

#ifndef MATLANG_OBJECT_H
#define MATLANG_OBJECT_H


enum object_type {
    ObjectT,
    NoneT,
    SymbolT,
    CommandT,
    ExpressionT,
    EvaluableT,
    IntegerT,
    MatrixT,
    RationalT,
};

class Object : public std::enable_shared_from_this<Object> {
private:
    object_type type_;

public:
    Object(object_type type = object_type::ObjectT) : type_(type) {
    }

    virtual ~Object() = default;

    virtual std::string GetString() = 0;
};

typedef std::shared_ptr<Object> sptrObj;

template<class T>
std::shared_ptr<T> As(const sptrObj &obj) {
    return std::static_pointer_cast<T>(obj);
}

template<class T>
bool Is(const sptrObj &obj) {
    return static_cast<bool>(dynamic_cast<T *>(obj.get()));
}

class NoneObject : public Object {
public:
    NoneObject() : Object(object_type::NoneT) {
    }

    std::string GetString() override {
        return "<NONE>";
    };
};

class Evaluable : public Object {
public:
    explicit Evaluable(object_type type = object_type::EvaluableT) : Object(type) {}

    virtual std::shared_ptr<Evaluable> operator+(const std::shared_ptr<Evaluable> &) const = 0;

    virtual std::shared_ptr<Evaluable> operator-(const std::shared_ptr<Evaluable> &) const = 0;

    virtual std::shared_ptr<Evaluable> operator*(const std::shared_ptr<Evaluable> &) const = 0;

    virtual std::shared_ptr<Evaluable> operator/(const std::shared_ptr<Evaluable> &) const = 0;
};

class CommandObject : public Object {
private:
    sptrObj cmd_ptr_;  // ptr to next object
    std::list<sptrObj> args_;

public:
    CommandObject() : Object(object_type::CommandT), cmd_ptr_(nullptr) {
    }

    void SetCommand(sptrObj cmd) {
        cmd_ptr_ = std::move(cmd);
    }

    sptrObj GetCommand() {
        return cmd_ptr_;
    }

    void SetArgs(std::list<sptrObj> &&args) {
        args_ = std::move(args);
    }

    void AddArg(sptrObj &&arg) {
        args_.push_back(std::move(arg));
    }

    std::list<sptrObj> &GetArgs() {
        return args_;
    }

    std::string GetString() override {
        std::string str = cmd_ptr_->GetString() + "(";
        for (const auto &ptr: args_) {
            str += ptr->GetString() + ", ";
        }
        return str + ")";
    }
};


class Symbol : public Object {
private:
    std::string name_;

public:
    Symbol(const std::string &name) : Object(object_type::SymbolT), name_(name) {
    }

    std::string GetString() override {
        return name_;
    };
};

#endif //MATLANG_OBJECT_H
