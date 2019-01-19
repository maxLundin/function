#include <iostream>
#include <functional>
#include "function.h"


struct Foo {
    Foo(int num) : num_(num) {}

    void print_add(int i) const { std::cout << num_ + i << '\n'; }

    int num_;
};

void print_num(int i) {
    std::cout << i << '\n';
}

struct PrintNum {
    PrintNum() = default;

    PrintNum(PrintNum const &a) {
        std::cout << "const &" << std::endl;
    }

    PrintNum &operator=(const PrintNum &other) {
        std::cout << "const operator = " << std::endl;
        return *this;
    }

    PrintNum &operator=(PrintNum &other) {
        std::cout << "operator = " << std::endl;
        return *this;
    }

    PrintNum(PrintNum &&other) {
        std::cout << "&& constructor" << std::endl;
    }

    ~PrintNum() {
        std::cout << "destr" << std::endl;
    }

    void operator()(int i) const {
        std::cout << i << " " << "one" << '\n';
    }
};

int main() {
    function<void(int)> fun(print_num);
    fun(10);

    function<void()> f_display_42([]() { print_num(42); });
    f_display_42();

    function<void()> f_display_3 = std::bind(print_num, 42);
    f_display_3();
    PrintNum m;
    function<void(int)> f_display_31337(std::move(m));
    std::cout << "1" << std::endl;
    f_display_31337(42);
    std::cout << "2" << std::endl;


    auto smth = fun;
    smth(32);

    function<void()> eee;
    eee = std::move(f_display_42);
    eee();

    auto eee1 = f_display_3;
    eee1();
    eee();

    std::cout << std::endl;
    fun(32);
    f_display_31337(32);
    fun.swap(f_display_31337);
    fun(32);
    f_display_31337(32);

}