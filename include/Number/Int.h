#pragma once
#ifndef __BIG_INT__
#define __BIG_INT__

#include "../DataStructures/Array.h"
using _BlockType = unsigned long long;
using _SizeType = unsigned long long;

namespace alpha {
    class UnsignedInt : public Array<_BlockType> {
    public:
        using Array<_BlockType>::Array;
        using Array<_BlockType>::operator=;
    public:
        UnsignedInt(const char* _Hex);
    private:
        void _Update(_SizeType _Siz)noexcept;
    public:
        [[nodiscard]] constexpr bool operator> (const UnsignedInt& _That)const noexcept;
        [[nodiscard]] constexpr bool operator< (const UnsignedInt& _That)const noexcept;
        [[nodiscard]] constexpr bool operator>=(const UnsignedInt& _That)const noexcept;
        [[nodiscard]] constexpr bool operator<=(const UnsignedInt& _That)const noexcept;


        constexpr UnsignedInt& operator<<=(const _SizeType _Shift)noexcept;
        constexpr UnsignedInt& operator>>=(const _SizeType _Shift)noexcept;

        constexpr UnsignedInt& operator|=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator&=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator^=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator+=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator-=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator*=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator/=(const UnsignedInt& _That)noexcept;
        constexpr UnsignedInt& operator%=(const UnsignedInt& _That)noexcept;


        [[nodiscard]] UnsignedInt operator<<(const _SizeType that)const noexcept;
        [[nodiscard]] UnsignedInt operator>>(const _SizeType that)const noexcept;
        [[nodiscard]] UnsignedInt operator|(const UnsignedInt& _That)const noexcept;
        [[nodiscard]] UnsignedInt operator&(const UnsignedInt& _That)const noexcept;
        [[nodiscard]] UnsignedInt operator^(const UnsignedInt& _That)const noexcept;
        [[nodiscard]] UnsignedInt operator+(const UnsignedInt& _That)const noexcept;
        [[nodiscard]] UnsignedInt operator-(const UnsignedInt& _That)const noexcept;
    public:
        void print(FILE* file)const;
    };

    void _print(const UnsignedInt& x);
}


#endif