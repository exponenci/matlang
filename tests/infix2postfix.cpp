#include <iostream>
#include <stack>
#include <list>
#include <cctype>

int Priority(char alpha) {
    if (alpha == '+' || alpha =='-') {
        return 1;
    }
    if (alpha == '*' || alpha =='/') {
        return 2;
    }
    if (alpha == '^') {
        return 3;
    }
    return 0;
}

bool is_number(std::string_view sv) {
    for (auto& c : sv) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::list<std::string> Convert(std::list<std::string> infix) {
    std::list<std::string> postfix;
    std::stack<std::string> s;
    for (auto & i : infix) {
        if (is_number(i)) {
            postfix.push_back(i);
        } else if (i == "(") { // if opening bracket then push the stack
            s.push(i);
        } else if (i == ")") {            // if closing bracket encounted then keep popping from stack until
            while (s.top() != "(") {
                postfix.push_back(s.top());
                s.pop();
            }
            s.pop();
        } else {
            while (!s.empty() && Priority(i[0]) <= Priority(s.top()[0])) {
                postfix.push_back(s.top());
                s.pop();
            }
            s.push(i);
        }
    }
    while (!s.empty()) {
        postfix.push_back(s.top());
        s.pop();
    }
    return postfix;
}


int EvaluatePostfix(std::list<std::string> exp) {
    // Create a stack of capacity equal to expression size
    std::stack<int> stack;
    // Scan all characters one by one
    for (auto & i : exp) {
        if (is_number(i)) {
            // If the scanned character is an operand (number here),
            // push it to the stack.
            stack.push(std::stoi(i));
        } else {
            // If the scanned character is an operator, pop two
            // elements from stack apply the operator
            int val1 = stack.top();
            stack.pop();
            int val2 = stack.top();
            stack.pop();
            switch (i[0]) {
                case '+': stack.push(val2 + val1); break;
                case '-': stack.push(val2 - val1); break;
                case '*': stack.push(val2 * val1); break;
                case '/': stack.push(val2/val1); break;
            }
        }
    }
    return stack.top();
}


int main() {
    std::list<std::string> data = {"0"}; // some string to calculate in infix format
    std::list<std::string> postfix = Convert(data);
    std::cout << EvaluatePostfix(postfix) << "\n";
    return 0;
}
