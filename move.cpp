//
// Created by max on 1/19/19.
//

#include <memory>
#include <iostream>

//template <class T>
//std::shared_ptr<T>
//factory()   // версия без аргументов
//{
//    return std::shared_ptr<T>(new T);
//}
//
//template <class T, class A1>
//std::shared_ptr<T>
//factory(const A1& a1)   // версия с одним аргументом
//{
//    std::cout << a1 << std::endl;
//    return std::shared_ptr<T>(new T(a1));
//}


template<typename T>
struct remove_reference {
    using type = T;
};

template<typename T>
struct remove_reference<T &> {
    using type = T;
};

template<typename T>
struct remove_reference<T &&> {
    using type = T;
};

template<typename T>
constexpr typename remove_reference<T>::type &&my_move(T &&a) {
    return static_cast<typename remove_reference<T>::type &&>(a);
}

template<typename T>
constexpr T &&my_forward(typename remove_reference<T>::type &a) {
    return static_cast<T &&>(a);
}


template<typename T>
void g(T a) {

}

template<typename T>
void f(T &&a) {
    std::cout << typeid(a).name() << std::endl;
    g(std::forward<T>(a));
}

int main() {
//    std::shared_ptr<double> p = factory<double>(5);
    f(5);
    int a = 5;
    f(a);

    f(std::move(a));

//    std::cout << typeid(std::move(a)).name() << std::endl;
}