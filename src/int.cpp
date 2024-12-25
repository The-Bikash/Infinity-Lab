#include "../include/Number/int.h"
#include<stdio.h>
#include<time.h>


#define putchar _print
#define fputs _print
#define console stdout
#define __specialcompiler 0

namespace alpha {

    #if __clang__
    static constexpr const auto _shft = 4;              // sizeof(uint128_t) = 16 = 2^4
    static constexpr const auto _hshft = 3;             // sizeof(size_t) = 8 = 2^3
    static constexpr const auto _lsft = 7;              // 128 = 2^7
    static constexpr const auto _hlsft = 6;             // 64 = 2^6
    static constexpr const auto _byte = 16;             // sizeof(uint128_t) = 16
    static constexpr const auto _bits = 128;            // bits count of uint128_t = 128
    static constexpr const auto _hbits = 64;            // bits count of size_t = 64
    static constexpr const auto _zero = (_Mtype)(0);
    static constexpr const auto _Hmax = ULLONG_MAX;
    static constexpr const auto _Tmax = (((_Mtype)ULLONG_MAX) << 64) | ULLONG_MAX;
    static const char _TotalHex = 16; // 16 * 4 = 64, this signifies that one block can store 16 length hex integer
    static const char _shift = 64 - 4;
    static const _BlockType _Tmax = 0xffffffffffffffff; // _Maximum can store unsigned long long data type.
    static const _BlockType _mask = 0xf000000000000000;
    #else
    static constexpr const auto _shft = 3;              // sizeof(uint64_t) = 8 = 2^3
    static constexpr const auto _hshft = 2;             // sizeof(uint32_t) = 4 = 2^2
    static constexpr const auto _lsft = 6;              // 64 = 2^6
    static constexpr const auto _hlsft = 5;             // 32 = 2^5
    static constexpr const auto _byte = 8;              // sizeof(uint64_t) = 8
    static constexpr const auto _bits = 64;             // bits count of uint64_t = 64
    static constexpr const auto _hbits = 32;            // bits count of uint32_t = 32
    static constexpr const auto _zero = (_Mtype)(0);
    static constexpr const unsigned  _Hmax = -1;
    static constexpr const unsigned long long _Tmax = -1;
    static const char _TotalHex = 8; // 8 * 4 = 32
    static const char _shift = 32 - 4;
    static const _BlockType _mask = 0xf0000000;
    #endif


    [[nodiscard]] inline constexpr bool UnsignedInt::operator> (const UnsignedInt& _That)const noexcept {
        if (_Siz == _That._Siz) {
            for (_SignedType i = _Siz - 1; i >= 0; --i)
                if (_Ptr[i] != _That._Ptr[i])
                    return _Ptr[i] > _That._Ptr[i];
            return false;
        }
        else return _Siz > _That._Siz;
    }

    [[nodiscard]] constexpr bool UnsignedInt::operator< (const UnsignedInt& _That)const noexcept {
        return _That > *this;
    }

    [[nodiscard]] constexpr bool UnsignedInt::operator>=(const UnsignedInt& _That)const noexcept {
        return !(_That > *this);
    }

    [[nodiscard]] constexpr bool UnsignedInt::operator<=(const UnsignedInt& _That)const noexcept {
        return !(*this > _That);
    }


    constexpr UnsignedInt& UnsignedInt::operator<<=(const _SizeType _Shift)noexcept {
        const auto _Sft = _Shift >> _lsft;
        const auto _Rft = _Shift - (_Sft << _lsft);
        const auto _Msz = _Siz + _Sft + 1;
        if (_Msz >= _Cap)
            _Reallocate(_Msz);
        if (_Sft) {
            memmove(_Ptr + _Sft, _Ptr, _Siz << _shft);
            memset(_Ptr, 0, _Sft << _shft);
            _Siz += _Sft;
        }
        if (_Rft) {
            auto _Rsift = _bits - _Rft;
            auto _Mask1 = _zero;
            auto _Mask2 = _zero;
            for (auto i = _Sft; i < _Siz; ++i) {
                _Mask2 = _Ptr[i] >> _Rsift;
                _Ptr[i] = _Ptr[i] << _Rft;
                _Ptr[i] = _Ptr[i] | _Mask1;
                _Mask1 = _Mask2;
            }
            if (_Mask1) {
                _Ptr[_Siz] = 0;
                _Ptr[_Siz] |= _Mask1;
                ++_Siz;
            }
        } return *this;
    }

    constexpr UnsignedInt& UnsignedInt::operator>>=(const _SizeType _Shift)noexcept {
        const auto _Sft = _Shift >> _lsft;
        const auto _Rft = _Shift - (_Sft << _lsft);
        if (_Sft) {
            if (_Sft >= _Siz) {
                _Ptr[0] = 0;
                _Siz = 1;
                return *this;
            }
            _Siz -= _Sft;
            memmove(_Ptr, _Ptr + _Sft, (size_t)_Siz << _shft);
        }
        if (_Rft) {
            auto _Rsift = _bits - _Rft;
            auto _Mask1 = _zero;
            auto _Mask2 = _zero;
            for (_SignedType i = _Siz - 1; i >= 0; --i) {
                _Mask2 = _Ptr[i] << _Rsift;
                _Ptr[i] = _Ptr[i] >> _Rft;
                _Ptr[i] = _Ptr[i] | _Mask1;
                _Mask1 = _Mask2;
            }
            if (_Siz != 1 && _Ptr[_Siz - 1] == 0)
                --_Siz;
        } return *this;
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator<<(const _SizeType that)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp <<= that;
        return _Tmp;
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator>>(const _SizeType that)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp >>= that;
        return _Tmp;
    }


    void UnsignedInt::_Update(_SizeType _ThatSiz)noexcept{
        _Reallocate(_ThatSiz);
        memset(_Ptr + _Siz, 0, (_ThatSiz - _Siz) * sizeof(_SizeType));
    }

    #define _BitwiseOperation(_Operator)\
    if (_That._Siz > _Siz)\
        _Update(_That._Siz);\
    for (_SizeType i = 0; i < _That._Siz; ++i)\
        _Ptr[i] _Operator _That._Ptr[i];\
    return *this;

    constexpr UnsignedInt& UnsignedInt::operator|=(const UnsignedInt& _That)noexcept {
        if(this == &_That) return *this;
        _BitwiseOperation(|=)
    }

    constexpr UnsignedInt& UnsignedInt::operator&=(const UnsignedInt& _That)noexcept {
        if(this == &_That) return *this;
        _BitwiseOperation(&=)
    }

    constexpr UnsignedInt& UnsignedInt::operator^=(const UnsignedInt& _That)noexcept {
        if(this == &_That) {
            resize(1, 0);
            return *this;
        }
        _BitwiseOperation(^=)
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator|(const UnsignedInt& _That)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp |= _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator&(const UnsignedInt& _That)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp &= _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator^(const UnsignedInt& _That)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp ^= _That;
        return _Tmp;
    }
    
    
    
    
    constexpr UnsignedInt& UnsignedInt::operator+=(const UnsignedInt& _That)noexcept {
        if (_That._Siz > _Siz) _Update(_That._Siz);
        auto _Cary1 = false;
        auto _Cary2 = false;
        auto _Cache = _zero;
        for (_SizeType i = 0; i < _That._Siz; ++i) {
            _Cache = _Ptr[i] + _That._Ptr[i];
            _Cary2 = _Cache == _Tmax && _Cary1 ? true : _Cache < _Ptr[i];
            _Ptr[i] = _Cache + _Cary1;
            _Cary1 = _Cary2;
        }
        for (auto i = _That._Siz; i < _Siz && _Cary1; ++i) {
            _Cary2 = _Ptr[i] == _Tmax;
            _Ptr[i] = _Ptr[i] + _Cary1;
            _Cary1 = _Cary2;
        }
        if (_Cary1) {
            if (_Siz == _Cap) _Reallocate(_Siz + 1);
            _Ptr[_Siz] = 1; ++_Siz;
        }
        return *this;
    }

    constexpr UnsignedInt& UnsignedInt::operator-=(const UnsignedInt& _That)noexcept {
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

    [[nodiscard]] UnsignedInt UnsignedInt::operator+(const UnsignedInt& _That)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp += _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedInt UnsignedInt::operator-(const UnsignedInt& _That)const noexcept {
        UnsignedInt _Tmp(*this);
        _Tmp -= _That;
        return _Tmp;
    }

    
    
void randomHex(char* buff, _SizeType size) {
    unsigned int seed = static_cast<unsigned int>(clock());
    const char hexcode[] = "0123456789abcdef";
    srand(seed);
    for (size_t i = 0; i < size; ++i) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        buff[i] = hexcode[seed & 0xF];
    }
}

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

UnsignedInt::UnsignedInt (const char* _Hex){
    _SizeType _Len = strlen(_Hex);
    _Hex = _Refine(_Hex, &_Len);

    _Allocate(_Len / _TotalHex + 1);
    _TransformHexToInt(_Hex, _Len, _Ptr);
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
    
    
    
    
    
    
    
    
    
    
    
    
//     constexpr UnsignedInt& UnsignedInt::operator*=(const UnsignedInt& _That)noexcept {
//         return *this;
//     }

//     constexpr UnsignedInt& UnsignedInt::operator/=(const UnsignedInt& _That)noexcept {
//         return *this;
//     }

//     constexpr UnsignedInt& UnsignedInt::operator%=(const UnsignedInt& _That)noexcept {
//         return *this;
//     }






//     void _print(const UnsignedInt& _That)noexcept {
//         char _Buff[_bits + 1]{};
//         _Buff[_bits] = '\0';
//         _Mtype _Mask = 0;
//         _Mtype _Tmp = 1; _Tmp <<= (_bits - 1);
//         for (_SignedType i = _That._Siz - 1; i >= 0; --i) {
//             _Mask = _Tmp;
//             for (auto j = 0; j < _bits; ++j) {
//                 _Buff[j] = (_That._Ptr[i] & _Mask) ? '1' : '0';
//                 _Mask >>= 1;
//             }
//             //fputs(_Buff, console);
//         }
//     }

//     inline void Add(UnsignedInt& _Answr, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept {
//         _Mtype* _This = 0;
//         _SizeType  _Siza = 0;
//         _Mtype* _That = 0;
//         _SizeType  _Sizb = 0;

//         if (_First._Siz >= _Secnd._Siz) {
//             _This = _First._Ptr;
//             _That = _Secnd._Ptr;
//             _Siza = _First._Siz;
//             _Sizb = _Secnd._Siz;
//         }
//         else {
//             _That = _First._Ptr;
//             _This = _Secnd._Ptr;
//             _Sizb = _First._Siz;
//             _Siza = _Secnd._Siz;
//         }

//         auto _Cache = _zero;
//         auto _Cary1 = false;
//         auto _Cary2 = false;

//         if (_Siza >= _Answr._Cap) {
//             free(_Answr._Ptr);
//             _Answr._Alloc(_Siza + 1);
//         } _Answr._Siz = _Siza;

//         for (_SizeType i = 0; i < _Sizb; ++i) {
//             _Cache = _This[i] + _That[i];
//             _Cary2 = _Cache == _Tmax && _Cary1 ? true : _Cache < _This[i];
//             _Answr._Ptr[i] = _Cache + _Cary1;
//             _Cary1 = _Cary2;
//         }
//         for (auto i = _Sizb; i < _Siza && _Cary1; ++i) {
//             _Cary2 = _This[i] == _Tmax;
//             _Answr._Ptr[i] = _This[i] + _Cary1;
//             _Cary1 = _Cary2;
//         }
//         if (_Cary1) {
//             _Answr._Ptr[_Answr._Siz] = 1; ++_Answr._Siz;
//         }
//     }

// #if false
//     void mul(Const dynamic_unsigned_int& _Answr,
//         const dynamic_unsigned_int& _First,
//         const dynamic_unsigned_int& _Secnd)noexcept
//     {
//         _Answr = _First._Siz + _Secnd._Siz;

//         dynamic_unsigned_int::_Mtype i, j, _Slot, _Cche;
//         auto _Cary = dynamic_unsigned_int::_zero;
//         auto _Cry1 = false;
//         auto _Cry2 = false;
//         __uint128_t _Blck;
//         auto _Vlue = _Secnd._Ptr[0];

//         std::memset(_Answr._Ptr + _First._Siz, 0, _Secnd._Siz << 3);

//         for (i = 0; i < _First._Siz; ++i) {
//             _Blck = _First._Ptr[i];
//             _Blck = _Blck * _Vlue + _Cary;
//             _Cary = _Blck >> 64;
//             _Answr._Ptr[i] = _Blck & dynamic_unsigned_int::_Tmax;
//         }   _Answr._Ptr[i] = _Cary;

//         for (i = 1; i < _Secnd._Siz; ++i) {
//             _Vlue = _Secnd._Ptr[i];
//             if (_Vlue != 0) {
//                 _Cary = 0; _Answr._Siz = i; _Cry1 = 0;
//                 for (j = 0; j < _First._Siz; ++j) {
//                     _Blck = _First._Ptr[j];
//                     _Blck = _Blck * _Vlue + _Cary;
//                     _Cary = _Blck >> 64;
//                     _Slot = _Blck & dynamic_unsigned_int::_Tmax;
//                     _Cche = _Answr._Ptr[_Answr._Siz] + _Slot;
//                     _Cry2 = _Cche == dynamic_unsigned_int::_Tmax && _Cry1 ? true : _Cche < _Answr._Ptr[_Answr._Siz];
//                     _Answr._Ptr[_Answr._Siz] = _Cche + _Cry1;
//                     _Cry1 = _Cry2; ++_Answr._Siz;
//                 }
//                 if (_Cary != 0 && _Cry1) {
//                     _Answr._Ptr[_Answr._Siz] = _Cary + _Cry1; ++_Answr._Siz;
//                     if (_Cary == dynamic_unsigned_int::_Tmax) {
//                         _Answr._Ptr[_Answr._Siz] = 1; ++_Answr._Siz;
//                     }
//                 }
//                 else if (_Cary) {
//                     _Answr._Ptr[_Answr._Siz] = _Cary; ++_Answr._Siz;
//                 }
//                 else if (_Cry1) {
//                     _Answr._Ptr[_Answr._Siz] = 1; ++_Answr._Siz;
//                 }
//             }
//         }
//     }
// #else 
//     /*inline void mul(dynamic_unsigned_int& _Answr, const dynamic_unsigned_int& _First, const dynamic_unsigned_int& _Secnd)noexcept {
//         _Answr = _First._Siz + _Secnd._Siz;

//         auto _Tspt = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_First._Ptr);
//         auto _Ttpt = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_Secnd._Ptr);
//         auto _A = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_Answr._Ptr);

//         auto _Tssz = _First._Siz << 1; _Tssz -= (_Tspt[_Tssz - 1] == 0);
//         auto _Ttsz = _Secnd._Siz << 1; _Ttsz -= (_Ttpt[_Ttsz - 1] == 0);

//         dynamic_unsigned_int::_Htype _Slot, _Cche, _Vlue = _Ttpt[0], _Cary = 0U;
//         dynamic_unsigned_int::_Mtype _Blck;
//         dynamic_unsigned_int::_SizeType i, j, I = 0;

//         auto _Cry1 = false;
//         auto _Cry2 = false;

//         iq::memset(_A + _Tssz, 0, (_Ttsz + 1) << _hshft);

//         for (i = 0; i < _Tssz; ++i) {
//             _Blck = _Tspt[i];
//             _Blck = _Blck * _Vlue + _Cary;
//             _Cary = _Blck >> _hbits;
//             _A[i] = _Blck & _Hmax;
//         }   _A[i] = _Cary;

//         for (i = 1; i < _Ttsz; ++i) {
//             _Vlue = _Ttpt[i];
//             if (_Vlue != 0) {
//                 _Cary = 0; I = i; _Cry1 = 0;
//                 for (j = 0; j < _Tssz; ++j) {
//                     _Blck = _Tspt[j];
//                     _Blck = _Blck * _Vlue + _Cary;
//                     _Cary = _Blck >> _hbits;
//                     _Slot = _Blck & _Hmax;

//                     _Cche = _A[I] + _Slot;
//                     _Cry2 = _Cche == _Hmax && _Cry1 ? true : _Cche < _A[I];
//                     _A[I] = _Cche + _Cry1;
//                     _Cry1 = _Cry2; ++I;
//                 }
//                 if (_Cary != 0 && _Cry1) {
//                     _A[I] = _Cary + _Cry1; ++I;
//                     if (_Cary == _Hmax) {
//                         _A[I] = 1; ++I;
//                     }
//                 }
//                 else if (_Cary) {
//                     _A[I] = _Cary; ++I;
//                 }
//                 else if (_Cry1) {
//                     _A[I] = 1; ++I;
//                 }
//             }
//         }
//         _Answr._Siz = I & 1 ? (++I) >> 1 : I >> 1;
//     }*/
// #endif
//     /* inline unsigned int* _Addition(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2, unsigned int& _Siz) {
//          auto _Ptr = (unsigned int*)std::malloc((size_t)(_Siz1 + 1) << 2); _Ptr[_Siz1] = 0;
//          auto _Cary1 = false;
//          auto _Cary2 = false;
//          auto _Cache = 0U;
//          for (auto i = 0U; i < _Siz2; ++i) {
//              _Cache = _Num1[i] + _Num2[i];
//              _Cary2 = _Cache == UINT32_MAX && _Cary1 ? true : _Cache < _Num1[i];
//              _Ptr[i] = _Cache + _Cary1;
//              _Cary1 = _Cary2;
//          }
//          for (auto i = _Siz2; i < _Siz1 && _Cary1; ++i) {
//              _Cary2 = _Num1[i] == UINT32_MAX;
//              _Ptr[i] = _Num1[i] + _Cary1;
//              _Cary1 = _Cary2;
//          }
//          _Siz = _Siz1;
//          if (_Cary1) {
//              _Ptr[_Siz] = 1; ++_Siz;
//          } return _Ptr;
//      }

//      void _Print(unsigned int* _Ptr, unsigned int _Siz)noexcept {
//          char _Buff[33]{}; _Buff[32] = '\0';
//          auto _Mask = 0ULL;
//          for (int i = _Siz - 1; i >= 0; --i) {
//              _Mask = 0b10000000000000000000000000000000;
//              for (auto j = 0; j < 32; ++j) {
//                  _Buff[j] = (_Ptr[i] & _Mask) ? '1' : '0';
//                  _Mask >>= 1;
//              }
//              std::fputs(_Buff, stdout);
//          }

//      }

//      dynamic_unsigned_int _Karatsuba2(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2);

//      dynamic_unsigned_int _Karatsuba1(unsigned int* _Num1, unsigned int* _Num2, unsigned int _Idx) {

//          if (_Idx == 1) return { ((size_t)(*_Num1)) * (*_Num2) };

//          auto _Idx1 = _Idx / 2;
//          auto _Idx2 = _Idx - _Idx1;

//          dynamic_unsigned_int _BD = _Karatsuba1(_Num1, _Num2, _Idx1);
//          dynamic_unsigned_int _AC = _Karatsuba1(_Num1 + _Idx1, _Num2 + _Idx1, _Idx2);

//          unsigned int _A_PLUS_B_SIZ; auto _A_PLUS_B = _Addition(_Num1 + _Idx1, _Idx2, _Num1, _Idx1, _A_PLUS_B_SIZ);
//          unsigned int _C_PLUS_D_SIZ; auto _C_PLUS_D = _Addition(_Num2 + _Idx1, _Idx2, _Num2, _Idx1, _C_PLUS_D_SIZ);

//          dynamic_unsigned_int _AD_PLUS_BC = _Karatsuba2(_A_PLUS_B, _A_PLUS_B_SIZ, _C_PLUS_D, _C_PLUS_D_SIZ);

//          std::free(_A_PLUS_B);
//          std::free(_C_PLUS_D);

//          _AD_PLUS_BC -= _AC;
//          _AD_PLUS_BC -= _BD;
//          _AD_PLUS_BC <<= _Idx1 << 5;

//          _AC <<= _Idx1 << 6;

//          _AC += _AD_PLUS_BC;
//          _AC += _BD;

//          return _AC;

//      }

//      dynamic_unsigned_int _Karatsuba2(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2) {
//          if (_Siz2 < _Siz1) {
//              for (; _Siz2 < _Siz1; ++_Siz2)
//                  _Num2[_Siz2] = 0;
//          } else if (_Siz1 < _Siz2) {
//              for (; _Siz1 < _Siz2; ++_Siz1)
//                  _Num1[_Siz1] = 0;
//          }
//          auto _Idx = _Siz1;
//          auto _Idx1 = _Idx / 2;
//          auto _Idx2 = _Idx - _Idx1;

//          if (_Idx == 1) return { ((size_t)(*_Num1)) * (*_Num2) };

//          dynamic_unsigned_int _BD = _Karatsuba1(_Num1, _Num2, _Idx1);
//          dynamic_unsigned_int _AC = _Karatsuba1(_Num1 + _Idx1, _Num2 + _Idx1, _Idx2);

//          unsigned int _A_PLUS_B_SIZ;
//          unsigned int _C_PLUS_D_SIZ;

//          auto _A_PLUS_B = _Addition(_Num1 + _Idx1, _Idx2, _Num1, _Idx1, _A_PLUS_B_SIZ);
//          auto _C_PLUS_D = _Addition(_Num2 + _Idx1, _Idx2, _Num2, _Idx1, _C_PLUS_D_SIZ);

//          dynamic_unsigned_int _AD_PLUS_BC = _Karatsuba2(_A_PLUS_B, _A_PLUS_B_SIZ, _C_PLUS_D, _C_PLUS_D_SIZ);

//          std::free(_A_PLUS_B);
//          std::free(_C_PLUS_D);

//          _AD_PLUS_BC -= _AC;
//          _AD_PLUS_BC -= _BD;
//          _AD_PLUS_BC <<= _Idx1 << 5;

//          _AC <<= _Idx1 << 6;

//          _AC += _AD_PLUS_BC;
//          _AC += _BD;

//          return _AC;

//      }

//      dynamic_unsigned_int karatsuba(dynamic_unsigned_int& _This, dynamic_unsigned_int& _That)noexcept {

//          auto _Size = dynamic_unsigned_int::_Fndsiz(_This._Siz);

//          std::memset(_This._Ptr + _This._Siz, 0, (size_t)(_Size - _This._Siz) << 3);
//          std::memset(_That._Ptr + _That._Siz, 0, (size_t)(_Size - _That._Siz) << 3);

//          auto _Num1 = reinterpret_cast<unsigned int*>(_This._Ptr);
//          auto _Num2 = reinterpret_cast<unsigned int*>(_That._Ptr);
//          _Size <<= 1;

//          dynamic_unsigned_int _Res = _Karatsuba1(_Num1, _Num2, _Size);

//          for (; _Res._Ptr[_Res._Siz - 1] == 0 && _Res._Siz > 0; --_Res._Siz);



//          return _Res;

//      }*/
// };

}
