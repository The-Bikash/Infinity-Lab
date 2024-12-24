#include "../include/Number/Int.h"
#include<stdio.h>


#define putchar _print
#define fputs _print
#define console stdout
#define __specialcompiler 0

namespace alpha {
    using _Mtype = long long;
    using _Type = long long;
    using _Htype = unsigned int;
    using _Stype = unsigned long long;
    using longInitializer = initializer<unsigned long long>;

    //#if __clang__
    //static constexpr const auto _shft = 4;              // sizeof(uint128_t) = 16 = 2^4
    //static constexpr const auto _hshft = 3;             // sizeof(size_t) = 8 = 2^3
    //static constexpr const auto _lsft = 7;              // 128 = 2^7
    //static constexpr const auto _hlsft = 6;             // 64 = 2^6
    //static constexpr const auto _byte = 16;             // sizeof(uint128_t) = 16
    //static constexpr const auto _bits = 128;            // bits count of uint128_t = 128
    //static constexpr const auto _hbits = 64;            // bits count of size_t = 64
    //static constexpr const auto _zero = (_Mtype)(0);
    //static constexpr const auto _Hmax = ULLONG_MAX;
    //static constexpr const auto _Tmax = (((_Mtype)ULLONG_MAX) << 64) | ULLONG_MAX;
    //#else
    static constexpr const auto _shft = 3;              // sizeof(uint64_t) = 8 = 2^3
    static constexpr const auto _hshft = 2;             // sizeof(uint32_t) = 4 = 2^2
    static constexpr const auto _lsft = 6;              // 64 = 2^6
    static constexpr const auto _hlsft = 5;             // 32 = 2^5
    static constexpr const auto _byte = 8;              // sizeof(uint64_t) = 8
    static constexpr const auto _bits = 64;             // bits count of uint64_t = 64
    static constexpr const auto _hbits = 32;            // bits count of uint32_t = 32
    static constexpr const auto _zero = (_Mtype)(0);
    static constexpr const auto _Hmax = uint32_max;
    static constexpr const auto _Tmax = uint64_max;

    struct _Information {
        unsigned int* _Num;
        unsigned int _Siz;

    };

    void print_binary_int(unsigned int _Num) {
        auto mask = 0b10000000000000000000000000000000;
        for (auto j = 0; j < 32; ++j) {
            putchar((_Num & mask) ? '1' : '0');
            mask >>= 1;
        }
    }

    void print_binary_long(unsigned long long _Num) {
        //putchar('\n');
        auto mask = 0b1000000000000000000000000000000000000000000000000000000000000000;
        for (auto j = 0; j < 64; ++j) {
            putchar((_Num & mask) ? '1' : '0');
            mask >>= 1;
            //if (j == 32)putchar('-');
        }
        putchar('-');
    }

    struct UnsignedIntImpl {
        _Mtype* _Num;
        _Stype  _Siz;
        _Stype  _Cap;

        inline _Stype _Fndsiz(_Stype _Size) {
            _Stype _Tmp = 2;
            while (_Tmp < _Size)
                _Tmp <<= 1;
            return _Tmp;
        }

        inline void _Alloc(_Stype _Size)noexcept {
            _Size = _Fndsiz(_Size);
            auto _Ptr = malloc(_Size << _shft);
            if (_Ptr == 0) {
                //fputs("Memory not available", console);
                exit(1);
            }
            _Cap = _Size;
            _Num = static_cast<_Mtype*>(_Ptr);
        }

        inline void _Calloc(_Stype _Size)noexcept {
            auto _Ptr = calloc(_Size, _byte);
            if (_Ptr == 0) {
                //fputs("Memory not available", console);
                exit(1);
            }
            _Num = static_cast<_Mtype*>(_Ptr);
            _Cap = _Size;
        }

        inline void _Rlloc(_Stype _Size) noexcept {
            if (_Size < _Siz) _Siz = _Size;
            _Size = _Fndsiz(_Size);
            auto _Tmp = realloc(_Num, _Size << _shft);

            if (_Tmp == 0) {
                //fputs("Memory not available", console);
                exit(1);
            }
            if constexpr (_debug)
                //fputs("\nReallocation Happened\n", console);
            _Num = static_cast<_Mtype*>(_Tmp);
            _Cap = _Size;
        }

        inline void _Update(_Stype _Size)noexcept {
            if (_Size >= _Cap)
                _Rlloc(_Size + 1);
            memset(_Num + _Siz, 0, (_Size - _Siz) << _shft);
            _Siz = _Size;
        }

        inline void _Free()noexcept {
            free(_Num);
            _Num = nullptr;
            _Siz = 0;
            _Cap = 0;
        }


    UnsignedIntImpl()noexcept {
        _Calloc(2);
        _Siz = 1;
    }

    UnsignedIntImpl(const longInitializer& _List)noexcept {
        if constexpr (__specialcompiler) {
            _Siz = _List.size();
            _Siz = _Siz & 1 ? (++_Siz) >> 1 : _Siz >> 1;

            _Alloc(_Siz);

            _Stype _Index = 0;
            _Stype i = 0;
            _Mtype _Tmp = 0;
            for (const auto& _Slot : _List) {
                if (i & 1) {
                    _Tmp = _Slot;
                    _Tmp <<= 64;
                    _Num[_Index] |= _Tmp;
                    _Index++;
                }
                else {
                    _Num[_Index] = _Slot;
                }
                ++i;
            }
        }
        else {
            _Siz = _List.size();
            _Alloc(_Siz);
            auto _Index = 0U;
            for (const auto& _Slot : _List)
                _Num[_Index++] = _Slot;
        }
    }

    UnsignedIntImpl(const _Stype _Size)noexcept {
         _Alloc(_Size);
        _Num[0] = 0;
        _Siz = 1;
    }

    UnsignedIntImpl(const UnsignedIntImpl& _That)noexcept {
        _Siz = _That._Siz;
        _Alloc(_That._Cap);
        memcpy(_Num, _That._Num, _Siz << _shft);
    }

    UnsignedIntImpl(UnsignedIntImpl&& _That)noexcept {
        _Num = _That._Num;
        _Siz = _That._Siz;
        _Cap = _That._Cap;

        _That._Num = nullptr;
        _Siz = 0;
        _Cap = 0;
    }

    ~UnsignedIntImpl()noexcept {
        if (_Num != nullptr)
            _Free();
    }

    constexpr UnsignedIntImpl& operator=(const longInitializer& _That)noexcept {
        if constexpr (__specialcompiler) {
            _Siz = _That.size();
            _Siz = _Siz & 1 ? (++_Siz) >> 1 : _Siz >> 1;
            if (_Siz >= _Cap) {
                free(_Num);
                _Alloc(_Siz);
            }

            _Stype _Index = 0;
            _Stype i = 0;
            _Mtype _Tmp = 0;
            for (const auto& _Slot : _That) {
                if (i & 1) {
                    _Tmp = _Slot;
                    _Tmp <<= 64;
                    _Num[_Index] |= _Tmp;
                    _Index++;
                }
                else {
                    _Num[_Index] = _Slot;
                }
                ++i;
            }
        }
        else {
            _Siz = _That.size();
            if (_Siz >= _Cap) {
                free(_Num);
                _Alloc(_Siz);
            }
            auto _Index = 0U;
            for (const auto& _Slot : _That)
                _Num[_Index++] = _Slot;
        }
        return *this;
    }

    constexpr UnsignedIntImpl& operator=(const _Stype _Size)noexcept {
        if (_Size > _Cap) {
            free(_Num);
            _Alloc(_Size);
        }
        _Siz = 1;
        _Num[0] = 0;
        return *this;
    }

    constexpr UnsignedIntImpl& operator=(const UnsignedIntImpl& _That)noexcept {
        if (this != &_That) {
            if (_That._Siz > _Cap) {
                free(_Num);
                _Alloc(_That._Siz);
            }
            _Siz = _That._Siz;
            memcpy(_Num, _That._Num, _That._Siz << _shft);
        }
        return *this;
    }

    constexpr UnsignedIntImpl& operator=(UnsignedIntImpl&& _That)noexcept {
        if (this != &_That) {
            _Free();
        }
        return *this;
    }


    [[nodiscard]] constexpr bool operator==(const UnsignedIntImpl& _That)const noexcept {
        if (_Siz == _That._Siz) {
            auto _Ans = memcmp(_Num, _That._Num, (size_t)_Siz << _shft);
            return _Ans == 0;
        }
        else return false;
    }

    [[nodiscard]] constexpr bool operator!=(const UnsignedIntImpl& _That)const noexcept {
        return !(*this == _That);
    }

    [[nodiscard]] inline constexpr bool operator> (const UnsignedIntImpl& _That)const noexcept {
        if (_Siz == _That._Siz) {
            for (_Type i = _Siz - 1; i >= 0; --i)
                if (_Num[i] != _That._Num[i])
                    return _Num[i] > _That._Num[i];
            return false;
        }
        else return _Siz > _That._Siz;
    }

    [[nodiscard]] constexpr bool operator< (const UnsignedIntImpl& _That)const noexcept {
        return _That > *this;
    }

    [[nodiscard]] constexpr bool operator>=(const UnsignedIntImpl& _That)const noexcept {
        return !(_That > *this);
    }

    [[nodiscard]] constexpr bool operator<=(const UnsignedIntImpl& _That)const noexcept {
        return !(*this > _That);
    }


    constexpr UnsignedIntImpl& operator<<=(const _Stype _Shift)noexcept {
        const auto _Sft = _Shift >> _lsft;
        const auto _Rft = _Shift - (_Sft << _lsft);
        const auto _Msz = _Siz + _Sft + 1;
        if (_Msz >= _Cap)
            _Rlloc(_Msz);
        if (_Sft) {
            memmove(_Num + _Sft, _Num, _Siz << _shft);
            memset(_Num, 0, _Sft << _shft);
            _Siz += _Sft;
        }
        if (_Rft) {
            auto _Rsift = _bits - _Rft;
            auto _Mask1 = _zero;
            auto _Mask2 = _zero;
            for (auto i = _Sft; i < _Siz; ++i) {
                _Mask2 = _Num[i] >> _Rsift;
                _Num[i] = _Num[i] << _Rft;
                _Num[i] = _Num[i] | _Mask1;
                _Mask1 = _Mask2;
            }
            if (_Mask1) {
                _Num[_Siz] = 0;
                _Num[_Siz] |= _Mask1;
                ++_Siz;
            }
        } return *this;
    }

    constexpr UnsignedIntImpl& operator>>=(const _Stype _Shift)noexcept {
        const auto _Sft = _Shift >> _lsft;
        const auto _Rft = _Shift - (_Sft << _lsft);
        if (_Sft) {
            if (_Sft >= _Siz) {
                _Num[0] = 0;
                _Siz = 1;
                return *this;
            }
            _Siz -= _Sft;
            memmove(_Num, _Num + _Sft, (size_t)_Siz << _shft);
        }
        if (_Rft) {
            auto _Rsift = _bits - _Rft;
            auto _Mask1 = _zero;
            auto _Mask2 = _zero;
            for (_Type i = _Siz - 1; i >= 0; --i) {
                _Mask2 = _Num[i] << _Rsift;
                _Num[i] = _Num[i] >> _Rft;
                _Num[i] = _Num[i] | _Mask1;
                _Mask1 = _Mask2;
            }
            if (_Siz != 1 && _Num[_Siz - 1] == 0)
                --_Siz;
        } return *this;
    }

    constexpr UnsignedIntImpl& operator|=(const UnsignedIntImpl& _That)noexcept {
        if (_That._Siz > _Siz)
            _Update(_That._Siz);
        for (_Stype i = 0; i < _That._Siz; ++i)
            _Num[i] |= _That._Num[i];
        return *this;
    }

    constexpr UnsignedIntImpl& operator&=(const UnsignedIntImpl& _That)noexcept {
        if (this != &_That) {
            if (_Siz > _That._Siz) {
                for (_Stype i = 0U; i < _That._Siz; ++i)
                    _Num[i] &= _That._Num[i];
                _Siz = _That._Siz;
            }
            else {
                for (_Stype i = 0u; i < _Siz; ++i)
                    _Num[i] &= _That._Num[i];
            }
        } return *this;
    }

    constexpr UnsignedIntImpl& operator^=(const UnsignedIntImpl& _That)noexcept {
        if (_That._Siz > _Siz)
            _Update(_That._Siz);
        for (_Stype _i = 0U; _i < _That._Siz; ++_i)
            _Num[_i] ^= _That._Num[_i];
        return *this;
    }

    constexpr UnsignedIntImpl& operator+=(const UnsignedIntImpl& _That)noexcept {
        if (_That._Siz > _Siz) _Update(_That._Siz);
        auto _Cary1 = false;
        auto _Cary2 = false;
        auto _Cache = _zero;
        for (_Stype i = 0; i < _That._Siz; ++i) {
            _Cache = _Num[i] + _That._Num[i];
            _Cary2 = _Cache == _Tmax && _Cary1 ? true : _Cache < _Num[i];
            _Num[i] = _Cache + _Cary1;
            _Cary1 = _Cary2;
        }
        for (auto i = _That._Siz; i < _Siz && _Cary1; ++i) {
            _Cary2 = _Num[i] == _Tmax;
            _Num[i] = _Num[i] + _Cary1;
            _Cary1 = _Cary2;
        }
        if (_Cary1) {
            if (_Siz == _Cap) _Rlloc(_Siz + 1);
            _Num[_Siz] = 1; ++_Siz;
        }
        return *this;
    }

    constexpr UnsignedIntImpl& operator-=(const UnsignedIntImpl& _That)noexcept {
        auto _Brrw1 = false;
        auto _Brrw2 = false;
        for (_Stype i = 0; i < _That._Siz; ++i) {
            _Brrw2 = _Num[i] == _That._Num[i] && _Brrw1 ? true : _Num[i] < _That._Num[i];
            _Num[i] = _Num[i] - _That._Num[i] - _Brrw1;
            _Brrw1 = _Brrw2;
        }
        if (_Brrw1) {
            auto i = _That._Siz;
            for (; _Num[i] == 0; ++i)
                _Num[i] = _Tmax;
            _Num[i] -= _Brrw1;
        }
        for (; _Num[_Siz - 1] == 0 && _Siz > 1; --_Siz) {}
        return *this;
    }

    constexpr UnsignedIntImpl& operator*=(const UnsignedIntImpl& _That)noexcept {
        return *this;
    }

    constexpr UnsignedIntImpl& operator/=(const UnsignedIntImpl& _That)noexcept {
        return *this;
    }

    constexpr UnsignedIntImpl& operator%=(const UnsignedIntImpl& _That)noexcept {
        return *this;
    }

    [[nodiscard]] UnsignedIntImpl operator<<(const _Stype that)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp <<= that;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator>>(const _Stype that)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp >>= that;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator|(const UnsignedIntImpl& _That)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp |= _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator&(const UnsignedIntImpl& _That)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp &= _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator^(const UnsignedIntImpl& _That)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp ^= _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator+(const UnsignedIntImpl& _That)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp += _That;
        return _Tmp;
    }

    [[nodiscard]] UnsignedIntImpl operator-(const UnsignedIntImpl& _That)const noexcept {
        UnsignedIntImpl _Tmp(*this);
        _Tmp -= _That;
        return _Tmp;
    }


    void _print(const UnsignedIntImpl& _That)noexcept {
        char _Buff[_bits + 1]{};
        _Buff[_bits] = '\0';
        _Mtype _Mask = 0;
        _Mtype _Tmp = 1; _Tmp <<= (_bits - 1);
        for (_Type i = _That._Siz - 1; i >= 0; --i) {
            _Mask = _Tmp;
            for (auto j = 0; j < _bits; ++j) {
                _Buff[j] = (_That._Num[i] & _Mask) ? '1' : '0';
                _Mask >>= 1;
            }
            //fputs(_Buff, console);
        }
    }

    inline void Add(UnsignedIntImpl& _Answr, const UnsignedIntImpl& _First, const UnsignedIntImpl& _Secnd)noexcept {
        _Mtype* _This = 0;
        _Stype  _Siza = 0;
        _Mtype* _That = 0;
        _Stype  _Sizb = 0;

        if (_First._Siz >= _Secnd._Siz) {
            _This = _First._Num;
            _That = _Secnd._Num;
            _Siza = _First._Siz;
            _Sizb = _Secnd._Siz;
        }
        else {
            _That = _First._Num;
            _This = _Secnd._Num;
            _Sizb = _First._Siz;
            _Siza = _Secnd._Siz;
        }

        auto _Cache = _zero;
        auto _Cary1 = false;
        auto _Cary2 = false;

        if (_Siza >= _Answr._Cap) {
            free(_Answr._Num);
            _Answr._Alloc(_Siza + 1);
        } _Answr._Siz = _Siza;

        for (_Stype i = 0; i < _Sizb; ++i) {
            _Cache = _This[i] + _That[i];
            _Cary2 = _Cache == _Tmax && _Cary1 ? true : _Cache < _This[i];
            _Answr._Num[i] = _Cache + _Cary1;
            _Cary1 = _Cary2;
        }
        for (auto i = _Sizb; i < _Siza && _Cary1; ++i) {
            _Cary2 = _This[i] == _Tmax;
            _Answr._Num[i] = _This[i] + _Cary1;
            _Cary1 = _Cary2;
        }
        if (_Cary1) {
            _Answr._Num[_Answr._Siz] = 1; ++_Answr._Siz;
        }
    }

#if false
    void mul(Const dynamic_unsigned_int& _Answr,
        const dynamic_unsigned_int& _First,
        const dynamic_unsigned_int& _Secnd)noexcept
    {
        _Answr = _First._Siz + _Secnd._Siz;

        dynamic_unsigned_int::_Mtype i, j, _Slot, _Cche;
        auto _Cary = dynamic_unsigned_int::_zero;
        auto _Cry1 = false;
        auto _Cry2 = false;
        __uint128_t _Blck;
        auto _Vlue = _Secnd._Num[0];

        std::memset(_Answr._Num + _First._Siz, 0, _Secnd._Siz << 3);

        for (i = 0; i < _First._Siz; ++i) {
            _Blck = _First._Num[i];
            _Blck = _Blck * _Vlue + _Cary;
            _Cary = _Blck >> 64;
            _Answr._Num[i] = _Blck & dynamic_unsigned_int::_Tmax;
        }   _Answr._Num[i] = _Cary;

        for (i = 1; i < _Secnd._Siz; ++i) {
            _Vlue = _Secnd._Num[i];
            if (_Vlue != 0) {
                _Cary = 0; _Answr._Siz = i; _Cry1 = 0;
                for (j = 0; j < _First._Siz; ++j) {
                    _Blck = _First._Num[j];
                    _Blck = _Blck * _Vlue + _Cary;
                    _Cary = _Blck >> 64;
                    _Slot = _Blck & dynamic_unsigned_int::_Tmax;
                    _Cche = _Answr._Num[_Answr._Siz] + _Slot;
                    _Cry2 = _Cche == dynamic_unsigned_int::_Tmax && _Cry1 ? true : _Cche < _Answr._Num[_Answr._Siz];
                    _Answr._Num[_Answr._Siz] = _Cche + _Cry1;
                    _Cry1 = _Cry2; ++_Answr._Siz;
                }
                if (_Cary != 0 && _Cry1) {
                    _Answr._Num[_Answr._Siz] = _Cary + _Cry1; ++_Answr._Siz;
                    if (_Cary == dynamic_unsigned_int::_Tmax) {
                        _Answr._Num[_Answr._Siz] = 1; ++_Answr._Siz;
                    }
                }
                else if (_Cary) {
                    _Answr._Num[_Answr._Siz] = _Cary; ++_Answr._Siz;
                }
                else if (_Cry1) {
                    _Answr._Num[_Answr._Siz] = 1; ++_Answr._Siz;
                }
            }
        }
    }
#else 
    /*inline void mul(dynamic_unsigned_int& _Answr, const dynamic_unsigned_int& _First, const dynamic_unsigned_int& _Secnd)noexcept {
        _Answr = _First._Siz + _Secnd._Siz;

        auto _Tspt = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_First._Num);
        auto _Ttpt = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_Secnd._Num);
        auto _A = reinterpret_cast<dynamic_unsigned_int::_Htype*>(_Answr._Num);

        auto _Tssz = _First._Siz << 1; _Tssz -= (_Tspt[_Tssz - 1] == 0);
        auto _Ttsz = _Secnd._Siz << 1; _Ttsz -= (_Ttpt[_Ttsz - 1] == 0);

        dynamic_unsigned_int::_Htype _Slot, _Cche, _Vlue = _Ttpt[0], _Cary = 0U;
        dynamic_unsigned_int::_Mtype _Blck;
        dynamic_unsigned_int::_Stype i, j, I = 0;

        auto _Cry1 = false;
        auto _Cry2 = false;

        iq::memset(_A + _Tssz, 0, (_Ttsz + 1) << _hshft);

        for (i = 0; i < _Tssz; ++i) {
            _Blck = _Tspt[i];
            _Blck = _Blck * _Vlue + _Cary;
            _Cary = _Blck >> _hbits;
            _A[i] = _Blck & _Hmax;
        }   _A[i] = _Cary;

        for (i = 1; i < _Ttsz; ++i) {
            _Vlue = _Ttpt[i];
            if (_Vlue != 0) {
                _Cary = 0; I = i; _Cry1 = 0;
                for (j = 0; j < _Tssz; ++j) {
                    _Blck = _Tspt[j];
                    _Blck = _Blck * _Vlue + _Cary;
                    _Cary = _Blck >> _hbits;
                    _Slot = _Blck & _Hmax;

                    _Cche = _A[I] + _Slot;
                    _Cry2 = _Cche == _Hmax && _Cry1 ? true : _Cche < _A[I];
                    _A[I] = _Cche + _Cry1;
                    _Cry1 = _Cry2; ++I;
                }
                if (_Cary != 0 && _Cry1) {
                    _A[I] = _Cary + _Cry1; ++I;
                    if (_Cary == _Hmax) {
                        _A[I] = 1; ++I;
                    }
                }
                else if (_Cary) {
                    _A[I] = _Cary; ++I;
                }
                else if (_Cry1) {
                    _A[I] = 1; ++I;
                }
            }
        }
        _Answr._Siz = I & 1 ? (++I) >> 1 : I >> 1;
    }*/
#endif
    /* inline unsigned int* _Addition(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2, unsigned int& _Siz) {
         auto _Num = (unsigned int*)std::malloc((size_t)(_Siz1 + 1) << 2); _Num[_Siz1] = 0;
         auto _Cary1 = false;
         auto _Cary2 = false;
         auto _Cache = 0U;
         for (auto i = 0U; i < _Siz2; ++i) {
             _Cache = _Num1[i] + _Num2[i];
             _Cary2 = _Cache == UINT32_MAX && _Cary1 ? true : _Cache < _Num1[i];
             _Num[i] = _Cache + _Cary1;
             _Cary1 = _Cary2;
         }
         for (auto i = _Siz2; i < _Siz1 && _Cary1; ++i) {
             _Cary2 = _Num1[i] == UINT32_MAX;
             _Num[i] = _Num1[i] + _Cary1;
             _Cary1 = _Cary2;
         }
         _Siz = _Siz1;
         if (_Cary1) {
             _Num[_Siz] = 1; ++_Siz;
         } return _Num;
     }

     void _Print(unsigned int* _Num, unsigned int _Siz)noexcept {
         char _Buff[33]{}; _Buff[32] = '\0';
         auto _Mask = 0ULL;
         for (int i = _Siz - 1; i >= 0; --i) {
             _Mask = 0b10000000000000000000000000000000;
             for (auto j = 0; j < 32; ++j) {
                 _Buff[j] = (_Num[i] & _Mask) ? '1' : '0';
                 _Mask >>= 1;
             }
             std::fputs(_Buff, stdout);
         }

     }

     dynamic_unsigned_int _Karatsuba2(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2);

     dynamic_unsigned_int _Karatsuba1(unsigned int* _Num1, unsigned int* _Num2, unsigned int _Idx) {

         if (_Idx == 1) return { ((size_t)(*_Num1)) * (*_Num2) };

         auto _Idx1 = _Idx / 2;
         auto _Idx2 = _Idx - _Idx1;

         dynamic_unsigned_int _BD = _Karatsuba1(_Num1, _Num2, _Idx1);
         dynamic_unsigned_int _AC = _Karatsuba1(_Num1 + _Idx1, _Num2 + _Idx1, _Idx2);

         unsigned int _A_PLUS_B_SIZ; auto _A_PLUS_B = _Addition(_Num1 + _Idx1, _Idx2, _Num1, _Idx1, _A_PLUS_B_SIZ);
         unsigned int _C_PLUS_D_SIZ; auto _C_PLUS_D = _Addition(_Num2 + _Idx1, _Idx2, _Num2, _Idx1, _C_PLUS_D_SIZ);

         dynamic_unsigned_int _AD_PLUS_BC = _Karatsuba2(_A_PLUS_B, _A_PLUS_B_SIZ, _C_PLUS_D, _C_PLUS_D_SIZ);

         std::free(_A_PLUS_B);
         std::free(_C_PLUS_D);

         _AD_PLUS_BC -= _AC;
         _AD_PLUS_BC -= _BD;
         _AD_PLUS_BC <<= _Idx1 << 5;

         _AC <<= _Idx1 << 6;

         _AC += _AD_PLUS_BC;
         _AC += _BD;

         return _AC;

     }

     dynamic_unsigned_int _Karatsuba2(unsigned int* _Num1, unsigned int _Siz1, unsigned int* _Num2, unsigned int _Siz2) {
         if (_Siz2 < _Siz1) {
             for (; _Siz2 < _Siz1; ++_Siz2)
                 _Num2[_Siz2] = 0;
         } else if (_Siz1 < _Siz2) {
             for (; _Siz1 < _Siz2; ++_Siz1)
                 _Num1[_Siz1] = 0;
         }
         auto _Idx = _Siz1;
         auto _Idx1 = _Idx / 2;
         auto _Idx2 = _Idx - _Idx1;

         if (_Idx == 1) return { ((size_t)(*_Num1)) * (*_Num2) };

         dynamic_unsigned_int _BD = _Karatsuba1(_Num1, _Num2, _Idx1);
         dynamic_unsigned_int _AC = _Karatsuba1(_Num1 + _Idx1, _Num2 + _Idx1, _Idx2);

         unsigned int _A_PLUS_B_SIZ;
         unsigned int _C_PLUS_D_SIZ;

         auto _A_PLUS_B = _Addition(_Num1 + _Idx1, _Idx2, _Num1, _Idx1, _A_PLUS_B_SIZ);
         auto _C_PLUS_D = _Addition(_Num2 + _Idx1, _Idx2, _Num2, _Idx1, _C_PLUS_D_SIZ);

         dynamic_unsigned_int _AD_PLUS_BC = _Karatsuba2(_A_PLUS_B, _A_PLUS_B_SIZ, _C_PLUS_D, _C_PLUS_D_SIZ);

         std::free(_A_PLUS_B);
         std::free(_C_PLUS_D);

         _AD_PLUS_BC -= _AC;
         _AD_PLUS_BC -= _BD;
         _AD_PLUS_BC <<= _Idx1 << 5;

         _AC <<= _Idx1 << 6;

         _AC += _AD_PLUS_BC;
         _AC += _BD;

         return _AC;

     }

     dynamic_unsigned_int karatsuba(dynamic_unsigned_int& _This, dynamic_unsigned_int& _That)noexcept {

         auto _Size = dynamic_unsigned_int::_Fndsiz(_This._Siz);

         std::memset(_This._Num + _This._Siz, 0, (size_t)(_Size - _This._Siz) << 3);
         std::memset(_That._Num + _That._Siz, 0, (size_t)(_Size - _That._Siz) << 3);

         auto _Num1 = reinterpret_cast<unsigned int*>(_This._Num);
         auto _Num2 = reinterpret_cast<unsigned int*>(_That._Num);
         _Size <<= 1;

         dynamic_unsigned_int _Res = _Karatsuba1(_Num1, _Num2, _Size);

         for (; _Res._Num[_Res._Siz - 1] == 0 && _Res._Siz > 0; --_Res._Siz);



         return _Res;

     }*/
};
}
