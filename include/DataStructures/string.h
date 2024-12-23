#pragma once
#ifndef __STRING__
#define __STRING__


#include "Array.h"


namespace alpha {
    struct str : public Array<char> {
        using Array<char>::Array;
        using Array<char>::operator=;
        str()noexcept : Array<char>() {}
        str(const char _Char) noexcept : Array<char>(1) {
            *_Ptr = _Char;
        }

        str(const char* _That) noexcept : Array<char>(strlen(_That)) {
            _ConstructCopy(_Ptr, _That, _Siz);
        }

        str& operator=(const char _Char) noexcept {
            _Siz = 1;
            if (1 > _Cap) 
                _ReallocateNoCopy(_Siz);
            *_Ptr = _Char;
            return *this;
        }

        str& operator=(const char* _That) noexcept{
            _Siz = strlen(_That);
            if (_Siz > _Cap)
                _ReallocateNoCopy(_Siz);
            _ConstructCopy(_Ptr, _That, _Siz);
            return *this;
        }

        str& operator+=(const char* _That) noexcept {
            const SizeType _StrSiz = strlen(_That);
            const SizeType _NewSiz = _Siz + _StrSiz;
            if (_NewSiz > _Cap)
                _Reallocate(_NewSiz);
            _ConstructCopy(_Ptr + _Siz, _That, _StrSiz);
            return *this; 
        }

        str operator+(const char* _That)const noexcept {
            const SizeType _StrSiz = strlen(_That);
            const SizeType _NewSiz = _Siz + _StrSiz;
            str _Tmp; _Tmp._Allocate(_NewSiz);
            _ConstructCopy(_Tmp._Ptr, _Ptr, _Siz);
            _ConstructCopy(_Tmp._Ptr + _Siz, _That, _StrSiz);
        }

        //    str& assign(const size_t _N, const char _C)noexcept;

        //    str& assign(const char* _Strng, const size_t _Len)noexcept;

        //    str& assign(const str& _That, const size_t _Pos, const size_t _Len)noexcept;


        //    str& insert(const size_t _Pos, const char* _That)noexcept;

        //    str& insert(const size_t _Pos, const char* _That, const size_t _Len)noexcept;

        //    void input()noexcept;
        //    const char* c_str()const noexcept;
        //    */
        //
    };

    //void _print(const str& String) {
    //    _print(String.data(), 1, String.size());
    //}

}

    

    //void fun() {
    //    str s;
    //    str ss("Hello world");

    //}

    //void _print(const str& _That)noexcept;
    //void _input(str& _That)noexcept;

    /*inline str _sprint(const char _Cha)noexcept {
        return str(_Cha);
    }
    inline str _sprint(const char* _Str)noexcept {
        return str(_Str);
    }*/

    /*   inline int _sprint(const unsigned char _Char)noexcept;
       inline int _sprint(const unsigned int _Char, FILE* File = stdout)noexcept;
       inline int _sprint(const unsigned long _var, FILE* File = stdout)noexcept;
       inline int _sprint(const unsigned long long _var, FILE* File = stdout)noexcept;
       inline int _sprint(const char  _Cha, FILE* file = stdout)noexcept;
       inline int _sprint(const bool _var, FILE* file = stdout)noexcept;
       inline int _sprint(const char* _Str, FILE* file = stdout)noexcept;
       inline int _sprint(const int _var, FILE* file = stdout)noexcept;
       inline int _sprint(const long _var, FILE* file = stdout)noexcept;
       inline int _sprint(const float _var, FILE* file = stdout)noexcept;
       inline int _sprint(const long long _var, FILE* file = stdout)noexcept;
       inline int _sprint(const double _var, FILE* file = stdout)noexcept;
       inline int _sprint(const long double _var, FILE* file = stdout)noexcept;


       template<class _Ty, class... _Ts>
       constexpr void sprint(string& _Str, const _Ty& _Val, const _Ts&... _Args)noexcept {
           _Str += _sprint(_Val);
           if constexpr (sizeof...(_Args) > 0) sprint(_Str, _Args...);
       }*/



#endif