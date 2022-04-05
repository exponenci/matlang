#pragma once

#ifndef MATLANG_TEST_FW_H
#define MATLANG_TEST_FW_H

#include <stdexcept>
#include <string>
#include <map>
#include <memory>
#include <vector>

class AbstractTest {
public:
    virtual void SetUp() = 0;

    virtual void TearDown() = 0;

    virtual void Run() = 0;

    virtual ~AbstractTest() {
    }
};

namespace functors {
    enum Functors {
        Substr, FullMatch
    };
}
class Functor {
private:
    functors::Functors type_;
    std::string pattern_;

public:
    explicit Functor(functors::Functors type, const std::string &pattern)
            : type_(type), pattern_(pattern) {
    }

    std::string_view GetPattern() const {
        return pattern_;
    }

    virtual bool operator()(std::string_view sv) const = 0;
};

class FullMatch : private Functor {
public:
    explicit FullMatch(const std::string &pattern)
            : Functor(functors::Functors::FullMatch, pattern) {
    }

    bool operator()(std::string_view sv) const override {
        return sv == Functor::GetPattern();
    }
};

class Substr : public Functor {
public:
    explicit Substr(const std::string &pattern) : Functor(functors::Functors::Substr, pattern) {
    }

    bool operator()(std::string_view sv) const override {
        return sv.find(Functor::GetPattern()) != std::string::npos;
    }
};

class AbstractTestCreator {
public:
    virtual std::unique_ptr<AbstractTest> Create() const = 0;

    ~AbstractTestCreator() = default;
};

template<typename T>
class TestCreator : public AbstractTestCreator {
public:
    std::unique_ptr<AbstractTest> Create() const override {
        return std::make_unique<T>();
    }
};

class TestRegistry;

class TestRegistryDeleter {
private:
    TestRegistry *instance_;

public:
    ~TestRegistryDeleter() noexcept;

    void Init(TestRegistry *ptr);
};

class TestRegistry {
private:
    static TestRegistry *instance;
    static TestRegistryDeleter deleter;

protected:
    TestRegistry() {};

    TestRegistry(const TestRegistry &) {};

    TestRegistry(TestRegistry &&) {};

    TestRegistry &operator=(const TestRegistry &) {
        return *this;
    };

    TestRegistry &operator=(TestRegistry &&) {
        return *this;
    };

    friend class TestRegistryDeleter;

public:
    static TestRegistry &Instance();

private:
    std::map<std::string, std::unique_ptr<AbstractTestCreator>> tests_registry_;

public:
    template<class TestClass>
    void RegisterClass(const std::string &class_name) {
        tests_registry_[class_name] = std::make_unique<TestCreator<TestClass>>();
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string &class_name) {
        if (!tests_registry_.contains(class_name)) {
            throw std::out_of_range("No test with such name");
        }
        return tests_registry_[class_name]->Create();
    }

    void RunTest(const std::string &test_name) {
        auto test_instance = tests_registry_[test_name]->Create();
        test_instance->SetUp();
        try {
            test_instance->Run();
        } catch (...) {
            test_instance->TearDown();
            throw;
        }
        test_instance->TearDown();
    }

    template<class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> all_class_names;
        all_class_names.reserve(tests_registry_.size());
        for (auto&[key, value]: tests_registry_) {
            if (callback(key)) {
                all_class_names.push_back(key);
            }
        }
        return all_class_names;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> all_class_names;
        all_class_names.reserve(tests_registry_.size());
        for (auto&[key, value]: tests_registry_) {
            all_class_names.push_back(key);
        }
        return all_class_names;
    }

    template<class Predicate>
    void RunTests(Predicate callback) {
        for (auto&[key, value]: tests_registry_) {
            if (callback(key)) {
                RunTest(key);
            }
        }
    }

    void Clear() {
        tests_registry_.clear();
    }
};

TestRegistry *TestRegistry::instance = nullptr;
TestRegistryDeleter TestRegistry::deleter;

TestRegistryDeleter::~TestRegistryDeleter() noexcept {
    delete instance_;
}

void TestRegistryDeleter::Init(TestRegistry *ptr) {
    instance_ = ptr;
}

TestRegistry &TestRegistry::Instance() {
    if (!instance) {
        instance = new TestRegistry();
        deleter.Init(instance);
    }
    return *instance;
}


#endif //MATLANG_TEST_FW_H
