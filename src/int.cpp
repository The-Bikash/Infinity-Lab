#include "../include/Number/int.h"
#include<stdio.h>
namespace alpha {
    #define GREEN_FLAG 1
    using _MaxType = unsigned __int128;
    using _SignedType = long long;
    using _SizeType = unsigned long long;
    using _PointerType = _BlockType*;

    static const char _TotalHex = 16;
    static const _BlockType _Tmax = -1;
    static const _BlockType _mask = 0xf000000000000000;

namespace IO {
    _BlockType _Binary(const char _Hex){
        if(_Hex >= '0' && _Hex <= '9') return _Hex - '0';
        if(_Hex >= 'a' && _Hex <= 'f') return _Hex - 'a' + 10;
        if(_Hex >= 'A' && _Hex <= 'F') return _Hex - 'A' + 10;
        return 0;
    }

    const char* _Refine(const char* _Hex, _SizeType* _Len){
        if(*_Len >= 2){
            if(_Hex[0] == '0' && (_Hex[1] == 'x' || _Hex[1] == 'X')){
                *_Len -= 2;
                if(*_Len == 0){
                    perror("Invalid Number Only 0x");
                    exit(EXIT_FAILURE);
                }
                return _Hex + 2;
            }
        }
        return _Hex;
    }

    void _TransformHexToInt(const char* _Hex, _SizeType _Len, _BlockType* _Ptr){
        const char* _Rbegin = _Hex + _Len - 1; 
        const char* _Rend = _Hex - 1;
        _SizeType _Siz = _Len / _TotalHex;
        _SizeType i = 0;
        for(; i < _Siz; ++i){
            _Ptr[i] = 0;
            for(char j = 0; j < _TotalHex; ++j)
                _Ptr[i] |= _Binary(*(_Rbegin - j)) << (j * 4);
            _Rbegin -= _TotalHex;
        }
        char shift = 0;
        _Ptr[i] = 0;
        while(_Rbegin != _Rend){
            _Ptr[i] |= _Binary(*_Rbegin) << shift;
            --_Rbegin;
            shift += 4;
        }
    }
}

    UnsignedInt::UnsignedInt (const char* _Hex){
        _SizeType _Len = strlen(_Hex);
        _Hex = IO::_Refine(_Hex, &_Len);
        _Siz = _Len / _TotalHex + 1;
        _Allocate(_Siz);
        IO::_TransformHexToInt(_Hex, _Len, _Ptr);
    }

    void UnsignedInt::print(FILE* _FILE)const {
        static const unsigned char _hexcode[] = "0123456789abcdef";
        #define _BuffSiz  1023
        unsigned char _Buffer[_BuffSiz + 1];
        const unsigned char* _Data = (const unsigned char*)_Ptr;
        _SignedType i = _Siz * sizeof(_BlockType) - 1;
        while(_Data[i] == 0 && i >= 1) --i;
        _SignedType _idx = 0;
        _Buffer[_idx++] = '0'; _Buffer[_idx++] = 'x';
        const unsigned char _Ch = _hexcode[_Data[i] >> 4];
        if(_Ch != '0') _Buffer[_idx++] = _Ch;
        _Buffer[_idx++] = _hexcode[_Data[i] & 0x0f];
        --i;
        for(; i >= 0; --i){
            _Buffer[_idx++] = _hexcode[_Data[i] >> 4];
            _Buffer[_idx++] = _hexcode[_Data[i] & 0x0f];

            if(_idx >= _BuffSiz){
                fwrite(_Buffer, 1, _idx, _FILE);
                _idx = 0;
            }
        }
        fwrite(_Buffer, 1, _idx, _FILE);
    }
    void _print(const UnsignedInt& x) {
        x.print(stdout);
    }
    bool UnsignedInt::operator> (const UnsignedInt& _That)const {
        if (_Siz != _That._Siz)
            return _Siz > _That._Siz;

        for (_SizeType i = _Siz; i-- > 0;) {
            if (_Ptr[i] != _That._Ptr[i])
                return _Ptr[i] > _That._Ptr[i];
        }
        return false;
    }
    bool UnsignedInt::operator< (const UnsignedInt& _That)const {
        return _That > *this;
    }
    bool UnsignedInt::operator>=(const UnsignedInt& _That)const {
        return !(_That > *this);
    }
    bool UnsignedInt::operator<=(const UnsignedInt& _That)const {
        return !(*this > _That);
    }


    UnsignedInt& UnsignedInt::operator<<=(const unsigned long long _Shift) {
        constexpr auto _BlockBits = 8 * sizeof(_Ptr[0]);
        const auto _BlockShift = _Shift / _BlockBits;
        const auto _BitShift = _Shift % _BlockBits;
        const auto _CarryShift = _BlockBits - _BitShift;
        const bool _HasBitShift = _BitShift != 0;
        const auto _NewSize = _Siz + _BlockShift + (_HasBitShift ? 1 : 0);

        if (_NewSize > _Cap)
            _Reallocate(_NewSize);

        if (_BlockShift > 0) {
            for (_SizeType i = _Siz; i-- > 0;)
                _Ptr[i + _BlockShift] = _Ptr[i];
            for (_SizeType i = 0; i < _BlockShift; ++i)
                _Ptr[i] = 0;
            _Siz += _BlockShift;
        }

        if (_HasBitShift) {
            _SizeType _Carry = 0;
            for (_SizeType i = _BlockShift; i < _Siz; ++i) {
                const _SizeType _NextCarry = _Ptr[i] >> _CarryShift;
                _Ptr[i] = (_Ptr[i] << _BitShift) | _Carry;
                _Carry = _NextCarry;
            }
            if (_Carry != 0)
                _Ptr[_Siz++] = _Carry;
        }

        return *this;
    }
    UnsignedInt& UnsignedInt::operator>>=(const unsigned long long _Shift) {
        constexpr auto _BlockBits = 8 * sizeof(_Ptr[0]);
        const auto _BlockShift = _Shift / _BlockBits;
        const auto _BitShift = _Shift % _BlockBits;
        const auto _CarryShift = _BlockBits - _BitShift;
        const bool _HasBitShift = _BitShift != 0;


        if (_BlockShift > 0) {
            if (_BlockShift >= _Siz) {
                _Ptr[0] = 0;
                _Siz = 1;
                return *this;
            }
            _Siz -= _BlockShift;
            for (_SizeType i = 0; i < _Siz; ++i)
                _Ptr[i] = _Ptr[i + _BlockShift];
        }

        if (_HasBitShift) {
            _SizeType _Carry = 0;
            for (_SizeType i = _Siz; i-- > 0; ) {
                const _SizeType _NextCarry = _Ptr[i] << _CarryShift;
                _Ptr[i] = (_Ptr[i] >> _BitShift) | _Carry;
                _Carry = _NextCarry;
            }
            if (_Siz > 1 && _Ptr[_Siz - 1] == 0)
                --_Siz;
        }

        return *this;
    }
    
    UnsignedInt& UnsignedInt::operator|=(const UnsignedInt& _That) {
        if (_That._Siz > _Cap)
            _Reallocate(_That._Siz);
        if (_That._Siz > _Siz) {
            for(_SizeType i = _Siz; i < _That._Siz; ++i)
                _Ptr[i] = 0;
            _Siz = _That._Siz;
        }
        for (_SizeType i = 0; i < _That._Siz; ++i)
            _Ptr[i] |= _That._Ptr[i];
        return *this;
    }
    UnsignedInt& UnsignedInt::operator&=(const UnsignedInt& _That) {
        if (_That._Siz > _Cap)
            _Reallocate(_That._Siz);
        if (_That._Siz > _Siz) {
            for(_SizeType i = _Siz; i < _That._Siz; ++i)
                _Ptr[i] = 0;
            _Siz = _That._Siz;
        }
        for (_SizeType i = 0; i < _That._Siz; ++i)
            _Ptr[i] &= _That._Ptr[i];
        return *this;
    }
    UnsignedInt& UnsignedInt::operator^=(const UnsignedInt& _That) {
        if (_That._Siz > _Cap)
            _Reallocate(_That._Siz);
        if (_That._Siz > _Siz) {
            for(_SizeType i = _Siz; i < _That._Siz; ++i)
                _Ptr[i] = 0;
            _Siz = _That._Siz;
        }
        for (_SizeType i = 0; i < _That._Siz; ++i)
            _Ptr[i] ^= _That._Ptr[i];
        return *this;
    }
    UnsignedInt& UnsignedInt::operator+=(const UnsignedInt& _That) {
        if (_That._Siz > _Cap)
            _Reallocate(_That._Siz);
        for (; _Siz < _That._Siz; ++_Siz)
            _Ptr[_Siz] = 0;

        bool _Cary1 = false;
        bool _Cary2 = false;
        auto _Cache = 0ULL;
        auto i = 0ULL;

        for (; i < _That._Siz; ++i) {
            _Cache = _Ptr[i] + _That._Ptr[i];
            _Cary2 = _Cache == _Tmax && _Cary1 ? true : _Cache < _Ptr[i];
            _Ptr[i] = _Cache + _Cary1;
            _Cary1 = _Cary2;
        }

        for (; i < _Siz && _Cary1; ++i) {
            _Cary2 = _Ptr[i] == _Tmax;
            _Ptr[i] = _Ptr[i] + _Cary1;
            _Cary1 = _Cary2;
        }

        if (_Cary1) {
            if (_Siz == _Cap)
                _Reallocate(_Siz + 1);
            _Ptr[_Siz++] = 1;
        }
        return *this;
    }
    UnsignedInt& UnsignedInt::operator-=(const UnsignedInt& _That) {
        auto _Brrw1 = false;
        auto _Brrw2 = false;
        for (_SizeType i = 0; i < _That._Siz; ++i) {
            _Brrw2 = _Ptr[i] == _That._Ptr[i] && _Brrw1 ? true : _Ptr[i] < _That._Ptr[i];
            _Ptr[i] = _Ptr[i] - _That._Ptr[i] - _Brrw1;
            _Brrw1 = _Brrw2;
        }
        if (_Brrw1) {
            auto i = _That._Siz;
            for (; _Ptr[i] == 0; ++i)
                _Ptr[i] = _Tmax;
            _Ptr[i] -= _Brrw1;
        }
        for (; _Ptr[_Siz - 1] == 0 && _Siz > 1; --_Siz) {}
        return *this;
    }


namespace SchoolBook {
    void mul(_BlockType* _Ans, const _BlockType* _First, const _BlockType* _Secnd, const _SizeType _Siza, const _SizeType _Sizb) {
        _SizeType i, j;
        _BlockType _Carry;
        _MaxType _Block;

        for (i = 0; i < _Sizb; ++i) {
            if(_Secnd[i] != 0){
                _Carry = 0;
                for (j = 0; j < _Siza; ++j) {
                    _Block = static_cast<_MaxType>(_Secnd[i]) * _First[j] + _Ans[i + j] + _Carry;
                    _Ans[i + j] = _Block;
                    _Carry = _Block >> (sizeof(_BlockType) * 8);
                }
                _Ans[i + _Siza] += _Carry;
            }
        }
    }
    void mul(_BlockType* _Ans, _BlockType* _First, _BlockType* _Secnd, _SizeType _Siz) {
        // Assumed that both array are equal size;
        _SizeType i, j;
        for (i = 0; i < _Siz * 2; ++i) _Ans[i] = 0;
        _BlockType _Carry = 0;
        _MaxType _Block;

        for (i = 0; i < _Siz; ++i) {
            if (_First[i] != 0) {
                _Carry = 0;
                for (j = 0; j < _Siz; ++j) {
                    _Block = static_cast<_MaxType>(_First[i]) * _Secnd[j] + _Ans[i + j] + _Carry;
                    _Ans[i + j] = static_cast<_BlockType>(_Block);
                    _Carry = _Block >> (sizeof(_BlockType) * 8);
                }
                _Ans[i + _Siz] += _Carry;
            }
        }
    }
}


namespace Karatsuba {
    // Assumed that _Rsiz >= _Lsiz
    inline void _Swap(_BlockType* x0y1, const _SizeType _Lsiz, const _SizeType _Rsiz) {
        if (_Lsiz == _Rsiz) {
            for (_SizeType i = 0; i < _Rsiz; ++i) {
                _BlockType _Tmp = x0y1[i];
                x0y1[i] = x0y1[_Rsiz + i];
                x0y1[_Rsiz + i] = _Tmp;
            }
        } else {
            _BlockType _Tmp = x0y1[0];
            for (_SizeType i = 1; i <= _Lsiz; ++i) {
                x0y1[i - 1] = x0y1[i + _Lsiz];
                x0y1[i + _Lsiz] = x0y1[i];
            }
            x0y1[_Lsiz] = _Tmp;
        }
    }

    _SizeType _Addition(const _BlockType* A, const _SizeType _Siza, const _BlockType* B, const _SizeType _Sizb, _BlockType* _Buff) {
        // Assumed that _Siza >= _Sizb;
        _MaxType _Block;
        _BlockType _Carry = 0;
        _SizeType i = 0;

        for (; i < _Sizb; ++i) {
            _Block = (_MaxType)A[i] + B[i] + _Carry;
            _Buff[i] = _Block;
            _Carry = _Block >> (sizeof(_BlockType) * 8);
        }
        for (; _Carry & (i < _Siza); ++i) {
            _Buff[i] = A[i] + 1;
            _Carry = _Buff[i] == 0;
        }
        if (_Carry) {
            _Buff[i] = 1; return ++i;
        }
        while (i < _Siza) {
            _Buff[i] = A[i];
            ++i;
        }
        return i;
    }

    void _InPlaceAddition(_BlockType* A, const _BlockType* const B, const _SizeType _Sizb) {
        // This function mimics the operation A += B.
        // Assumed that _Siza > _Sizb
        _BlockType _Cache = 0;
        bool _Carry = false;
        _SizeType i = 0;
        for (; i < _Sizb; ++i) {
            _Cache = A[i] + B[i];
            A[i] = _Cache + _Carry;
            _Carry = _Cache == _Tmax && _Carry ? 1 : _Cache < B[i];
        }
        if (_Carry) for (++A[i]; A[i] == 0; ++A[++i]) {}

    }
    _SizeType _InPlaceSubtraction(_BlockType* A, _SizeType _SizA, const _BlockType* const B, _SizeType _SizB) {
        // This function mimic the operator -=.
        // Assumed that Integer A > Integer B;

        bool _Brrw1 = false;
        bool _Brrw2 = false;
        _SizeType i = 0;
        for (; i < _SizB; ++i) {
            _Brrw2 = A[i] == B[i] && _Brrw1 ? 1 : A[i] < B[i];
            A[i] = A[i] - B[i] - _Brrw1;
            _Brrw1 = _Brrw2;
        }


        if (_Brrw1) {
            for (; A[i] == 0; ++i)
                A[i] = _Tmax;
            --A[i];
        }

        if (A[_SizA - 1] == 0) --_SizA;

        return _SizA;

    }

    _SizeType _MakeEqualLength(_BlockType* xy, const _SizeType Ly, const _SizeType Lx) {
        const _SizeType _Siz = Ly + Lx;
        if (Ly > Lx) {
            const _SizeType d = Ly - Lx;
            for (_SizeType i = 0; i < d; ++i)
                xy[_Siz + i] = 0;
            return Ly;
        }
        _SignedType d = Lx - Ly;
        for (_SignedType i = _Siz - 1; i >= Ly; --i)
            xy[i + d] = xy[i];
        while (d--)
            xy[Ly + d] = 0;
        return Lx;
    }

    void mul(_BlockType* x1y1x0y0, _SizeType n, _BlockType* P) {
        if (n <= 32) {
            SchoolBook::mul(P, x1y1x0y0 + n, x1y1x0y0, n);
            memcpy(x1y1x0y0, P, n * 2 * sizeof(_BlockType));
            return;
        }
        _SizeType _Lsiz = n >> 1, _Rsiz = n - _Lsiz;
        _PointerType x1y1 = x1y1x0y0 + _Lsiz * 2;
        _Swap(x1y1x0y0 + _Lsiz, _Lsiz, _Rsiz); // O(n) with very low constant
        _SizeType L1 = Karatsuba::_Addition(x1y1, _Rsiz, x1y1x0y0, _Lsiz, P); // store y1 + y0 in First half of P
        _SizeType L2 = Karatsuba::_Addition(x1y1 + _Rsiz, _Rsiz, x1y1x0y0 + _Lsiz, _Lsiz, P + L1); // store x1 + x0 in second half of P
        if (L1 != L2)
            L1 = Karatsuba::_MakeEqualLength(P, L1, L2); // Best case O(1), worst case O(n) with very low constant
        _PointerType _Memory = P + L1 * 2;
        Karatsuba::mul(P, L1, _Memory); // P = (y1 + y0)(x1 + x0);
        Karatsuba::mul(x1y1, _Rsiz, _Memory); // x1y1 = x1*y1;
        Karatsuba::mul(x1y1x0y0, _Lsiz, _Memory); // x0y0 = x0 * y0
        L1 = Karatsuba::_InPlaceSubtraction(P, L1 * 2, x1y1, _Rsiz * 2); // p -= x1y1;
        L1 = Karatsuba::_InPlaceSubtraction(P, L1, x1y1x0y0, _Lsiz * 2);  // p -= x0y0;
        // x1y1x0y0 = x1y1 * 2 ^ n + x0y0; So we don't need to add any additional zeros behind x1y1.
        Karatsuba::_InPlaceAddition(x1y1x0y0 + _Lsiz, P, L1); // ==> x1y1x0y0 += p * 2^n/2;
    }
}


    UnsignedInt& UnsignedInt::operator*=(const UnsignedInt& _That){
        _SizeType _expected_siz;
        if(_Siz >= _That._Siz) {
            _expected_siz = _Siz * 2;
            if(_expected_siz > _Cap)
                _Reallocate(_expected_siz);
            for(_SizeType i = 0; i < _That._Siz; ++i)
                _Ptr[i + _Siz] = _That._Ptr[i];
            for(_SizeType i = _Siz + _That._Siz; i < _expected_siz; ++i)
                _Ptr[i] = 0;
        } else {
            _expected_siz = _That._Siz * 2;
            if(_expected_siz > _Cap)
                _Reallocate(_expected_siz);
            for(_SizeType i = _Siz; i < _That._Siz; ++i)
                _Ptr[i] = 0;
            for(_SizeType i = 0; i < _That._Siz; ++i)
                _Ptr[i + _That._Siz] = _That._Ptr[i];
        }
        _PointerType _Pool = static_cast<_PointerType>(operator new[]((_expected_siz + 100) * sizeof(_BlockType)));
        Karatsuba::mul(_Ptr, _Siz, _Pool);
        operator delete[](_Pool);
        _Siz = _expected_siz;
        for (; _Ptr[_Siz- 1] == 0 && _Siz > 1; --_Siz) {}
        return *this;
    }


}