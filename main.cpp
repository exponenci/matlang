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
    {
        Interpreter interpreter;
        interpreter.Run("print([[1, 2, 3], [4, 5, 6]]);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("let a = 1/3; print(-a);");
    }
    {
        Interpreter interpreter;
        interpreter.Run("print(1 * (2 + 3)); "            // 5
                        "print( (((1 + 2) + 3) + 4) ); "  // 10
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
                        "print(A - B);"
                        "print(2 * A); "
                        "print(A * 2); "
                        "print((C + (transpose(A) * B) + C) * 2); "
                        "print(2 * C + transpose(A) * B - C * transpose(C)); "
                        );
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[5, -6, -7, 7],\n"
                        "[3, -2, 5, -17],\n"
                        "[2, 4, -3, 29]]; "
                        "print(rref(A));"
                        "print(to_diag(A));"
                        "print(to_triangle(A));"
                        "print(rank(A));"
                        "print(A);"
        );
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[5, -6, -7,   7],\n"
                        "         [3, -2,  5, -17],\n"
                        "         [2,  4, -3,  29],\n "
                        "         [1,  1, -1,   1]]; "
                        "print(rref(A));"
                        "print(to_diag(A));"
                        "print(to_triangle(A));"
                        "print(inv(A));"
                        "print(det(A));"
                        "print(rank(A));"
                        "print(A);"
                        );
        /*  1       0       0       0
            0       1       0       0
            0       0       1       0
            0       0       0       1
            5       0       0       0
            0       8/5     0       0
            0       0       -37     0
            0       0       0       -8
            5       -6      -7      7
            0       8/5     46/5    -106/5
            0       0       -37     111
            0       0       0       -8
            45/1184 169/1184        39/592  1/4
            -49/592 -13/592 -3/296  1/2
            -95/2368        301/2368        115/1184        -3/8
            11/2368 15/2368 49/1184 -1/8
            2368
            4
            5       -6      -7      7
            3       -2      5       -17
            2       4       -3      29
            1       1       -1      1
        */
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[5, -6, -7,   7],\n"
                        "         [3, -2,  5, -17],\n"
                        "         [2,  4, -3,  29],\n "
                        "         [2,  4, -3,  29],\n "
                        "         [1,  1, -1,   1]]; "
                        "print(rref(A));"
                        "print(to_diag(A));"
                        "print(to_triangle(A));"
                        "print(rank(A));"
                        "print(A);"
                        );
    }
    {
        Interpreter interpreter;
        interpreter.Run("let A = [[5, -6, -7,   7],\n"
                        "         [2,  4, -3,  29],\n"
                        "         [2,  4, -3,  29],\n "
                        "         [2,  4, -3,  29]];\n "
                        "print(rref(A));"
                        "print(to_diag(A));"
                        "print(to_triangle(A));"
                        "print(det(A));"
                        "print(rank(A));"
                        "print(A);"
        );
    }
    return 0;
}
