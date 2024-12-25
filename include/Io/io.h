#pragma once
#ifndef __IO__
#define __IO__
#include <cstdio>
#include "../Core/utility.h"

namespace alpha{

    unsigned short len(const unsigned int _Val)noexcept;
    unsigned short Len(const unsigned long long _Val)noexcept;

    void itos(int _Val, char* _Buffer)noexcept;
    void ltos(long long _Val, char* _Buffer)noexcept;
    void Itos(unsigned int _Val, char* _Buffer)noexcept;
    void Ltos(unsigned long long _Val, char* _Buffer)noexcept;

    int stoi(const char* _Str)noexcept;
    long long stol(const char* _Str)noexcept;
    unsigned int stoI(const char* _Str)noexcept;
    unsigned long long stoL(const char* _Str)noexcept;

    int _print(const unsigned char _Char, FILE* File = stdout)noexcept;
    int _print(const unsigned int _Char, FILE* File = stdout)noexcept;
    //int _print(const unsigned long _var, FILE* File = stdout)noexcept;
    int _print(const unsigned long long _var, FILE* File = stdout)noexcept;
    int _print(const char  _Cha, FILE* file = stdout)noexcept;
    int _print(const bool _var, FILE* file = stdout)noexcept;
    int _print(const char* _Str, FILE* file = stdout)noexcept;
    int _print(const int _var, FILE* file = stdout)noexcept;
    int _print(const long _var, FILE* file = stdout)noexcept;
    int _print(const float _var, FILE* file = stdout)noexcept;
    int _print(const long long _var, FILE* file = stdout)noexcept;
    int _print(const double _var, FILE* file = stdout)noexcept;
    int _print(const long double _var, FILE* file = stdout)noexcept;

    size_t _print(const void* _Str, size_t _ElementSize, size_t _ElementCount, FILE* file = stdout)noexcept;

    int _input(unsigned char& _Char, FILE* file = stdout)noexcept;
    int _input(unsigned int& _Char, FILE* file = stdout)noexcept;
    int _input(unsigned long& _var, FILE* file = stdout)noexcept;
    int _input(unsigned long long& _var, FILE* file = stdout)noexcept;
    int _input(char&  _Cha, FILE* file = stdout)noexcept;
    int _input(bool& _var, FILE* file = stdout)noexcept;
    char* _input(char* _Str, int count, FILE* file = stdout)noexcept;
    int _input(int& _var, FILE* file = stdout)noexcept;
    int _input(long& _var, FILE* file = stdout)noexcept;
    int _input(float& _var, FILE* file = stdout)noexcept;
    int _input(long long& _var, FILE* file = stdout)noexcept;
    int _input(double& _var, FILE* file = stdout)noexcept;
    int _input(long double& _var, FILE* file = stdout)noexcept;

    template<class _Ty> 
    constexpr void _print(const _Ty& _Obj)noexcept {
        _print("\nYou should define void _print(const ");
        _print(type_name<_Ty>);
        if constexpr (is_fundamental_v<_Ty>) _print(" _var) function in your code\n\n");
        else _print("& _Obj) function in your code\n\n");
        exit(1);
    }
    template<class _Ty> 
    void _input(_Ty& _Obj) noexcept {
        _print("\nYou should define void _input(");
        _print(type_name<_Ty>);
        _print("& _Obj) function in your code\n\n");
        exit(1);
    }


    template<class _Ty, class... _Ts>
    constexpr void print(const _Ty& _Val, const _Ts&... _Args)noexcept {
        _print(_Val); if constexpr (sizeof...(_Args) > 0) print(_Args...);
    }
    template<class _Ty, class... _Ts>
    void input(_Ty& _Val, _Ts&... _Args)noexcept {
        _input(_Val); if constexpr (sizeof...(_Args) > 0) input(_Args...);
    }
}

#endif