//
// Created by max on 1/19/19.
//

#pragma once

#include <iostream>
#include <memory>
#include <variant>

const size_t SMALL_OBJECT_CONST = 20;

template<class>
class function;

template<int>
class tag {
};

template<class R, class... Args>
class function<R(Args...)> {
    using f_ptr = R(*)(Args...);


public:
    function() = default;

    explicit function(std::nullptr_t) {
        func = nullptr;
    }


    function(const function &other) : func(std::move(other.func->copy())) {
    }

    function(function &&other) noexcept : func(std::move(other.func)) {
    }

    function &operator=(const function &other) {
        function temp(other);
        temp.swap(*this);
    }

    function &operator=(function &&other) noexcept {
        function temp(other);
        temp.swap(*this);
    }

    void swap(function &other) noexcept {
        std::swap(func, other.func);
    }

    explicit operator bool() const noexcept {
        return func;
    }

private:
    class base {
    public:
        base() = default;

        virtual R eval(Args... args) = 0;

        virtual std::unique_ptr<base> copy() = 0;

        virtual ~base() {};
    };

    template<typename Fu>
    class template_base_small : public base {
    public:
        explicit template_base_small(Fu f) : base(), value(f) {
        }

        R eval(Args...args) override {
            return value(std::forward<Args>(args)...);
        }

        std::unique_ptr<base> copy() override {
            return std::make_unique<template_base_small<Fu>>(value);
        }

        ~template_base_small() = default;

    private:
        Fu value;
    };

    template<typename Fu>
    class template_base_big : public base {
    public:
        explicit template_base_big(Fu f) : base(), value(&f) {
        }

        R eval(Args...args) override {
            return (*value)(std::forward<Args>(args)...);
        }

        std::unique_ptr<base> copy() override {
            return std::make_unique<template_base_big<Fu>>(*value);
        }

        ~template_base_big() = default;

    private:
        std::unique_ptr<Fu> value;
    };

public:
    template<class F>
    function(F f) {
        if (sizeof(f) <= SMALL_OBJECT_CONST) {
            func = std::make_unique<template_base_small<F>>(std::forward<F>(f));
        } else {
            func = std::make_unique<template_base_big<F>>(std::forward<F>(f));
        }
    }

    R operator()(Args &&... args) {
        return func->eval(std::forward<Args>(args)...);
    }

    ~function() = default;

private:
    std::unique_ptr<base> func;
};
