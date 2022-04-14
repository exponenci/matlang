#include <iostream>
#include <sstream>

#include "include/tokenizer.h"
#include "include/parser.h"


void REQUIRE(bool cond, std::string_view sv = "") {
    if (!cond) {
        std::cerr << "Error:\t" << sv << "\n";
    }
}

auto ReadFull(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};

    auto obj = Read(&tokenizer);
    REQUIRE(tokenizer.IsEnd());
    return obj;
}

int main() {
    {
        auto node = ReadFull("mat A = [[]];");
        node = ReadFull("mat A = inv(B);");
        node = ReadFull("print(A + B * C);");
    }
    {
        auto node = ReadFull("mat A = inv(B, C, D, E, F);");
        REQUIRE(Is<CommandObject>(node));
        REQUIRE(As<Symbol>(As<CommandObject>(node)->GetCommand())->GetString() == "init");
        auto data = As<CommandObject>(node)->GetArgs();
        REQUIRE(As<Symbol>(data.front())->GetString() == "A");
        REQUIRE(Is<CommandObject>(data.back()));
        REQUIRE(As<Symbol>(As<CommandObject>(data.back())->GetCommand())->GetString() == "inv");
        auto args = As<CommandObject>(data.back())->GetArgs();
        std::vector<std::string> to_cmp = {"B", "C", "D", "E", "F"};
        for (size_t i = 0; i < args.size(); ++i) {
            REQUIRE(As<Symbol>(args.front())->GetString() == to_cmp[i]);
            args.pop_front();
        }
    }
    {
        auto node = ReadFull("mat A = print(B + C * D, D, [[]]);");
        REQUIRE(Is<CommandObject>(node));
        REQUIRE(As<Symbol>(As<CommandObject>(node)->GetCommand())->GetString() == "init");
        auto data = As<CommandObject>(node)->GetArgs();
        REQUIRE(As<Symbol>(data.front())->GetString() == "A");
        REQUIRE(Is<CommandObject>(data.back()));
        REQUIRE(As<Symbol>(As<CommandObject>(data.back())->GetCommand())->GetString() == "print");
        auto args = As<CommandObject>(data.back())->GetArgs();
        REQUIRE(Is<Expression>(args.front()));
        auto other_data = As<Expression>(args.front())->GetArgs();
        std::vector<std::string> to_cmp = {"B", "+", "C", "*", "D"};
        for (size_t i = 0; i < other_data.size(); ++i) {
            REQUIRE(As<Symbol>(other_data.front())->GetString() == to_cmp[i]);
            other_data.pop_front();
        }
        args.pop_front();
        REQUIRE(As<Symbol>(args.front())->GetString() == "D");
        args.pop_front();
        REQUIRE(Is<Matrix<std::shared_ptr<Object>>>(args.front()));
    }
    {
        auto node = ReadFull("print(A);");
        REQUIRE(Is<CommandObject>(node));
        REQUIRE(As<Symbol>(As<CommandObject>(node)->GetCommand())->GetString() == "print");
        REQUIRE(As<Symbol>(As<CommandObject>(node)->GetArgs().front())->GetString() == "A");
    }
    {
        auto node = ReadFull("print(inv(A), GCD);");
        REQUIRE(Is<CommandObject>(node));
        REQUIRE(As<Symbol>(As<CommandObject>(node)->GetCommand())->GetString() == "print");
        REQUIRE(Is<CommandObject>(As<CommandObject>(node)->GetArgs().front()));
        auto args = As<CommandObject>(node)->GetArgs();
        REQUIRE(As<Symbol>(As<CommandObject>(args.front())->GetCommand())->GetString() == "inv");
        REQUIRE(As<Symbol>(As<CommandObject>(args.front())->GetArgs().front())->GetString() == "A");
        REQUIRE(As<Symbol>(args.back())->GetString() == "GCD");
    }
    {
        auto node = ReadFull("print(inv(kek(bra(foo(A)))), det(B));");
        REQUIRE(Is<CommandObject>(node));
        auto args = As<CommandObject>(node)->GetArgs();
        size_t n = 4;
        std::vector<std::string> to_cmp = {"inv", "kek", "bra", "foo"};
        while (n--) {
            args = As<CommandObject>(node)->GetArgs();
            node = args.front();
            REQUIRE(Is<CommandObject>(node));
            REQUIRE(As<Symbol>(As<CommandObject>(node)->GetCommand())->GetString() == to_cmp[3 - n]);
        }
    }
    {
        auto node = ReadFull("mat A = [[100, 200, 300], [2, 3, 4]];");
        node = ReadFull("print(A * [[1,2,3], [4,5,6]] + [[1,2,4]]);");
    }
    {
        auto node = ReadFull("mat A = [[1, -2, 5], [2, -32, 4], [-1, 0, -198]];");
        REQUIRE(Is<CommandObject>(node));
        REQUIRE(Is<Matrix<std::shared_ptr<Object>>>(As<CommandObject>(node)->GetArgs().back()));
        std::vector<std::vector<int>> data = {{1, -2, 5}, {2, -32, 4}, {-1, 0, -198}};
        auto args = As<Matrix<std::shared_ptr<Object>>>(As<CommandObject>(node)->GetArgs().back())->GetArray();
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                REQUIRE(Is<Integer>(args[i][j]));
                REQUIRE(As<Integer>(args[i][j])->Get() == data[i][j]);
            }
        }
    }
    {
        auto node = ReadFull("mat A = "
                             "[ [1 + d, -det(A), 5], "
                             "  [foo(bar(tmp1, tmp2)) + A * 2, -32, 4 + tmp], "
                             "  [-1, a * b + c * d, -198]"
                             "];");
        REQUIRE(Is<CommandObject>(node));
        // IT IS DIFFICULT TO CHECK THE RIGHT PARSING, SO JUST CHECK THIS IN GDB
        node = ReadFull("print(A * [[1, 2, det(B)], [423, 23 + 43 * 43, 13]]);");
        REQUIRE(Is<CommandObject>(node));
    }
    {
        auto node = ReadFull("print(2 + 3 * (4 - 3));");
        REQUIRE(Is<CommandObject>(node));
    }
    {
        auto node = ReadFull("print([[2, 2], [3, 2]] * ([[2, 2], [3, 2]] * [[2, 2], [3, 2]]));");
        REQUIRE(Is<CommandObject>(node));
    }
    return 0;
}
