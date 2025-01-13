#pragma once
#ifndef __BIG_INT__
#define __BIG_INT__

#include "../DataStructures/Array.h"
using _BlockType = unsigned long long;
using _SizeType = unsigned long long;

namespace alpha {
    class UnsignedInt : public Array<unsigned long long> {
    public:
        UnsignedInt(const char* _Hex);
    public:
        bool operator> (const UnsignedInt& _That)const;
        bool operator< (const UnsignedInt& _That)const;
        bool operator>=(const UnsignedInt& _That)const;
        bool operator<=(const UnsignedInt& _That)const;


        UnsignedInt& operator<<=(const unsigned long long _Shift);
        UnsignedInt& operator>>=(const unsigned long long _Shift);
        UnsignedInt& operator|=(const UnsignedInt& _That);
        UnsignedInt& operator&=(const UnsignedInt& _That);
        UnsignedInt& operator^=(const UnsignedInt& _That);
        UnsignedInt& operator+=(const UnsignedInt& _That);
        UnsignedInt& operator-=(const UnsignedInt& _That);
        UnsignedInt& operator*=(const UnsignedInt& _That);
        // constexpr UnsignedInt& operator/=(const UnsignedInt& _That)noexcept;
        // constexpr UnsignedInt& operator%=(const UnsignedInt& _That)noexcept;

    public:
        void print(FILE* file)const;
    };

    void _print(const UnsignedInt& x);
}


#endif