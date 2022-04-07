#include "expression.h"

void Expression::AddArg(sptrObj &&arg) {
    args_.push_back(std::move(arg));
}

size_t Expression::GetSize() const {
    return args_.size();
}

std::list<sptrObj> &Expression::GetArgs() {
    return args_;
}

std::string Expression::GetString() {
    return "<expression object>";
}
