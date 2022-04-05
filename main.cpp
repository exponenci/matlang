#include <iostream>
#include <sstream>

#include "tokenizer.h"
#include "parser.h"


void REQUIRE(bool cond, std::string_view sv = "") {
    if (!cond) {
        std::cerr << "Error:\t" << sv << "\n";
    }
}

int main() {
    return 0;
}
