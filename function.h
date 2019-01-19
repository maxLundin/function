//
// Created by max on 1/19/19.
//

#pragma once

#include <iostream>
#include <memory>
#include <variant>

const size_t SMALL_OBJECT_CONST = 1;

template<class>
class function;

template<int>
class tag {
};

template<class R, class... Args>
class function<R(Args...)> {

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
        return *this;
    }

    function &operator=(function &&other) noexcept {
        swap(other);
        return *this;
    }

    void swap(function &other) noexcept {
        std::swap(func, other.func);
    }

    explicit operator bool() const noexcept {
        return func;
    }

    template<class F>
    function(F f) {
        if constexpr (sizeof(f) <= SMALL_OBJECT_CONST) {
            func = std::make_unique<template_base_small < F>>
            (std::move(f));
        } else {
            func = std::make_unique<template_base_big < F>>
            (std::move(f));
        }
    }

    R operator()(Args &&... args) {
        return func->eval(std::forward<Args>(args)...);
    }

    ~function() = default;

private:
    class base {
    public:
        base() = default;

        virtual R eval(Args &&... args) = 0;

        virtual std::unique_ptr<base> copy() = 0;

        virtual ~base() {};
    };

    template<typename Fu>
    class template_base_small : public base {
    public:
        explicit template_base_small(Fu f) : base(), value(std::move(f)) {
        }

        R eval(Args &&... args) override {
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
        explicit template_base_big(Fu f) : base(), value(new Fu(std::move(f))) {
        }

        R eval(Args &&... args) override {
            return (*value)(std::forward<Args>(args)...);
        }

        std::unique_ptr<base> copy() override {
            return std::make_unique<template_base_big<Fu>>(*value);
        }

        ~template_base_big() = default;

    private:
        std::unique_ptr<Fu> value;
    };

private:
    std::unique_ptr<base> func;
};
