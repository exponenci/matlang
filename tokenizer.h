#pragma once

#include <variant>
#include <optional>
#include <istream>

#ifndef MATLANG_TOKENIZER_H
#define MATLANG_TOKENIZER_H


enum class BracketToken { OPEN, CLOSE }; // [] (do i need some tokens for {} ()?)

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

using Token =
std::variant<BracketToken, SymbolToken, ConstantToken, SemicolonToken>;

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
    int ReadNumber(); // reads number till first non-digit symbol

    std::string ReadSymbol(); // reads string-type value till first space symbol

    void ClearSpace(); // reads space symbols till first non-space symbol

    bool OnEOF(); // returns true on eof

    constexpr static bool IsSpecialSymbol(int); // special symbols reserved by system
    constexpr static bool IsProhibitedSymbol(int); // prohibited by language syntax symbols

    // now we parse strings like "-a" as expression with 2 objects: "-" and value corresponding to variable "a"
    // TODO fix this in Expression when generate postfix from infix and/or when evaluate expression via Dispatcher::Eval
};

#endif //MATLANG_TOKENIZER_H
