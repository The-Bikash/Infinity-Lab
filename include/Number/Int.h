#pragma once
#ifndef __BIG_INT__
#define __BIG_INT__

#include "../DataStructures/Array.h"
using _Stype = size_t;
struct IntImpl;
namespace alpha {
    class Int {
    public:
        Int()noexcept;
        Int(const _Stype _Size)noexcept;
        Int(Int&& _That)noexcept;
        Int(const Int& _That)noexcept;

        ~Int()noexcept;

        constexpr Int& operator=(const _Stype _Size)noexcept;
        constexpr Int& operator=(Int&& _That)noexcept;
        constexpr Int& operator=(const Int& _That)noexcept;


        [[nodiscard]] constexpr bool operator==(const Int& _That)const noexcept;
        [[nodiscard]] constexpr bool operator!=(const Int& _That)const noexcept;
        [[nodiscard]] constexpr bool operator> (const Int& _That)const noexcept;
        [[nodiscard]] constexpr bool operator< (const Int& _That)const noexcept;
        [[nodiscard]] constexpr bool operator>=(const Int& _That)const noexcept;
        [[nodiscard]] constexpr bool operator<=(const Int& _That)const noexcept;


        constexpr Int& operator<<=(const _Stype _Shift)noexcept;
        constexpr Int& operator>>=(const _Stype _Shift)noexcept;

        constexpr Int& operator|=(const Int& _That)noexcept;
        constexpr Int& operator&=(const Int& _That)noexcept;
        constexpr Int& operator^=(const Int& _That)noexcept;
        constexpr Int& operator+=(const Int& _That)noexcept;
        constexpr Int& operator-=(const Int& _That)noexcept;
        constexpr Int& operator*=(const Int& _That)noexcept;
        constexpr Int& operator/=(const Int& _That)noexcept;
        constexpr Int& operator%=(const Int& _That)noexcept;


        [[nodiscard]] Int operator<<(const _Stype that)const noexcept;
        [[nodiscard]] Int operator>>(const _Stype that)const noexcept;
        [[nodiscard]] Int operator|(const Int& _That)const noexcept;
        [[nodiscard]] Int operator&(const Int& _That)const noexcept;
        [[nodiscard]] Int operator^(const Int& _That)const noexcept;
        [[nodiscard]] Int operator+(const Int& _That)const noexcept;
        [[nodiscard]] Int operator-(const Int& _That)const noexcept;

    private:
        IntImpl* _Impl;
    };


    struct IntImpl : Array<_Stype> {
        using Array<_Stype>::Array; // Take all constructors from base class
        using Array<_Stype>::operator=; // Take all assignment from base class



    };
}

#endif