//
// Created by max on 1/19/19.
//

template<typename T>
constexpr T &&my_forward(T &&a) {
    return static_cast<T &&>(a);
}

template<typename T>
T &&my_move(T &a) {
    return static_cast<T&&>(a);
}