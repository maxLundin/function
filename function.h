//
// Created by max on 1/19/19.
//

#pragma once

#include <iostream>
#include <memory>
#include <variant>
#include <cstring>

const size_t SMALL_OBJECT_CONST = 20;

template<class>
class function;

template<int>
class tag {
};

template<class R, class... Args>
class function<R(Args...)> {

public:
    function() : func(nullptr), is_small(false) {
    }

    explicit function(std::nullptr_t) : func(nullptr), is_small(false) {
    }


    function(const function &other) : func(nullptr), is_small(other.is_small) {
        if (is_small) {
            memcpy(buff, other.buff, SMALL_OBJECT_CONST);
        } else {
            func = (std::move(other.func->copy()));
        }
    }

    function(function &&other) noexcept : func(nullptr) {
        std::swap(buff, other.buff);
        std::swap(is_small, other.is_small);
        other.is_small = false;
        other.func.reset(nullptr);
    }

    function &operator=(const function &other) {
        function temp(other);
        is_small = other.is_small;
        temp.swap(*this);
        return *this;
    }

    function &operator=(function &&other) noexcept {
        auto temp(std::forward<function>(other));
        swap(temp);
        return *this;
    }

    void swap(function &other) noexcept {
        std::swap(buff, other.buff);
        std::swap(is_small, other.is_small);
    }

    explicit operator bool() const noexcept {
        if (!is_small) {
            return func.operator bool();
        }
        return true;
    }

    template<class F>
    function(F f) {
        if constexpr (sizeof(base_template_impl<F>(f)) <= SMALL_OBJECT_CONST) {
            new(buff) base_template_impl<F>(std::move(f));
            is_small = true;
        } else {
            new(buff) std::unique_ptr<base_template_impl < F>>
            (new base_template_impl<F>(std::move(f)));
            is_small = false;
        }
    }

    R operator()(Args &&... args) {
        if (!is_small) {
            return func->eval(std::forward<Args>(args)...);
        } else {
            return ((base *) buff)->eval(std::forward<Args>(args)...);
        }
    }

    ~function() {
        if (is_small) {
            ((base *) buff)->~base();
        } else {
            func.reset();
        }
    }

private:
    class base {
    public:
        base() = default;

        virtual R eval(Args &&... args) = 0;

        virtual std::unique_ptr<base> copy() = 0;

        virtual ~base() {};
    };

    template<typename Fu>
    class base_template_impl : public base {
    public:
        explicit base_template_impl(Fu f) : base(), value(std::move(f)) {
        }

        R eval(Args &&... args) override {
            return value(std::forward<Args>(args)...);
        }

        std::unique_ptr<base> copy() override {
            return std::make_unique<base_template_impl<Fu>>(value);
        }

        ~base_template_impl() = default;

    private:
        Fu value;
    };


private:
    union {
        std::unique_ptr<base> func;
        char buff[std::max(SMALL_OBJECT_CONST, sizeof(std::unique_ptr<base>))];
    };
    bool is_small;
};
