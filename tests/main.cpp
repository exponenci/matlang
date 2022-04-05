#include <iostream>
#include <memory>

class Base {
private:
    int data_;
public:
    Base(int value) : data_(value) {}
    ~Base() {}
};


int main() {
    Base data{4};
    std::shared_ptr<Base> ptr1{&data}, ptr2{&data};
    return 0;
}