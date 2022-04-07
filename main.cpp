#include <iostream>
#include <sstream>

#include "matrix.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

void REQUIRE(bool cond, std::string_view sv = "") {
    if (!cond) {
        std::cerr << "Error:\t" << sv << "\n";
    }
}

auto ReadFull(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};

    auto obj = ReadScript(&tokenizer);
    REQUIRE(tokenizer.IsEnd());
    return obj;
}

int main() {
//    {
//        auto node = ReadFull("mat A = [[]];");
//        node = ReadFull("mat A = inv(B);");
//        node = ReadFull("print(A + B * C);");
//        node = ReadFull("mat A = [[]]; "
//                        "mat A = inv(B);;;print(A + B * C);;");
//        std::cout << "hello\n";
//    }
    {
        Interpreter interpreter;
        interpreter.Run("mat A = [[1, 2], [2, 1]]; print(A);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("mat A = [[1, 2], [2, 1], [1,3]]; "
                        "print(A); "
                        "mat B = [[1, 0], [0, 1]]; "
                        "print(A); "
                        "print(B); "
                        "print(A, B);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("mat A = [[1, 2], [2, 1], [1, 3]]; "
                        "mat B = [[7, -1], [5, 0], [4, -3]]; "
                        "mat C = [[2, 3], [1, 2]];"
                        "print((C + (transpose(A) * B) + C) * 2); "
                        "print(2 * C + transpose(A) * B - C * transpose(C)); "
                        "print(2 * A); "
                        "print(A * 2); "
                        "print(A - B);"
                        );
    }
    return 0;
}
