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

    function<void(int)> f_display_31337 = PrintNum();
    f_display_31337(42);

    auto smth = fun;
    smth(32);

    auto eee = f_display_42;
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