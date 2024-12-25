#pragma once
#ifndef __COMPLEX__
#define __COMPLEX__

#include "../Core/core.h"
#include "../Io/io.h"
#include "../Core/utility.h"

namespace alpha {
	
	template<class _Ty>
	class Complex {
	public:
		inline constexpr Complex() :_ReZ(_Zero), _ImZ(_Zero) {}
		inline constexpr Complex(const _Ty& x) : _ReZ(x), _ImZ(_Zero) {}
		inline constexpr Complex(const _Ty& x, const _Ty& y) : _ReZ(x), _ImZ(y) {}
		inline constexpr Complex(const Complex<_Ty>& _That)noexcept {
			_ReZ = _That._ReZ;
			_ImZ = _That._ImZ;
		}
		template<typename U = _Ty,typename enable_if_t<!is_arithmetic_v<U>, int> = 0>
		inline constexpr Complex(Complex<_Ty>&& _That)noexcept {
			_ReZ = move(_That._ReZ);
			_ImZ = move(_That._ImZ);
		}
		inline constexpr void conjugates()const noexcept {
			if constexpr (is_reference_v<decltype(-declval<_Ty>())>){
				-_ImZ;
			} else {
				_ImZ = -_ImZ;
			}
		}
		
		inline constexpr Complex<_Ty>& operator-()const noexcept {
			if constexpr (is_reference_v<decltype(-declval<_Ty>())>) {
				-_ReZ;
				-_ImZ;
			} else {
				_ReZ = -_ReZ;
				_ImZ = -_ImZ;
			}
			return *this;
		}
		inline constexpr Complex<_Ty>& operator+()const noexcept {}

		inline constexpr bool operator == (const _Ty& x)const noexcept {
			return _ReZ == x && _ImZ == _Zero;
		}
		inline constexpr bool operator != (const _Ty& x)const noexcept {
			return _ReZ != x || _ImZ != _Zero;
		}

		inline constexpr Complex<_Ty>& operator+=(const _Ty& x) {
			_ReZ += x;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator+ (const _Ty& x)const {
			auto _Tmp = *this;
			_Tmp += x;
			return _Tmp;
		}
		inline constexpr Complex<_Ty>& operator-=(const _Ty& x) {
			_ReZ -= x;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator- (const _Ty& x)const {
			auto _Tmp = *this;
			_Tmp -= x;
			return _Tmp;
		}
		inline constexpr Complex<_Ty>& operator*=(const _Ty& x) {
			_ReZ *= x;
			_ImZ *= x;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator* (const _Ty& x)const {
			auto _Tmp = *this;
			_Tmp *= x;
			return _Tmp;
		}
		inline constexpr Complex<_Ty>& operator/=(const _Ty& x) {
			_ReZ /= x;
			_ImZ /= x;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator/ (const _Ty& x)const {
			auto _Tmp = *this;
			_Tmp /= x;
			return _Tmp;
		}

		inline constexpr bool operator == (const Complex& z)const noexcept {
			return _ReZ == z._ReZ && _ImZ == z._ImZ;
		}
		inline constexpr bool operator != (const Complex& z)const noexcept {
			return _ReZ != z._ReZ || _ImZ != z._ImZ;
		}

		inline constexpr Complex<_Ty>& operator+=(const Complex& z) {
			_ReZ += z._ReZ;
			_ImZ += z._ImZ;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator+ (const Complex& z)const {
			auto _Tmp = *this;
			_Tmp += z;
			return _Tmp;
		}
		inline constexpr Complex<_Ty>& operator-=(const Complex& z) {
			_ReZ -= z._ReZ;
			_ImZ -= z._ImZ;
			return *this;
		}
		inline constexpr Complex<_Ty>  operator- (const Complex& z)const {
			auto _Tmp = *this;
			_Tmp -= z;
			return _Tmp;
		}
		inline constexpr Complex<_Ty>& operator*=(const Complex& z)noexcept {
			/*
			* **************Karathsuba complex multiplication****************
			* s1 = ac, s2 = bd, s3 = (a+b)(c+d), Real = s1 - s2, Imag = s3 - s1 - s2
			* ***************Guass Complex Multiplication********************
			* k1 = c(a+b), k2 = a(d-c), k3 = b(c+d), Real = k1 - k3, Imag = k1 + k2
			* ****************FFT for complex multiplication****************
			* x = a(c-d), y = (a+b), z = (a-b), Real = zd + x, Imag = yc - x
			*/
			if constexpr (is_arithmetic<_Ty>::value) {
				// Using normal complex multiplication with 4 real multiplication and 2 real addition
				auto _Tmp = _ReZ * z._ReZ - _ImZ * z._ImZ;
				_ImZ = _ReZ * z._ImZ + _ImZ * z._ReZ;
				_ReZ = _Tmp;
			} else {
				// Using Guass Complex Multiplication with 3 real multiplication and 5 real addition
				auto K1 = _ReZ + _ImZ;
				K1 *= z._ReZ; // K1 = (a+b)c
				auto K3 = z._ReZ + z._ImZ;
				K3 *= _ImZ; // K3 = (c+d)*b
				_ImZ = z._ImZ;
				_ImZ -= z._ReZ;
				_ImZ *= _ReZ; // K2 = (d-c)a
				_ImZ += K1; // _ImZ = K2 + K1;
				K1 -= K3;
				_ReZ = move(K1);
			}
			return *this;
		} 
		inline constexpr Complex<_Ty>  operator* (const Complex& z)const {
			if constexpr (is_arithmetic<_Ty>::value) {
				// Using normal complex multiplication with 4 multiplication and 2 addition
				return Complex(_ReZ * z._ReZ - _ImZ * z._ImZ, _ReZ * z._ImZ + _ImZ * z._ReZ);
			} else {
				// Using Guass Complex Multiplication with 3 multiplication and 5 addition
				//Complex<_Ty> _Tmp(_ReZ + _ImZ, z._ReZ + z._ImZ);
				//_Tmp._ReZ *= z._ReZ; // K1 = (a+b)c
				//_Tmp._ImZ *= _ImZ; // K3 = (c+d)*b
				//_ImZ = z._ImZ;
				//_ImZ -= z._ReZ;
				//_ImZ *= _ReZ; // K2 = (d-c)a
				//_ImZ += K1; // _ImZ = K2 + K1;
				//K1 -= K3;
				//_ReZ = move(K1);
			}
			return *this;
		}
		inline constexpr Complex<_Ty>& operator/=(const Complex& z) {

			
			return *this;
		}
		inline constexpr Complex<_Ty>  operator/ (const Complex& z)const noexcept {

			
			return *this;
		}

		template<class _Iterator>
		constexpr void _MultoContainer(_Iterator _First, _Iterator _Last)const noexcept {
			// FFT complex multiplication with 3 real multiplication and 3 real addtition
			// X = b(c-d), C1 = (a+b), C2 = (a-b), Real = c*c2 + x, Imag = d*c1 + x
			auto _UFirst = _Get_unwrapped(_First);
			const auto _ULast = _Get_unwrapped(_Last);
			const auto _c1 = _ReZ + _ImZ;
			const auto _c2 = _ReZ - _ImZ;
			_Ty _X;
			for (; _UFirst != _ULast; ++_UFirst) {
				_X = (*_UFirst)._ReZ;
				_X -= (*_UFirst)._ImZ;
				_X *= _ImZ;

				(*_UFirst)._ReZ *= _c2;
				(*_UFirst)._ReZ += _X;

				(*_UFirst)._ImZ *= _c1;
				(*_UFirst)._ImZ += _X;
			}
		}
		template<class _Iterator, class U = _Ty, typename enable_if_t<!is_arithmetic_v<U>, int> = 0>
		constexpr Complex<_Ty> _EvalutePolynomial(_Iterator _First, _Iterator _Last)const noexcept {

			// Evaluting polynomial using horner's method and FFT complex multiplication
			// X = b(c-d), C1 = (a+b), C2 = (a-b), Real = c*c2 + x, Imag = d*c1 + x

				  auto _ULast  = _Get_unwrapped(_Last) - 1;
			const auto _UFirst = _Get_unwrapped(_First) - 1;

			Complex<_Ty> _Result = *_ULast;
			--_ULast;
			if (this->_ReZ != _Zero) {
				if (this->_ImZ != _Zero) {
					_Ty _X;
					const _Ty _c1 = _ReZ + _ImZ;
					const _Ty _c2 = _ReZ - _ImZ;

					for (; _ULast != _UFirst; --_ULast) {
						///////////////////////////////////////
						_X = _Result._ReZ;				///
						_X -= _Result._ImZ;				///
						_X *= _ImZ;						///
														///
						_Result._ReZ *= _c2;			///||||-> _Result *= *this; (FFT complex multiplication)
						_Result._ReZ += _X;				///		  _Result += *_ULast;
						_Result._ImZ *= _c1;			///
						_Result._ImZ += _X;				///
						///////////////////////////////////////
						_Result *= *_ULast;
					} return _Result;
				} else {
					for (; _ULast != _UFirst; --_ULast) {
						_Result *= this->_ReZ;
						_Result += *_ULast;
					} return _Result;
				}
			} else {
				if (this->_ImZ != _Zero) {
					__debugbreak(); // write code
					return Complex();
				} else return *(_UFirst + 1);
			}
		}
		template<class _Iterator>
		constexpr void _DivtoContainer(_Iterator _First, _Iterator _Last)const noexcept {
			auto _UFirst = _Get_unwrapped(_First);
			const auto _ULast = _Get_unwrapped(_Last);

			_Ty _CsqPlusDsq = _ReZ * _ReZ;
			_Ty _X = _ImZ * _ImZ;
			_CsqPlusDsq += _X;
			
			this->conjugate();
			const _Ty _c1 = _ReZ + _ImZ;
			const _Ty _c2 = _ReZ - _ImZ;
			
			for (; _UFirst != _ULast; ++_UFirst) {
				_X = (*_UFirst)._ReZ;
				_X -= (*_UFirst)._ImZ;
				_X *= _ImZ;

				(*_UFirst)._ReZ *= _c2;
				(*_UFirst)._ReZ += _X;
				(*_UFirst)._ReZ /= _CsqPlusDsq;

				(*_UFirst)._ImZ *= _c1;
				(*_UFirst)._ImZ += _X;
				(*_UFirst)._ImZ /= _CsqPlusDsq;
			}
			this->conjugate();
		}
		constexpr _Ty real()const {
			return _ReZ;
		}
		constexpr void real(const _Ty& _Val)noexcept {
			_ReZ = _Val;
		}
		constexpr _Ty imag()const {
			return _ImZ;
		}
		constexpr void imag(const _Ty& _Val)noexcept {
			_ImZ = _Val;
		}
		/*constexpr _Ty modi()const {
			return std::sqrt(_ReZ * _ReZ + _ImZ * _ImZ);
		}*/
		/*constexpr Point<_Ty> steriographicProj()const {
			_Ty Tmp = ReZ * ReZ + ImZ * ImZ + identity;
			return { _Ty(2) * ReZ / Tmp, _Ty(2) * ImZ / Tmp, (ReZ * ReZ + ImZ * ImZ - identity) / Tmp };
		}*/
		inline constexpr void print()const {
			if (_ImZ == _Zero || _ImZ == _Unity || _ImZ == _nUnity || _ReZ == _Zero) {
				if (_ReZ == _Zero) {
					if (_ImZ == _Zero) alpha::print(_Zero);
					else if (_ImZ == _Unity) alpha::print('i');
					else if (_ImZ == _nUnity) alpha::print("- i");
					else alpha::print(_ImZ, 'i');
				} else {
					alpha::print(_ReZ);
					if (_ImZ == _Unity) alpha::print("+i");
					else if (_ImZ == _nUnity) alpha::print("-i");
					else if (_ImZ != _Zero) {
						if constexpr (alpha::_Orderable_v<_Ty>) {
							if (_ImZ > _Zero) alpha::print('+', _ImZ, 'i');
							else alpha::print(_ImZ, 'i');
						} else {
							alpha::print('+', _ImZ, 'i');
						}
					}
				}
			} else {
				if constexpr (alpha::_Orderable_v<_Ty>) {
					if (_ImZ < _Zero) alpha::print(_ReZ, _ImZ, 'i');
					else alpha::print(_ReZ, '+', _ImZ, 'i');
				} else alpha::print(_ReZ, '+', _ImZ, 'i');
			}
		}
		inline constexpr void input() {
			alpha::input(_ReZ, _ImZ);
		}
	private:
		inline static const _Ty _Zero = _Ty(0);
		inline static const _Ty _Unity = _Ty(1);
		inline static const _Ty _nUnity = _Ty(-1);
		_Ty _ReZ;
		mutable _Ty _ImZ;
	};

	template<class T>
	constexpr Complex<T> operator + (const T& Left, const Complex<T>& Right) {
		return Right + Left;
	}
	template<class T>
	constexpr Complex<T> operator - (const T& Left, const Complex<T>& Right) {
		return -Right + Left;
	}
	template<class T>
	constexpr Complex<T> operator * (const T& Left, const Complex<T>& Right) {
		return Right * Left;
	}
	template<class T>
	constexpr Complex<T> operator / (const T& Left, const Complex<T>& Right) {
		Complex<T> Tmp(Left);
		Tmp /= Right;
		return Tmp;
	}
	template<class T>
	constexpr T Re(const Complex<T>& z) {
		return z.real();
	}
	template<class T>
	constexpr T Im(const Complex<T>& z) {
		return z.imag();
	}
	template<class T>
	constexpr Complex<T> conjugates(Complex<T> z) { z.conjugates(); return z; }
	template<class _Ty>
	void _print(const Complex<_Ty>& z) {
		z.print();
	}
	template<class _Ty>
	void _input(const Complex<_Ty>& z) {
		z.input();
	}
}

#endif