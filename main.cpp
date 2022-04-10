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
//    {
//        Interpreter interpreter;
//        interpreter.Run("print([[1, 2, 3], [4, 5, 6]]]);");
//    }
    {
        Interpreter interpreter;
        interpreter.Run("print(1 * (2 + 3)); "            // 5
                        "print( (((1 + 2) + 3) + 4) ); "    // 10
                        "print((1 + (2 * 3) + 4) * 5);"   // 55
                        );

        // f((g(a) + b) * (c + h(d)))
        // f(a * (b + c))
        // f((((a + b) + c) + d));
        // f((a + (b * c) + d) * e);

    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[1, 2], [2, 1]]; print(A);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[1, 2], [2, 1], [1,3]]; "
                        "print(A); "
                        "let B = [[1, 0], [0, 1]]; "
                        "print(A); "
                        "print(B); "
                        "print(A, B);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[1, 2], [2, 1], [1, 3]]; "
                        "let B = [[7, -1], [5, 0], [4, -3]]; "
                        "let C = [[2, 3], [1, 2]];"
                        "print((C + (transpose(A) * B) + C) * 2); "
                        "print(2 * C + transpose(A) * B - C * transpose(C)); "
                        "print(2 * A); "
                        "print(A * 2); "
                        "print(A - B);"
                        );
    }
    return 0;
}
