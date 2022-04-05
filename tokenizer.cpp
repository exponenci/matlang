#include "tokenizer.h"
#include "error.h"

bool SymbolToken::operator==(const SymbolToken &other) const {
    return name_ == other.name_;
}

bool ConstantToken::operator==(const ConstantToken &other) const {
    return value_ == other.value_;
}

bool SemicolonToken::operator==(const SemicolonToken &) const {
    return true;
}

bool DivisionToken::operator==(const DivisionToken &) const {
    return true;
}

Tokenizer::Tokenizer(std::istream *in) : in_(in) {
    Next();
}

bool Tokenizer::IsEnd() {
    // returns true if cursor has already read all tokens
    if (!OnEOF()) {
        already_read_ = false;
    }
    return already_read_;
}

#include <iostream>

void Tokenizer::Next() {
    ClearSpace();
    if (OnEOF()) {
        already_read_ = true;
        return;
    }
    int curr_in_value = in_->peek();
    if (curr_in_value == 40 || curr_in_value == 41 || curr_in_value == 42 || curr_in_value == 44 ||
            curr_in_value == 60 || curr_in_value == 61 || curr_in_value == 62) {
        in_->get();
        curr_token_ = SymbolToken(curr_in_value);
    } else if (curr_in_value == 43) {  // `+`
        in_->get();
        if (std::isdigit(in_->peek())) {
            curr_token_ = ConstantToken(ReadNumber());
        } else {
            curr_token_ = SymbolToken("+");
        }
    } else if (curr_in_value == 45) {  // `-`
        in_->get();
        if (std::isdigit(in_->peek())) {
            curr_token_ = ConstantToken(-ReadNumber());
        } else {
            curr_token_ = SymbolToken("-");
        }
    } else if (curr_in_value == 47) {
        in_->get();
        curr_token_ = DivisionToken();  // `/`
    } else if (curr_in_value == 59) {
        in_->get();
        curr_token_ = SemicolonToken();  // `;`
    } else if (curr_in_value == 91) {
        in_->get();
        curr_token_ = BracketToken::OPEN;  // `[`
    } else if (curr_in_value == 93) {
        in_->get();
        curr_token_ = BracketToken::CLOSE;  // `]`
    } else {
        if (std::isdigit(curr_in_value)) {
            curr_token_ = ConstantToken(ReadNumber());
            if (std::isalpha(in_->peek())) {
                throw SyntaxError("invalid name of object\n");
            }
        } else {
            curr_token_ = SymbolToken(ReadSymbol());
        }
    }
}

Token Tokenizer::GetToken() {
    return curr_token_;
}

int Tokenizer::ReadNumber() {
    std::string read_value;
    while (!OnEOF() && std::isdigit(in_->peek())) {
        read_value += static_cast<char>(in_->get());
    }
    return std::stoi(read_value);
}

std::string Tokenizer::ReadSymbol() {
    std::string read_value;
    if (!IsValidStringBegin(in_->peek())) {
        throw SyntaxError{"invalid `symbol` declaration"};
    }
    while (!OnEOF() && !std::isspace(in_->peek()) && !IsSpecialSymbol(in_->peek())) {
        read_value += static_cast<char>(in_->get());
    }
    return read_value;
}

void Tokenizer::ClearSpace() {
    // clear all space symbols from current cursor position till first non-space symbol
    while (!OnEOF() && std::isspace(in_->peek())) {
        in_->get();
    }
}

bool Tokenizer::OnEOF() {
    // returns true if cursor is on eof
    return !std::char_traits<char>::not_eof(in_->peek());
}

bool Tokenizer::IsValidStringBegin(int char_code) {
    return 59 < char_code && char_code < 123 && char_code != 63 && \
            char_code != 64 && char_code != 96;
}

bool Tokenizer::IsSpecialSymbol(int char_code) {
    // returns true if char is ont of '*+,-./;[]>=<'
    return char_code == 42 || char_code == 43 || char_code == 45 || char_code == 46 || \
        char_code == 47 || char_code == 59 || char_code == 91 || char_code == 93 || \
        char_code == 60 || char_code == 61 || char_code == 62 || char_code == 40 || \
        char_code == 41 || char_code == 44;
}
