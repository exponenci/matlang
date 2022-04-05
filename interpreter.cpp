#include "interpreter.h"

#include <algorithm>

std::string Interpreter::Run(const std::string& expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};

    std::string result;
    result.reserve(100);

    std::list<std::shared_ptr<Object>> parsed_script = ReadScript(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("no whole line has been read;");
    }
    for (const auto& line : parsed_script) {
        Simplify(line); // make some evaluations, remove `ExpressionObject`s
    }
    WriteResult(parsed_script, result);
    return result;
}
//
//void Interpreter::WriteResult(std::shared_ptr<Object> curr_ptr, std::string& result) {
//    if (!curr_ptr) {
//        return;
//    }
//    if (Is<CommandObject>(curr_ptr)) {
//        result = operation_holder_.RunCommandLine(curr_ptr)->GetString();
//    } else if (Is<Number>(curr_ptr)) {
//        result = As<Number>(curr_ptr)->GetString();
//    } else if (Is<Symbol>(curr_ptr)) {
//        if (operation_holder_.IsVariable(curr_ptr)) {
//            result = operation_holder_.GetVariable(curr_ptr)->GetString();
//        } else {
//            throw NameError("unknown variable requested");
//        }
//    }
//}
