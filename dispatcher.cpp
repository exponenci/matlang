#include "dispatcher.h"

#include <utility>


Dispatcher::Dispatcher() {
    registers_ = {
            {"+",         std::make_shared<ArithmeticCommand>(
                    this, [](int64_t lhs, int64_t rhs) -> int64_t { return lhs + rhs; }, 0)},
            {"-",         std::make_shared<ArithmeticCommand>(
                    this, [](int64_t lhs, int64_t rhs) -> int64_t { return lhs - rhs; }, -1)},
            {"*",         std::make_shared<ArithmeticCommand>(
                    this, [](int64_t lhs, int64_t rhs) -> int64_t { return lhs * rhs; }, 1)},
            {"/",         std::make_shared<ArithmeticCommand>(
                    this, [](int64_t lhs, int64_t rhs) -> int64_t { return lhs / rhs; }, -1)},
            {"max",
                          std::make_shared<ArithmeticCommand>(
                                  this, [](int64_t lhs, int64_t rhs) -> int64_t { return std::max(lhs, rhs); }, -1)},
            {"min",
                          std::make_shared<ArithmeticCommand>(
                                  this, [](int64_t lhs, int64_t rhs) -> int64_t { return std::min(lhs, rhs); }, -1)},
            {"abs",       std::make_shared<AbsCommand>(this)},
            {"boolean?",  std::make_shared<IsBoolean>(this)},
            {"and",       std::make_shared<AndCommand>(this)},
            {"or",        std::make_shared<OrCommand>(this)},
            {"not",       std::make_shared<NotCommand>(this)},
            {"null?",     std::make_shared<IsNull>(this)},
            {"pair?",     std::make_shared<IsPair>(this)},
            {"list?",     std::make_shared<IsList>(this)},
            {"list",      std::make_shared<MakeList>(this)},
            {"cons",      std::make_shared<MakePair>(this)},
            {"car",       std::make_shared<GetElemInPair>(0, this)},
            {"cdr",       std::make_shared<GetElemInPair>(1, this)},
            {"list-ref",  std::make_shared<GetListElem>(this)},
            {"list-tail", std::make_shared<GetListTail>(this)},
            {"symbol?",   std::make_shared<IsSymbol>(this)},
            {"define",    std::make_shared<DefineCommand>(this)},
            {"set!",      std::make_shared<SetCommand>(this)},
            {"set-car!",  std::make_shared<SetPairElem>(0, this)},
            {"set-cdr!",  std::make_shared<SetPairElem>(1, this)},
            {"if",        std::make_shared<IfExpression>(this)},
            {"lambda",    std::make_shared<LambdaExpression>(this)},
    };
}

Dispatcher &Dispatcher::Instance() {
    if (!instance) {
        instance = new Dispatcher{};
        deleter.Init(instance);
    }
    return *instance;
}

std::shared_ptr<Command> Dispatcher::operator()(const std::string &cmd_name) {
    if (!registers_.contains(cmd_name)) {
        throw RuntimeError("no such command;");
    }
    return registers_[cmd_name];
}

//std::shared_ptr<Object> Dispatcher::RunCommandLine(std::shared_ptr<Object> curr_ptr) {
//    if (Is<Symbol>(curr_ptr)) {
//        if (IsVariable(curr_ptr)) {
//            return GetVariable(curr_ptr);
//        } else {
//            throw NameError("RunCommandLine: no such var");
//        }
//    }
//    if (!Is<CommandObject>(curr_ptr)) {
//        return curr_ptr;
//    }
//    std::shared_ptr<CommandObject> command_ptr = As<CommandObject>(curr_ptr);
//    if (instance->contexts_.empty()) {
//        instance->contexts_.push_back(&instance->variables_);
//    }
//    if (Is<Symbol>(command_ptr->GetCommand())) {
//        std::string command_name = As<Symbol>(command_ptr->GetCommand())->GetString();
//        if (registers_.contains(command_name)) {
//            return (*this)(command_name)->Run(command_ptr->GetArgs());
//        }
//        if ((*instance->contexts_.back()).contains(command_name) &&
//            Is<LambdaObject>((*instance->contexts_.back()).at(command_name))) {
//            std::shared_ptr<LambdaObject> lambda_object =
//                    As<LambdaObject>((*instance->contexts_.back()).at(command_name));
//            std::list<std::shared_ptr<Object>> data;
//            for (auto &arg: command_ptr->GetArgs()) {
//                if (Is<CommandObject>(arg)) {
//                    data.push_back(RunCommandLine(arg));
//                } else {
//                    data.push_back(arg);
//                }
//            }
//            instance->contexts_.push_back(
//                    lambda_object->GetContext(data, instance->contexts_.back()));  // set new context
//            std::shared_ptr<Object> result;
//            for (auto command: lambda_object->GetCommands()) {
//                result = RunCommandLine(command);
//            }
//            if (Is<LambdaObject>(result)) {
//                As<LambdaObject>(result)->SetDependency(
//                        instance->contexts_.back());  // set new context;
//            }
//            GarbageCollector();
//            instance->contexts_.pop_back();
//            GarbageCollector();
//            return result;  // func & arg
//        }
//    } else if (Is<LambdaObject>(command_ptr->GetCommand())) {
//        std::shared_ptr<LambdaObject> lambda_object = As<LambdaObject>(command_ptr->GetCommand());
//        instance->contexts_.push_back(lambda_object->GetContext(
//                command_ptr->GetArgs(), instance->contexts_.back()));  // set new context
//        std::shared_ptr<Object> result;
//        for (auto command: lambda_object->GetCommands()) {
//            result = RunCommandLine(command);
//        }
//        GarbageCollector();
//        instance->contexts_.pop_back();
//        GarbageCollector();
//
//        return result;  // func & arg
//    }
//    throw RuntimeError("Dispatcher::Run : no command or lambda was provided");
//}

std::shared_ptr<Object> Dispatcher::At(const std::string& varname) const {
    if (!instance->variables_.contains(varname)) {
        return nullptr;
    }
    return instance->variables_.at(varname);
}

void Dispatcher::Init(const std::string& varname, std::shared_ptr<Object> sptr) {
    instance->variables_[varname] = std::move(sptr);
}
//
//std::map<std::string, std::shared_ptr<Object>> *Dispatcher::GetCurrDependencies() {
//    return instance->contexts_.back();
//}


Dispatcher *Dispatcher::instance = nullptr;
Deleter Dispatcher::deleter;

void Deleter::Init(Dispatcher *held_dp) {
    held_dp_ = held_dp;
}

Deleter::~Deleter() noexcept {
    delete held_dp_;
    held_dp_ = nullptr;
}
