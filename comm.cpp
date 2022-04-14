#include "comm.h"

ArithmeticCommand::ArithmeticCommand(std::function<sptrObj(sptrObj, sptrObj)> &&f)
        : BaseCommand(cmd::cmd_type::Arithmetic),
          func_(std::move(f)) {
}

sptrObj ArithmeticCommand::Run(std::list<sptrObj> &args) {
    if (args.size() != 2) {
        throw RuntimeError("ArithmeticCommand: invalid number of arguments for operation\n");
    }
    return func_(args.front(), args.back());
}

PrintCommand::PrintCommand()
        : BaseCommand(cmd::cmd_type::Print) {}

sptrObj PrintCommand::Run(std::list<sptrObj> &args) {
    for (auto &arg: args) {
        std::cout << arg->GetString() << "\n";
    }
    return std::make_shared<NoneObject>();
}

TransposeCommand::TransposeCommand()
        : BaseCommand(cmd::cmd_type::Transpose) {}

sptrObj TransposeCommand::Run(std::list<sptrObj> &args) {
    if (args.size() != 1) {
        throw RuntimeError("TransposeCommand: invalid number of arguments to transpose\n");
    }
    if (Is<Matrix>(args.front())) {
        return As<Matrix>(args.front())->Transposed();
    }
    throw RuntimeError("TransposeCommand: invalid value was provided to transpose\n");
}
