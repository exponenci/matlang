#include "tokenizer.h"
#include "error.h"

SymbolToken::SymbolToken(int char_code)
        : name_(1, static_cast<char>(char_code)) {
}

SymbolToken::SymbolToken(const std::string &s)
        : name_(s) {
}

ConstantToken::ConstantToken(int value)
        : value_(value) {
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

// special tokens for lang: (){},.^+-*/<=>  ;[]
void Tokenizer::Next() {
    ClearSpace();
    if (OnEOF()) {
        already_read_ = true;
        return;
    }
    int curr_in_value = in_->peek();
    if (IsProhibitedSymbol(curr_in_value)) {
        throw SyntaxError("Tokenizer::Next: prohibited symbol was used in script\n");
    } else if (IsSpecialSymbol(curr_in_value)) {
        in_->get();
        if (curr_in_value == 43 && std::isdigit(in_->peek())) { // +178
            curr_token_ = ConstantToken(ReadNumber());
        } else if (curr_in_value == 45 && std::isdigit(in_->peek())) { // -42
            curr_token_ = ConstantToken(-ReadNumber());
        } else if (curr_in_value == 59) { // ;
            curr_token_ = SemicolonToken();
        } else if (curr_in_value == 91) { // [
            curr_token_ = BracketToken::OPEN;
        } else if (curr_in_value == 93) { // ]
            curr_token_ = BracketToken::CLOSE;
        } else {                          // !&()*,./<=>^{|}~+-
            curr_token_ = SymbolToken(curr_in_value);
        }
    } else {
        if (std::isdigit(curr_in_value)) {
            curr_token_ = ConstantToken(ReadNumber());
            if (std::isalpha(in_->peek())) { // 213x
                throw SyntaxError("Tokenizer::Next: invalid variable name\n");
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
    if (!std::isalpha(in_->peek()) && in_->peek() != 95) { // valid string beginning is only _A-Za-z
        throw SyntaxError{"invalid `symbol` declaration"};
    }
    read_value += static_cast<char>(in_->get());
    // valid string names consist of only _A-Za-z0-9
    while (!OnEOF() && (std::isalnum(in_->peek()) || in_->peek() == 95)) {
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

constexpr bool Tokenizer::IsProhibitedSymbol(int char_code) {
    // prohibited symbols:   $#%'"?@\ //
    constexpr std::string_view prohibited_symbols = "$#%'\"?@\\";
    return prohibited_symbols.find(static_cast<char>(char_code)) != std::string_view::npos;
}

constexpr bool Tokenizer::IsSpecialSymbol(int char_code) {
    // returns true if char is ont of '!&()*+,-./:;<=>[]^{|}~'
    // !&| - used for not, and, or respectively
    // ()[]{} - brackets for functions, vectors/matrices, code blocks
    // *+-/^ - arithmetic operations
    // :,.~ - dunno why, perhaps will be useful one day
    // <=> for comparison (in future it cat be ok to handle <= and >=)
    // ; - command line end
    constexpr std::string_view special_symbols = "!&()*+,-./:;<=>[]^{|}~";
    return special_symbols.find(static_cast<char>(char_code)) != std::string_view::npos;
//    return char_code == 33 || char_code == 38 ||
//           (39 < char_code && char_code < 48) || (57 < char_code && char_code < 63) ||
//           (90 < char_code && char_code < 95 && char_code != 92) || (122 < char_code && char_code < 127);
}
