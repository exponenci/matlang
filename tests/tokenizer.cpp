#include <iostream>
#include <sstream>

#include "include/tokenizer.h"


void REQUIRE(bool cond, std::string_view sv = "") {
    if (!cond) {
        std::cerr << "Error:\t" << sv << "\n";
    }
}

void CHECK_NOT_EMPTY(Tokenizer *ptr, const Token& expecting) {
    ptr->Next();
    REQUIRE(!ptr->IsEnd());
    REQUIRE(ptr->GetToken() == expecting);
}



int main() {
    std::cerr << "start testing...\n";
    {
        std::stringstream ss{"4+]2/3;aboba;;"};
        Tokenizer tokenizer{&ss};

        REQUIRE(!tokenizer.IsEnd());
        REQUIRE(tokenizer.GetToken() == Token{ConstantToken{4}});

        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"+"}});

        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::CLOSE});
        REQUIRE(tokenizer.GetToken() == Token{BracketToken::CLOSE});

        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{2}});

        CHECK_NOT_EMPTY(&tokenizer, Token{DivisionToken{}});

        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{3}});

        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});
        REQUIRE(tokenizer.GetToken() == Token{SemicolonToken{}});

        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"aboba"}});
        REQUIRE(tokenizer.GetToken() == Token{SymbolToken{"aboba"}});

        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});

        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});

        tokenizer.Next();
        REQUIRE(tokenizer.IsEnd());

        std::cerr << "test#1 is over\n";
    }
    {
        std::stringstream ss{"A_lambda    *bruh=\t\n;\t[[2/3, 5/3, d],[1,4,3]];inv(A_lambda);det(bruh);\n\n\n"};
        Tokenizer tokenizer{&ss};
        REQUIRE(!tokenizer.IsEnd());
        REQUIRE(tokenizer.GetToken() == Token{SymbolToken{"A_lambda"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"*"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"bruh"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"="}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::OPEN});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::OPEN});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{2}});
        CHECK_NOT_EMPTY(&tokenizer, Token{DivisionToken{}});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{3}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{","}});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{5}});
        CHECK_NOT_EMPTY(&tokenizer, Token{DivisionToken{}});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{3}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{","}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"d"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::CLOSE});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{","}});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::OPEN});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{1}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{","}});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{4}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{","}});
        CHECK_NOT_EMPTY(&tokenizer, Token{ConstantToken{3}});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::CLOSE});
        CHECK_NOT_EMPTY(&tokenizer, Token{BracketToken::CLOSE});
        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"inv"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"("}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"A_lambda"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{")"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"det"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"("}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{"bruh"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SymbolToken{")"}});
        CHECK_NOT_EMPTY(&tokenizer, Token{SemicolonToken{}});
        tokenizer.Next();
        REQUIRE(tokenizer.IsEnd());
        std::cerr << "test#2 is over\n";
    }
    {
        std::stringstream ss;
        ss << "2 ";
        Tokenizer tokenizer{&ss};
        REQUIRE(tokenizer.GetToken() == Token{ConstantToken{2}});
        ss << "* ";
        tokenizer.Next();
        REQUIRE(tokenizer.GetToken() == Token{SymbolToken{"*"}});
        ss << "2";
        tokenizer.Next();
        REQUIRE(tokenizer.GetToken() == Token{ConstantToken{2}});
        std::cerr << "test#3 is over\n";
    }
    {
        std::string input = R"EOF(
                                   )EOF";
        std::stringstream ss{input};
        Tokenizer tokenizer{&ss};
        REQUIRE(tokenizer.IsEnd());
        std::cerr << "test#4 is over\n";
    }
    {
        std::string input = R"EOF(
                            4 +
                            )EOF";
        std::stringstream ss{input};
        Tokenizer tokenizer{&ss};
        REQUIRE(!tokenizer.IsEnd());
        REQUIRE(tokenizer.GetToken() == Token{ConstantToken{4}});
        tokenizer.Next();
        REQUIRE(!tokenizer.IsEnd());
        REQUIRE(tokenizer.GetToken() == Token{SymbolToken{"+"}});
        tokenizer.Next();
        REQUIRE(tokenizer.IsEnd());
        std::cerr << "test#5 is over\n";
    }
    {
        std::stringstream ss;
        Tokenizer tokenizer{&ss};
        REQUIRE(tokenizer.IsEnd());
        std::cerr << "test#6 is over\n";
    }
    {
        try {
            std::string input = "1aboba";
            std::stringstream ss{input};
            Tokenizer tokenizer{&ss};
            tokenizer.Next();
            std::cerr << "test#7 failed\n";
        } catch (const std::runtime_error& err) {
            std::cerr << "test#7 is over\n";
        }
    }
    std::cerr << "all test are over!\n";
    return 0;
}
