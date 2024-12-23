#pragma once
#ifndef __RATIONAL_NUMBER__
#define __RATIONAL_NUMBER__

#include "../Io/io.h"

namespace alpha {

	template<class _Int> 
	class Rational {
	public:
		constexpr Rational()noexcept 
			:_Numerator(_Unit), _Denominator(_Unit) {}
		constexpr Rational(const _Int& _Arg)noexcept : 
			_Numerator(_Arg), _Denominator(_Unit) {}
		constexpr Rational(const _Int& _Arg1, const _Int& _Arg2)noexcept {
			_Initialize(_Arg1, _Arg2);
		}
		constexpr void print()const {
			_Denominator == _Unit ? alpha::print(_Numerator) : alpha::print('(', _Numerator, '/', _Denominator, ')');
		}
		template<class _RealNumber>
		constexpr operator _RealNumber() const noexcept {
			return static_cast<_RealNumber>(_Numerator) / static_cast<_RealNumber>(_Denominator);
		}
		constexpr bool operator==(const Rational& _That)const {
			return _Numerator == _That._Numerator && _Denominator == _That._Denominator;
		}
		constexpr bool operator!=(const Rational& _That)const {
			return !(this->operator==(_That));
		}
		constexpr bool operator>(const Rational& _That)const {
			return _Numerator * _That._Denominator > _That._Numerator * _Denominator;
		}
		constexpr bool operator<(const Rational& x)const {
			return _Numerator * x._Denominator < x._Numerator* _Denominator;
		}
		constexpr bool operator>=(const Rational& x)const {
			return _Numerator * x._Denominator >= x._Numerator * _Denominator;
		}
		constexpr bool operator<=(const Rational& x)const {
			return _Numerator * x._Denominator <= x._Numerator * _Denominator; 
		}

		constexpr Rational operator-()const {
			Rational _Tmp; 
			_Tmp._Numerator = -_Numerator;
			_Tmp._Denominator = _Denominator;
			return _Tmp;
		}
		constexpr Rational operator+(const Rational& x)const {
			return Rational(_Numerator * x._Denominator + x._Numerator * _Denominator, x._Denominator * _Denominator);
		}
		constexpr Rational operator-(const Rational& x)const { 
			return Rational(_Numerator * x._Denominator - x._Numerator * _Denominator, x._Denominator * _Denominator);
		}
		constexpr Rational operator*(const Rational& x)const {
			return Rational(_Numerator * x._Numerator, _Denominator * x._Denominator);
		}
		constexpr Rational operator/(const Rational& x)const {
			return Rational(_Numerator * x._Denominator, _Denominator * x._Numerator);
		}

		constexpr Rational& operator+=(const Rational& x) {
			_Initialize((_Numerator * x._Denominator + x._Numerator * _Denominator), (x._Denominator * _Denominator));
			return *this;
		}
		constexpr Rational& operator -= (const Rational& x) {
			_Initialize((_Numerator * x._Denominator - x._Numerator * _Denominator), (x._Denominator * _Denominator));
			return *this;
		}
		constexpr Rational& operator *= (const Rational& x) {
			_Initialize((_Numerator * x._Numerator), (_Denominator * x._Denominator));
			return *this;
		}
		constexpr Rational& operator /= (const Rational& x) {
			_Initialize(_Numerator * x._Denominator, _Denominator * x._Numerator);
			return *this;
		}

	private:
		_Int _Numerator, _Denominator;
		inline const static _Int _Zero = 0;
		inline const static _Int _Unit = 1;
		inline constexpr _Int _Gcd(_Int _A, _Int _B)noexcept {
			_Int _R = _A % _B;
			while (_R != _Zero) {
				_A = _B;
				_B = _R;
				_R = _A % _B;
			}
			return _B;
		}

		inline constexpr auto _Initialize(const _Int& _Arg1, const _Int& _Arg2)noexcept {
			const _Int _D = _Gcd(_Arg1, _Arg2);
			_Numerator = _Arg1 / _D;
			_Denominator = _Arg2 / _D;
			if (_Denominator < 0) {
				_Numerator = -_Numerator;
				_Denominator = -_Denominator;
			}
		}

	};

	template<typename _Ty>
	constexpr  void _print(const Rational<_Ty>& _That) { 
		_That.print();
	}
}

#endif