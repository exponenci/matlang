#pragma once

#ifndef MATLANG_TOKENIZER_H
#define MATLANG_TOKENIZER_H

#include <variant>
#include <optional>
#include <istream>


enum class BracketToken { OPEN, CLOSE }; // []

struct SymbolToken {
    std::string name_;

    SymbolToken(int char_code) : name_(1, static_cast<char>(char_code)) {
    }

    SymbolToken(const std::string& s) : name_(s) {
    }

    bool operator==(const SymbolToken& other) const;
};

struct ConstantToken {
    int value_;

    ConstantToken(int value) : value_(value) {
    }

    bool operator==(const ConstantToken& other) const;
};

struct SemicolonToken {
    bool operator==(const SemicolonToken&) const;
};

struct DivisionToken {
    bool operator==(const DivisionToken&) const;
};

using Token =
std::variant<BracketToken, SymbolToken, ConstantToken, SemicolonToken, DivisionToken>;

class Tokenizer {
private:
    Token curr_token_;
    std::istream* in_;
    bool already_read_ = false;

public:
    // Создаёт токенизатор, читающий символы из потока in.
    explicit Tokenizer(std::istream*);

    // Достигли мы конца потока или нет.
    bool IsEnd();

    // Попытаться прочитать следующий токен.
    // Либо IsEnd() станет true, либо токен можно будет получить через GetToken().
    void Next();

    // Получить текущий токен.
    Token GetToken();

private:
    int ReadNumber();

    std::string ReadSymbol();

    void ClearSpace();

    bool OnEOF();

    static bool IsValidStringBegin(int);

    static bool IsSpecialSymbol(int);
};

#endif //MATLANG_TOKENIZER_H
