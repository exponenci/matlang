#pragma once

#include <stack>

#include "object.h"
#include "matrix.h"
#include "integer.h"

#ifndef MATLANG_EXPRESSION_H
#define MATLANG_EXPRESSION_H


class Expression : public Object {
private:
    std::list<sptrObj> args_;

public:
    Expression();

    explicit Expression(std::list<sptrObj> &&);

private:
    static int Priority(const std::shared_ptr<Object> &);

    static bool IsOperation(const std::shared_ptr<Object> &);

    static bool IsSymbolEqual(const std::shared_ptr<Object> &, std::string_view);

public:
    std::string GetString() override;

    size_t GetSize() const;

    void AddArg(sptrObj &&arg);

    std::list<sptrObj> &GetArgs();

    void FormatInfix();
    void Infix2Postfix();
};

#endif //MATLANG_EXPRESSION_H
