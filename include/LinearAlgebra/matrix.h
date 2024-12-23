#pragma once
#ifndef __MATRIX__
#define __MATRIX__



#include "../DataStructures/algorithm.h"
#include "../DataStructures/simd.h"
#include "../Functions/polynomial.h"


#define __Matrix(_Pos) *(_Mat + _Pos)
#define x_matrix__(_Pos) *(x._Mat + _Pos)
#define Tmp_matrix__(_Pos) *(Tmp._Mat + _Pos)
#define Tmp_ptr_matrix__(_Pos) *(Tmp_ptr->_Mat + _Pos)

#define _Matrix(_Pos,_Cpivot) *(_Mat + _Pos * _Col + _Cpivot)
#define x_matrix_(_Pos,_Cpivot) *(x._Mat + _Pos * x._Col + _Cpivot)
#define Tmp_matrix_(_Pos,_Cpivot) *(Tmp._Mat + _Pos * Tmp._Col + _Cpivot)
#define Tmp_ptr_matrix_(_Pos,_Cpivot) *(Tmp_ptr->_Mat + _Pos * Tmp_ptr->_Col + _Cpivot)

namespace alpha {
	template <class _Ty>
	using list = Array<_Ty>;
	template<typename _Ty>
	class Matrix {
	private:
		_Ty* _Mat;
		size_t _Row, _Col;
		size_t _Siz;
		inline static const _Ty _Zero = 0;
		inline static const _Ty unity = 1;
		enum _ElementaryRowOperation : char {
			ROWSWITCH, ROWDIVISION, ROWMINUS
		};
		struct Method {
			_ElementaryRowOperation o;
			size_t _Arg1, _Arg2;
			_Ty Tmp;
		};
		
		constexpr void deepCopy(_Ty* a, _Ty* b, size_t range) {
			for (size_t i = 0; i < range; ++i)
				*(a + i) = *(b + i);
		}

		constexpr void reallocate(size_t size) {
			delete[]_Mat; _Siz = size;
			_Mat = new _Ty[_Siz];
		}

		Array<Array<size_t>> makeCombi(size_t n, size_t k)const {
			Array<Array<size_t>> ans;
			Array<size_t> tmp;
			makeCombiUtil(ans, tmp, n, 1, k);
			return ans;
		}

		void makeCombiUtil(Array<Array<size_t> >& ans, Array<size_t>& tmp, size_t n, size_t left, size_t _idx)const {
			if (_idx == 0) {
				ans.push_back(tmp);
				return;
			}
			for (size_t i = left; i <= n; ++i) {
				tmp.push_back(i);
				makeCombiUtil(ans, tmp, n, i + 1, _idx - 1);
				tmp.pop_back();
			}
		}

	public:
		constexpr Matrix(void) {
			_Mat = nullptr;
			_Siz = _Row = _Col = 0;
		}

		constexpr Matrix(const size_t x) {
			_Mat = new _Ty[_Siz = x * x];
			_Row = _Col = x;
		}

		constexpr Matrix(const size_t ROW, const size_t COLUMN) {
			_Mat = new _Ty[_Siz = ROW * COLUMN];
			_Row = ROW; _Col = COLUMN;
		}

		constexpr Matrix(const initializer<_Ty>& LIST) {
			_Row = LIST.size(); _Col = LIST.begin()->size();
			_Mat = new _Ty[_Siz = _Row * _Col]; size_t index = 0;
			for (const auto& i : LIST)
				for (const auto& j : i)
					_Mat[index++] = j;
		}

		constexpr Matrix(const Matrix& x) {
			_Row = x._Row; _Col = x._Col;
			_Mat = new _Ty[_Siz = _Row * _Col];
			deepCopy(_Mat, x._Mat, _Siz);
		}

		constexpr Matrix(Matrix&& x)noexcept {
			_Mat = x._Mat; _Row = x._Row;  _Col = x._Col; _Siz = x._Siz;
			x._Mat = nullptr; x._Row = 0; x._Col = 0; x._Siz = 0;
		}

		constexpr ~Matrix(void) {
			delete[]_Mat;
		}

		constexpr operator int(void) {
			return static_cast<int>(_Row * _Col);
		}

		constexpr operator size_t(void) {
			return static_cast<size_t>(_Row * _Col);
		}

		constexpr void operator = (const size_t x) {
			if constexpr (x * x > _Siz)
				reallocate(x * x);
			_Row = _Col = x;
		}

		constexpr void operator = (const initializer<_Ty>& LIST) {
			_Row = LIST.size(); _Col = LIST.begin()->size(); size_t index = 0;
			if constexpr (_Row * _Col > _Siz)
				reallocate(_Row * _Col);
			for (const auto& i : LIST)
				for (const auto& j : i)
					__Matrix(index++) = j;
		}

		constexpr void operator = (const Matrix& x) {
			if constexpr (this != &x) {
				_Row = x._Row; _Col = x._Col;
				if constexpr (_Row * _Col > _Siz)
					reallocate(_Row * _Col);
				deepCopy(_Mat, x._Mat, _Row * _Col);
			}
		}

		constexpr void operator = (Matrix&& x)noexcept {
			delete[]_Mat;
			_Mat = x._Mat; _Row = x._Row;  _Col = x._Col; _Siz = x._Siz;
			x._Mat = nullptr; x._Row = 0; x._Col = 0; x._Siz = 0;
		}

		constexpr void R_M(const size_t i, const _Ty& x) {
			if (i < _Row)
				for (size_t j = 0; j < _Col; ++j)
					_Matrix(i, j) *= x;
		}

		constexpr void R_D(const size_t i, const _Ty& x) {
			if (i < _Row)
				for (size_t j = 0; j < _Col; ++j)
					_Matrix(i, j) /= x;
		}

		constexpr void R_PLUS(const size_t i, const size_t j, const _Ty& x) {
			if (i < _Row && j < _Row && i != j)
				for (size_t k = 0; k < _Col; ++k)
					_Matrix(i, k) += (x * _Matrix(j, k));
		}

		constexpr void R_MINUS(const size_t i, const size_t j, const _Ty& x) {
			if (i < _Row && j < _Row && i != j)
				for (size_t k = 0; k < _Col; ++k)
					_Matrix(i, k) -= (x * _Matrix(j, k));
		}

		constexpr void R_SWITCH(const size_t i, const size_t j) {
			if (i < _Row && j < _Row && i != j) {
				_Ty Tmp;
				for (size_t k = 0; k < _Col; ++k) {
					Tmp = _Matrix(i, k);
					_Matrix(i, k) = _Matrix(j, k);
					_Matrix(j, k) = Tmp;
				}
			}
		}

		constexpr void C_M(const size_t j, const _Ty& x) {
			if constexpr (j < _Col)
				for (size_t i = 0; i < _Row; ++i)
					_Matrix(i, j) *= x;
		}

		constexpr void C_D(const size_t j, const _Ty& x) {
			if constexpr (j < _Col)
				for (size_t i = 0; i < _Row; ++i)
					_Matrix(i, j) /= x;
		}

		constexpr void C_PLUS(const size_t i, const size_t j, const _Ty& x) {
			if constexpr (i < _Col && j < _Col)
				for (size_t k = 0; k < _Row; ++k)
					_Matrix(k, i) += (x * _Matrix(k, j));
		}

		constexpr void C_MINUS(const size_t i, const size_t j, const _Ty& x) {
			if constexpr (i < _Col && j < _Col)
				for (size_t k = 0; k < _Row; ++k)
					_Matrix(k, i) -= (x * _Matrix(k, j));
		}

		constexpr void C_SWITCH(const size_t i, const size_t j) {
			if constexpr (i < _Col && j < _Col && i != j) {
				_Ty Tmp;
				for (size_t k = 0; k < _Row; ++k) {
					Tmp = _Matrix(k, i);
					_Matrix(k, i) = _Matrix(k, j);
					_Matrix(k, j) = Tmp;
				}
			}
		}

		[[nodiscard]] constexpr bool IS_ZERO_ROW(const size_t i)const {
			for (size_t j = 0; j < _Col; ++j)
				if constexpr (_Matrix(i, j) != _Zero)
					return false;
			return true;
		}

		[[nodiscard]] constexpr bool IS_ZERO_COLUMN(const size_t j)const {
			for (size_t i = 0; i < _Row; ++i)
				if constexpr (_Matrix(i, j) != _Zero)
					return false;
			return true;
		}

		[[nodiscard]] constexpr Matrix r_m(const size_t i, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.R_M(i, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix r_d(const size_t i, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.R_D(i, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix r_plus(const size_t i, const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.R_PLUS(i, j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix r_minus(const size_t i, const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.R_MINUS(i, j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix r_switch(const size_t i, const size_t j)const {
			Matrix Tmp(*this); Tmp.R_SWITCH(i, j);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix c_m(const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.C_M(j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix c_d(const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.C_D(j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix c_plus(const size_t i, const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.C_PLUS(i, j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix c_minus(const size_t i, const size_t j, const _Ty& x)const {
			Matrix Tmp(*this); Tmp.C_MINUS(i, j, x);
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix c_switch(const size_t i, const size_t j)const {
			Matrix Tmp(*this); Tmp.C_SWITCH(i, j);
			return Tmp;
		}

	private:
		constexpr void C_MAKE(const size_t i, const size_t j, bool x = false) {
			_Ty Tmp = _Matrix(i, j);
			if constexpr (Tmp != _Zero) {
				if constexpr (Tmp != unity) R_D(i, Tmp); size_t k;
				if (x == true) k = 0; else k = i + 1;
				for (k; k < _Row; ++k) {
					Tmp = _Matrix(k, j);
					if constexpr (Tmp != _Zero)
						R_MINUS(k, i, Tmp);
				}
			}
		}

	private:
		constexpr void C_MAKE_SPECIAL(const size_t i, const size_t j, bool x = false) {
			_Ty Tmp = _Matrix(i, j), Tmpx;
			if constexpr (Tmp != _Zero) {
				size_t k; if (x == true) k = 0; else k = i + 1;
				for (k; k < _Row; ++k) {
					Tmpx = _Matrix(k, j) / Tmp;
					if constexpr (Tmpx != _Zero)
						R_MINUS(k, i, Tmpx);
				}
			}
		}

	public:
		constexpr void UPPER_TRIANGULAR(void) {
			size_t i = 0, j = 0, Pivoti = 0, Pivotj = 0, RANGE;
			if constexpr (_Row <= _Col) RANGE = _Row; else RANGE = _Col;
			for (j = 0; j < RANGE; ++j) {
				i = Pivoti;
				if constexpr (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if constexpr (_Matrix(i, j) != _Zero) {
							R_SWITCH(i, Pivoti);
							break;
						}
					}
					if constexpr (i == _Row)
						++Pivotj;
				}
				if constexpr (i != _Row) {
					C_MAKE_SPECIAL(Pivoti, Pivotj);
					++Pivoti; ++Pivotj;
				}
			}
		}

		constexpr void DIAGONALX(void) {
			size_t i = 0, j = 0, Pivoti = 0, Pivotj = 0, RANGE;
			if constexpr (_Row <= _Col) RANGE = _Row; else RANGE = _Col;
			for (j = 0; j < RANGE; ++j) {
				i = Pivoti;
				if constexpr (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if constexpr (_Matrix(i, j) != _Zero) {
							R_SWITCH(i, Pivoti);
							break;
						}
					}
					if constexpr (i == _Row)
						++Pivotj;
				}
				if constexpr (i != _Row) {
					C_MAKE_SPECIAL(Pivoti, Pivotj, true);
					++Pivoti; ++Pivotj;
				}
			}
		}

		constexpr void REF(void) {
			size_t i = 0, j = 0, Pivoti = 0, Pivotj = 0, RANGE;
			if constexpr (_Row <= _Col) RANGE = _Row; else RANGE = _Col;
			for (j = 0; j < RANGE; ++j) {
				i = Pivoti;
				if constexpr (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if constexpr (_Matrix(i, j) != _Zero) {
							R_SWITCH(i, Pivoti);
							break;
						}
					}
					if constexpr (i == _Row)
						++Pivotj;
				}
				if constexpr (i != _Row) {
					C_MAKE(Pivoti, Pivotj);
					++Pivoti; ++Pivotj;
				}
			}
		}

		constexpr void RREF(void) {
			size_t i = 0, j = 0, Pivoti = 0, Pivotj = 0, RANGE;
			if constexpr (_Row <= _Col) RANGE = _Row; else RANGE = _Col;
			for (j = 0; j < RANGE; ++j) {
				i = Pivoti;
				if constexpr (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if constexpr (_Matrix(i, j) != _Zero) {
							R_SWITCH(i, Pivoti);
							break;
						}
					}
					if constexpr (i == _Row)
						++Pivotj;
				}
				if constexpr (i != _Row) {
					C_MAKE(Pivoti, Pivotj, true);
					++Pivoti; ++Pivotj;
				}
			}
		}

	private:
		constexpr void RREF_SPECIAL_FOR_INVERSE(list<Method>& x) {
			size_t i = 0, j = 0, Pivoti = 0, Pivotj = 0, RANGE;
			_Ty Tmp;
			if (_Row <= _Col) RANGE = _Row; else RANGE = _Col;
			for (j = 0; j < RANGE; ++j) {
				i = Pivoti;
				if (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if (_Matrix(i, j) != _Zero) {
							x.emplace_back(ROWSWITCH, i, Pivoti, _Zero);
							R_SWITCH(i, Pivoti);
							break;
						}
					}
					if (i == _Row)
						++Pivotj;
				}
				if (i != _Row) {
					Tmp = _Matrix(Pivoti, Pivotj);
					if (Tmp != _Zero) {
						if (Tmp != unity) {
							x.emplace_back(ROWDIVISION, Pivoti, 0, Tmp);
							R_D(Pivoti, Tmp);
						}
						for (size_t k = 0; k < _Row; ++k) {
							Tmp = _Matrix(k, j);
							if (Tmp != _Zero) {
								x.emplace_back(ROWMINUS, k, Pivoti, Tmp);
								R_MINUS(k, Pivoti, Tmp);
							}
						}
					}
					++Pivoti; ++Pivotj;
				}
			}
		}

	public:
		constexpr void ZERO_MATRIX(void) {
			size_t RANGE = _Row * _Col;
			for (size_t i = 0; i < RANGE; ++i)
				__Matrix(i) = _Zero;
		}

		constexpr void IDENTITY_MATRIX(void) {
			size_t RANGE = (_Row > _Col) ? _Col : _Row; ZERO_MATRIX();
			for (size_t i = 0; i < RANGE; ++i)
				_Matrix(i, i) = unity;
		}

		[[nodiscard]] constexpr _Ty DET() {
			if (_Row == 1) return *_Mat;
			if (_Row == 2) return
				*(_Mat + 0) * *(_Mat + 3) - // a11a22 -
				*(_Mat + 1) * *(_Mat + 2);  // a12a21
			if (_Row == 3) return
				*(_Mat + 0) * *(_Mat + 4) * *(_Mat + 8) + // a11a22a33 +
				*(_Mat + 1) * *(_Mat + 5) * *(_Mat + 6) + // a12a23a31 + 
				*(_Mat + 2) * *(_Mat + 3) * *(_Mat + 7) - // a13a21a32 - 
				*(_Mat + 2) * *(_Mat + 4) * *(_Mat + 6) - // a13a22a31 - 
				*(_Mat + 1) * *(_Mat + 3) * *(_Mat + 8) - // a12a21a33 - 
				*(_Mat + 0) * *(_Mat + 5) * *(_Mat + 7);  // a11a23a32

			size_t i, j, Pivoti = 0, Pivotj = 0; _Ty DET_ = unity;
			for (j = 0; j < _Row; ++j) {
				i = Pivoti;
				if (_Matrix(i, j) == _Zero) {
					for (i = Pivoti + 1; i < _Row; ++i) {
						if (_Matrix(i, j) != _Zero) {
							R_SWITCH(i, Pivoti);
							DET_ = -DET_;
							break;
						}
					}
					if (i == _Row) {
						return _Zero;
					}
				}
				_Ty Tmp = _Matrix(Pivoti, Pivotj);
				if (Tmp != unity) {
					R_D(Pivoti, Tmp);
					DET_ *= Tmp;
				}
				for (size_t k = Pivoti + 1; k < _Row; ++k) {
					Tmp = _Matrix(k, Pivotj);
					if (Tmp != _Zero)
						R_MINUS(k, Pivoti, Tmp);
				}
				++Pivoti; ++Pivotj;
			}
			return DET_;
		}

	private:
		[[nodiscard]] constexpr bool INVERSE_SLOW(void) {
			if (_Row == _Col) {
				size_t i, j; Matrix x(_Row, 2 * _Row);
				for (i = 0; i < _Row; ++i)
					for (j = 0; j < _Col; ++j)
						x_matrix_(i, j) = _Matrix(i, j);
				for (i = 0; i < _Row; ++i) {
					for (j = _Col; j < x._Col; ++j) {
						if (i == j - _Col) x_matrix_(i, j) = 1;
						else x_matrix_(i, j) = _Zero;
					}
				}
				x.RREF();
				for (i = 0; i < _Row; ++i)
					if (x_matrix_(i, i) == _Zero) return false;
				for (i = 0; i < _Row; ++i)
					for (j = 0; j < _Col; ++j)
						_Matrix(i, j) = x_matrix_(i, j + _Col);
				return true;
			}
			return false;
		}

		[[nodiscard]] constexpr size_t RANK(void) {
			size_t Rank = 0; REF();
			for (size_t i = 0; i < _Row; ++i)
				if (!IS_ZERO_ROW(i)) ++Rank;
				else break;
			return Rank;
		}

	public:
		[[nodiscard]] constexpr bool Inverse(void) {
			if (_Row == _Col) {
				list<Method> Tmp; size_t i;
				RREF_SPECIAL_FOR_INVERSE(Tmp);
				for (i = 0; i < _Row; ++i)
					if (_Matrix(i, i) == _Zero) return false;
				for (const auto& method : Tmp) {
					if (method.o == ROWSWITCH)
						R_SWITCH(method._Arg1, method._Arg2);
					else if (method.o == ROWDIVISION)
						R_D(method._Arg1, method.Tmp);
					else
						R_MINUS(method._Arg1, method._Arg2, method.Tmp);
				}
				return true;
			}
			return false;
		}

		[[nodiscard]] constexpr _Ty trace(void)const {
			size_t Range = (_Row >= _Col) ? _Col : _Row;
			_Ty sum = _Zero;
			for (size_t i = 0; i < Range; ++i)
				sum += _Matrix(i, i);
			return sum;
		}

		[[nodiscard]] constexpr _Ty det()const {
			return Matrix(*this).DET();
		}

		[[nodiscard]] constexpr Matrix inverse(void)const {
			Matrix Tmp(*this);
			if (Tmp.Inverse())
				return Tmp;
			//ErrorHandling("inverse()");
			exit(0);
		}

		[[nodiscard]] constexpr Matrix upperTriangular(void)const {
			Matrix Tmp(*this); Tmp.UPPER_TRIANGULAR(); return Tmp;
		}

		[[nodiscard]] constexpr Matrix diagonalx(void)const {
			Matrix Tmp(*this); Tmp.DIAGONALX(); return Tmp;
		}

		[[nodiscard]] constexpr Matrix ref(void)const {
			Matrix Tmp(*this); Tmp.REF(); return Tmp;
		}

		[[nodiscard]] constexpr Matrix rref(void)const {
			Matrix Tmp(*this); Tmp.RREF(); return Tmp;
		}

		[[nodiscard]] constexpr size_t rank(void)const {
			return Matrix(*this).RANK();
		}

		[[nodiscard]] constexpr Matrix zero_matrix(void)const {
			Matrix Tmp(_Row, _Col);
			Tmp.ZERO_MATRIX();
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix identity_matrix(void)const {
			Matrix Tmp(_Row, _Col);
			Tmp.IDENTITY_MATRIX();
			return Tmp;
		}

		[[nodiscard]] constexpr Matrix transpose(void)const {
			Matrix Tmp(_Col, _Row); size_t i, j;
			for (i = 0; i < _Row; ++i)
				for (j = 0; j < _Col; ++j)
					Tmp_matrix_(j, i) = _Matrix(i, j);
			return Tmp;
		}

		// 4. Element Access ---------------------------------------------------------------------------------------------

		[[nodiscard]] constexpr const size_t column(void)const {
			return _Col;
		}
		[[nodiscard]] constexpr const size_t row(void)const {
			return _Row;
		}
		[[nodiscard]] constexpr _Ty& operator[] (const size_t x) {
			if (x < _Row * _Col)
				return *(_Mat + x);
			__debugbreak();
		}
		[[nodiscard]] constexpr _Ty& at(const size_t x) {
			if (x < _Row * _Col)
				return *(_Mat + x);
			__debugbreak();
		}
		[[nodiscard]] constexpr _Ty& at(const size_t i, const size_t j) {
			if (i < _Row && j < _Col)
				return _Matrix(i, j);
			__debugbreak();
		}
		[[nodiscard]] constexpr _Ty& operator () (const size_t x, const size_t y) {
			if (x <= _Row && y <= _Col && x > 0 && y > 0)
				return _Matrix((x - 1), (y - 1));
			__debugbreak();
		}
		[[nodiscard]] constexpr const _Ty& at(const size_t x)const {
			if (x < _Row * _Col)
				return *(_Mat + x);
			__debugbreak();
		}
		[[nodiscard]] constexpr const _Ty& at(const size_t i, const size_t j)const {
			if (i < _Row && j < _Col)
				return _Matrix(i, j);
			__debugbreak();
		}
		[[nodiscard]] constexpr const _Ty& operator () (const size_t x, const size_t y)const {
			if (x <= _Row && y <= _Col && x > 0 && y > 0)
				return _Matrix((x - 1), (y - 1));
			__debugbreak();
		}

		// 5. Operator Overloading ----------------------------------------------------------------------------------------

		[[nodiscard]] constexpr Matrix operator + (const Matrix& x)const {
			if (_Row == x._Row && _Col == x._Col) {
				size_t range = _Row * _Col;
				Matrix Tmp(_Row, _Col);
				for (size_t i = 0; i < range; ++i)
					Tmp_matrix__(i) = x_matrix__(i) + __Matrix(i);
				return Tmp;
			}
			__debugbreak();
		}
		[[nodiscard]] constexpr Matrix operator - (const Matrix& x)const {
			if (_Row == x._Row && _Col == x._Col) {
				Matrix Tmp(_Row, _Col); size_t range = _Row * _Col;
				for (size_t i = 0; i < range; ++i)
					Tmp_matrix__(i) = __Matrix(i) - x_matrix__(i);
				return Tmp;
			}
			__debugbreak();
		}
		[[nodiscard]] constexpr Matrix operator * (const Matrix& x)const {
			if (_Col == x._Row) {
				Matrix Tmp(_Row, x._Col); size_t i, j, k; _Ty sum;
				for (i = 0; i < _Row; ++i) {
					for (j = 0; j < x._Col; ++j) {
						sum = _Zero;
						for (k = 0; k < _Col; ++k)
							sum += _Matrix(i, k) * x_matrix_(k, j);
						Tmp_matrix_(i, j) = sum;
					}
				}
				return Tmp;
			}
			__debugbreak();
		}
		[[nodiscard]] constexpr Matrix operator * (const _Ty& x)const {
			Matrix Tmp(_Row, _Col); size_t RANGE = _Row * _Col;
			for (size_t i = 0; i < RANGE; ++i)
				Tmp_matrix__(i) = x * __Matrix(i);
			return Tmp;
		}

		template<size_t  _SIZE_> [[nodiscard]] constexpr Array<_Ty> operator * (const Array<_Ty>& x)const {
			if (_Col == x.size()) {
				Array<_Ty> Tmp(_Row); _Ty* ptr = Tmp.data(); const _Ty* xptr = x.data();
				size_t i, j; _Ty sum;
				for (i = 0; i < _Row; ++i) {
					sum = _Zero;
					for (j = 0; j < _Col; ++j)
						sum += _Matrix(i, j) * *(xptr + j);
					*(ptr + i) = sum;
				}
				return Tmp;
			}
			__debugbreak();
		}

		// 6. Sub matrices, Minors, Principal Minors ------------------------------------------------------------------------

		[[nodiscard]] constexpr Matrix subMatrix(const Array<size_t>& I, const Array<size_t>& J)const {
			size_t Tmp_row = I.size();
			if (Tmp_row > 0 && Tmp_row <= _Row && Tmp_row == J.size() && *(--I.end()) <= _Row && *(--J.end()) <= _Col) {
				Matrix Tmp(Tmp_row); size_t _Arg1 = 0, j1 = 0;
				for (const auto& i : I) {
					j1 = 0;
					for (const auto& j : J) {
						Tmp_matrix_(_Arg1, j1) = _Matrix((i - 1), (j - 1));
						++j1;
					}
					++_Arg1;
				}
				//Tmp.print();
				return Tmp;
			}
			//std::cout << "Error in sub matrix computation";
			exit(0);
		}
		[[nodiscard]] constexpr _Ty minor(const Array<size_t>& I, const Array<size_t>& J)const {
			return subMatrix(I, J).DET();
		}
		[[nodiscard]] constexpr _Ty signedMinor(const Array<size_t>& I, const Array<size_t>& J)const {
			size_t Tmp = 0;
			for (const auto& i : I)
				Tmp += i;
			for (const auto& i : J)
				Tmp += i;
			return (Tmp & 1) ? -subMatrix(I, J).DET() : subMatrix(I, J).DET();
		}
		[[nodiscard]] constexpr _Ty complementaryMinor(const Array<size_t>& I, const Array<size_t>& J)const {
			Array<size_t> Tmpi, Tmpj;
			size_t indexI = 0, indexJ = 0, sizei = I.size(), sizej = J.size();
			Tmpi.reserve(_Row - sizei);	Tmpj.reserve(_Row - sizej); --sizei; --sizej;
			for (size_t i = 1; i <= _Row; ++i) {
				if (i != I[indexI]) Tmpi.push_back(i); else if (indexI < sizei) ++indexI;
				if (i != J[indexJ]) Tmpj.push_back(i); else if (indexJ < sizej) ++indexJ;
			}
			return subMatrix(Tmpi, Tmpj).DET();
		}
		[[nodiscard]] constexpr _Ty signedComplementaryMinor(const Array<size_t>& I, const Array<size_t>& J)const {
			Array<size_t> Tmpi, Tmpj;
			size_t indexI = 0, indexJ = 0, Tmp = 0, sizei = I.size(), sizej = J.size();
			Tmpi.reserve(_Row - sizei);	Tmpj.reserve(_Row - sizej); --sizei; --sizej;
			for (size_t i = 1; i <= _Row; ++i) {
				if (i != I[indexI]) { Tmpi.push_back(i); Tmp += i; }
				else if (indexI < sizei) ++indexI;
				if (i != J[indexJ]) { Tmpj.push_back(i); Tmp += i; }
				else if (indexJ < sizej) ++indexJ;
			}
			return (Tmp & 1) ? -subMatrix(Tmpi, Tmpj).DET() : subMatrix(Tmpi, Tmpj).DET();
		}
		[[nodiscard]] constexpr _Ty cofactor(const size_t I, const size_t J)const {
			Array<size_t> Tmpi, Tmpj;
			Tmpi.reserve(_Row - 1); Tmpj.reserve(_Row - 1);
			for (size_t i = 1; i <= _Row; ++i) {
				if (i != I) Tmpi.push_back(i);
				if (i != J) Tmpj.push_back(i);
			}
			return ((I + J) & 1) ? -subMatrix(Tmpi, Tmpj).DET() : subMatrix(Tmpi, Tmpj).DET();
		}
		[[nodiscard]] constexpr _Ty sum_ofPrincipleMinor(const size_t Order)const {
			Array<Array<size_t>> Tmp = makeCombi(_Row, Order);
			_Ty sum = _Zero;
			for (const auto& i : Tmp)
				sum += subMatrix(i, i).DET();
			return sum;

			Array<size_t> v(_Row);
			iota(v.begin(), v.end(), 1);
			for_each_combination(v, Order, [&sum, this](const Array<size_t>& i) { sum += this->subMatrix(i, i).DET(); });
			return sum;
		}
		[[nodiscard]] constexpr Matrix adj()const {
			Matrix Tmp(_Row); size_t i, j;
			for (i = 0; i < _Row; ++i)
				for (j = 0; j < _Row; ++j)
					Tmp_matrix_(j, i) = cofactor(i + 1, j + 1);
			return Tmp;
		}

		// 7. Diagonalization EigenValues, EigenVectors, Characteristic Polynomial -------------------------------------------

		//[[nodiscard]] constexpr Polynomial<field> characteristicPolynomial()const {
		//	Array<field> Tmp; field k = -identity;
		//	Tmp.push_back(identity);
		//	for (size_t i = 1; i <= ROW_; ++i)
		//		(i & 1) ? Tmp.push_back(-sum_ofPrincipleMinor(i)) : Tmp.push_back(sum_ofPrincipleMinor(i));
		//	return Polynomial(Tmp);
		//}

		// 8. Method---------------------------------------------------------------------------------------------------------

		[[nodiscard]] constexpr _Ty DETERMINATE_DO_NOT_RECOMENDED(void) {
			if (_Row == _Col) {
				if (_Row == 2) return *_Mat * *(_Mat + 3) - *(_Mat + 2) * *(_Mat + 1);
				Matrix Tmp(_Row - 1, _Col - 1);
				_Ty result(0); size_t i, j, k, m;
				for (i = 0; i < _Col; ++i) {
					k = 0;
					for (j = 0; j < _Col; ++j)
						if (i != j) {
							for (m = 0; m < Tmp._Row; ++m)
								Tmp_matrix_(m, k) = _Matrix((m + 1), j);
							++k;
						}
					if (i % 2 == 0)
						result += __Matrix(i) * DETERMINATE(Tmp);
					else
						result -= __Matrix(i) * DETERMINATE(Tmp);
				}
				return result;
			}
			else {
				__debugbreak();
			}
		} // NOT RECOMMENDED
		/*constexpr void print(void)const {
			size_t  i, j = 0;
			for (i = 0; i < _Row; ++i) {
				std::cout << "\n( ";
				for (j = 0; j < _Col; ++j)
					std::cout << _Matrix(i, j) << " ";
				std::cout << ')';
			}
		}*/
		/*constexpr void scan(void) {
			size_t i, j;
			for (i = 0; i < _Row; ++i)
				for (j = 0; j < _Col; ++j)
					std::cin >> _Matrix(i, j);
		}*/
	private:
		_Ty DETERMINATE(const Matrix& x) {
			if (x._Row == 2) return *x._Mat * *(x._Mat + 3) - *(x._Mat + 2) * *(x._Mat + 1);
			Matrix Tmp(x._Row - 1, x._Col - 1);
			_Ty result(0); size_t i, j, k, m;
			for (i = 0; i < x._Col; ++i) {
				k = 0;
				for (j = 0; j < x._Col; ++j)
					if (i != j) {
						for (m = 0; m < Tmp._Row; ++m)
							Tmp_matrix_(m, k) = x_matrix_((m + 1), j);
						++k;
					}
				if (i % 2 == 0)
					result += x_matrix__(i) * DETERMINATE(Tmp);
				else
					result -= x_matrix__(i) * DETERMINATE(Tmp);
			}
			return result;
		}
	};

}

typedef __int64                       __time64_t;
extern "C" {
	void __cdecl srand(unsigned int _Seed);

	int __cdecl rand(void);

	__time64_t __cdecl _time64(__time64_t* _Time);
}

namespace alpha {
	using _Int = unsigned int;
	enum MatrixType {
		RowMatrix = 1000,		ColumnMatrix,			SquareMatrix,			DiagonalMatrix,
		ScalarMatrix,			UnitMatrix,				TriangularMatrix,		NullMatrix,
		SymmetricMatrix,		SkewSymmetricMatrix,	HermitianMatrix,		SingularMatrix,
		UnitaryMatrix,			OrthogonalMatrix,		ProjectionMatrix,		BlockMatrix,
		ReflectionMatrix,		NilpotantMatrix,		BiDiagonalMatrix,		
		BlockDiagonalMatrix,	BooleanMatrix,			PermutationMatrix,
		VandermondeMatrix,		IdentityMatrix,			ZeroMatrix,
		CompanionMatrix,		RandomMatrix,			RandomSymmetricMatrix,	RandomSkewSymmetricMatrix
	};
};


namespace alpha{

	template<class _Ty>
	class SquareMat {
	public:
		inline static bool EnableIntelligence = false;
	public:
		constexpr auto row()const noexcept {
			return _Row;
		}
		_Ty** data() {
			return _Mat;
		}

		constexpr void display()const noexcept {
			for (_Int i = 0; i < _Row; ++i) {
				_print("\n(");
				for (_Int j = 0; j < _Row; ++j)
					alpha::print(_Mat[i][j], ' ');
				_print(')');
			}
		}

		constexpr void input() noexcept {
			_Ty _Val;
			for (_Int i = 0; i < _Row; ++i) {
				for (_Int j = 0; j < _Row; ++j) {
					alpha::input(_Val);
					_Mat[i][j] = _Val;
				}
			}
		}

		constexpr MatrixType type()const noexcept {
			return MatrixType::SquareMatrix;
		}

		constexpr const char* type_name()const noexcept {
			return "SquareMatrix";
		}
	public:
		explicit constexpr SquareMat(const _Int _Row)noexcept {
			_AllocateExactly(_Row);
		}

		constexpr SquareMat(const _Int _Row, const _Ty& _Arg3)noexcept {
			_AllocateExactly(_Row);
			_Memset(_Arg3);
		}

		constexpr SquareMat(_Int _Row, MatrixType _Type)noexcept {
			_AllocateExactly(_Row);
			switch (_Type) {
				case MatrixType::IdentityMatrix: {
					_Memset(_Zero);
					for (_Int i = 0; i < _Row; ++i)
						_Mat[i][i] = _Unity;
					break;
				}
				case MatrixType::RandomMatrix: {
					unsigned int _Seed = static_cast<unsigned int>(_time64(0));
					srand(_Seed);
					for (_Int i = 0; i < _Row; ++i)
						for (_Int j = 0; j < _Row; ++j) {
							auto val = rand() % 10;
							//if (rand() % 2) val = -val;
							_Mat[i][j] = static_cast<_Ty>(val);
						}
							
					break;
				}
				case MatrixType::ZeroMatrix: {
					_Memset(_Zero);
					break;
				}
				default: {
					if constexpr(_debug)
						__debugbreak();
					break;
				}

			}
		}

		constexpr SquareMat(const initializer<initializer<_Ty>>& _That)noexcept {
			if constexpr (_debug) {
				for (const auto& row : _That) {
					if (_That.size() != row.size()) {
						__debugbreak();
					}
				}
			}

			_Row = static_cast<_Int>(_That.size());
			_AllocateExactly(_Row);
			_Int i = 0, j = 0;
			for (const auto& _row : _That) {
				j = 0;
				for (const auto& _Arg3 : _row) {
					_Mat[i][j] = _Arg3; ++j;
				} ++i;
			}
		}

		constexpr SquareMat(const SquareMat& _Mat, const _Int _I, const _Int _J)noexcept {
			if constexpr (_debug) {
				// check i and j valid or not
			}
			_AllocateExactly(_Mat._Row - 1);
			_Int I = 0, J = 0;
			for (_Int i = 0; i < _Mat._Row; ++i) {
				if (i != _I) {
					J = 0;
					for (_Int j = 0; j < _Mat._Row; ++j) {
						if (j != _J) {
							this->_Mat[I][J] = _Mat._Mat[i][j];
							++J;
						}
					} 
					++I;
				}
			}
		}


		constexpr SquareMat(const SquareMat& _Mat)noexcept {
			_AllocateExactly(_Mat._Row);
			_Copy(_Mat);
		}

		constexpr SquareMat(SquareMat&& _That)noexcept {
			_Mat = _That._Mat;
			_Arr = _That._Arr;
			_Row = _That._Row;

			_That._Mat = 0;
			_That._Arr = 0;
			_That._Row = 0;
		}

		constexpr~SquareMat()noexcept {
			if(_Arr != nullptr)
				_Free();
		}

		constexpr auto swap(SquareMat& _That)noexcept {
			_Ty* _ArrTmp = _Arr;
			_Ty** _MatTmp = _Mat;
			_Int  _RowTmp = _Row;

			_Arr = _That._Arr;
			_Mat = _That._Mat;
			_Row = _That._Row;

			_That._Arr = _ArrTmp;
			_That._Mat = _MatTmp;
			_That._Row = _RowTmp;
		}

		constexpr auto resize(_Int _NewSize)noexcept {

		}
	public:
		[[nodiscard]] constexpr _Ty* operator[](const _Int ith)noexcept {
			if constexpr (_debug) {
				if (ith >= _Row)
					__debugbreak();
			}
			return _Mat[ith];
		}
		[[nodiscard]] constexpr const _Ty* operator[](const _Int ith)const noexcept {
			if constexpr (_debug) {
				if (ith >= _Row)
					__debugbreak();
			}
			return _Mat[ith];
		}
		[[nodiscard]] constexpr _Ty& operator()(const _Int ith, const _Int jth)noexcept {
			if constexpr (_debug) {
				if (ith >= _Row || jth >= _Row)
					__debugbreak();
			}
			return _Mat[ith][jth];
		}
		[[nodiscard]] constexpr const _Ty& operator()(const _Int ith, const _Int jth)const noexcept {
			if constexpr (_debug) {
				if (ith >= _Row || jth >= _Row)
					__debugbreak();
			}
			return _Mat[ith][jth];
		}
	private:
		inline constexpr void _ElemRowTy1(const _Int i1th, const _Int i2th)noexcept {
			if constexpr (_debug) {
				if (i1th >= _Row || i2th == _Row) {
					__debugbreak();
				}
			}

			auto _Tmp = _Mat[i1th];
			_Mat[i1th] = _Mat[i2th];
			_Mat[i2th] = _Tmp;
		}
		inline constexpr void _ElemRowTy2(const _Int ith,  const _Ty& _Scalar, _Int _Pos = 0)noexcept {
			if constexpr (_debug) {
				if (ith >= _Row) {
					__debugbreak();
				}
			}
			if constexpr (alpha::is_same_v<_Ty, float>) {
				alpha::avx_mul_vecf(_Mat[ith] + _Pos, _Scalar, _Row - _Pos);
			} else if constexpr (alpha::is_same_v<_Ty, double>) {
				alpha::avx_mul_vecd(_Mat[ith] + _Pos, _Scalar, _Row - _Pos);
			} else {
				auto _Ptr = _Mat[ith];
				for (; _Pos < _Row; ++_Pos) {
					_Ptr[_Pos] *= _Scalar;
				}
			}
		}
		inline constexpr void _ElemRowTy3(const _Int i1th, const _Int i2th, const _Ty& _Scalar, _Int _Pos = 0)noexcept {
			if constexpr (_debug) {
				if (i1th >= _Row || i2th == _Row || i1th == i2th) {
					__debugbreak();
				}
			}

			auto _Ptr1 = _Mat[i1th];
			auto _Ptr2 = _Mat[i2th];
			auto _Val = _Scalar;

			if constexpr (alpha::is_same_v<_Ty, float>) {
				alpha::avx_madd_vecf(_Ptr1 + _Pos, _Ptr2 + _Pos, _Ptr1 + _Pos, _Scalar, _Row - _Pos);
			} else if constexpr (alpha::is_same_v<_Ty, double>) {
				alpha::avx_madd_vecd(_Ptr1 + _Pos, _Ptr2 + _Pos, _Ptr1 + _Pos, _Scalar, _Row - _Pos);
			} else {
				for (; _Pos < _Row; ++_Pos) {
					_Ptr1[_Pos] += _Scalar * _Ptr2[_Pos];
				}
			}
		}
	public:
		inline constexpr void ElementaryRowOperationType1(const _Int _I1, const _Int _I2)noexcept {
			_ElemRowTy1(_I1, _I2);
		}
		inline constexpr void ElementaryRowOperationType2(const _Int _I, const _Ty& _Scalar)noexcept {
			_ElemRowTy2(_I, _Scalar);
		}
		inline constexpr void ElementaryRowOperationType3(const _Int _I1, const _Int _I2, const _Ty& _Scalar)noexcept {
			_ElemRowTy3(_I1, _I2, _Scalar);
		}

		inline constexpr void ElementaryColOperationType1(const _Int _J1, const _Int _J2)noexcept {
			if constexpr (_debug) {
				if (_J1 >= _Row || _J2 == _Row) {
					__debugbreak();
				}
			}

			for (_Int i = 0; i < _Row; ++i)
				swap(_Mat[i][_J1], _Mat[i][_J2]);
		}
		inline constexpr void ElementaryColOperationType2(const _Int _J, const _Ty& _Scalar)noexcept {
			if constexpr (_debug) {
				if (_J >= _Row) {
					__debugbreak();
				}
			}

			for (_Int i = 0; i < _Row; ++i)
				_Mat[i][_J] *= _Scalar;
		}
		inline constexpr void ElementaryColOperationType3(const _Int _J1, const _Int _J2, const _Ty& _Scalar)noexcept {
			if constexpr (_debug) {
				if (_J1 >= _Row || _J2 == _Row || _J1 == _J2) {
					__debugbreak();
				}
			}

			for (_Int i = 0; i < _Row; ++i)
				_Mat[i][_J1] += _Scalar * _Mat[i][_J2];
		}
	private:
		enum _RowOperationId : unsigned char { _Type1, _Type2, _Type3 };

		struct _RowOperation {
			_RowOperationId _Id;
			_Int _Arg1;
			_Int _Arg2;
			_Ty  _Arg3;

			inline constexpr _RowOperation(_Int I1, _Int I2)noexcept
				: _Id(_Type1), _Arg1(I1), _Arg2(I2){}
			inline constexpr _RowOperation(_Int I1, _Ty& _Scalar)noexcept
				: _Id(_Type2), _Arg1(I1), _Arg3(move(_Scalar)){}
			inline constexpr _RowOperation(_Int I1, _Int I2, _Ty& _Scalar)noexcept
				: _Id(_Type3), _Arg1(I1), _Arg2(I2), _Arg3(move(_Scalar)) {}
		};
	private:
		inline constexpr auto _MakeDownRestZero(const _Int i, const _Int j)noexcept {
			this->_ElemRowTy2(i, _Unity / _Mat[i][j], j);
			for (_Int _idx = i + 1; _idx < _Row; ++_idx) {
				if (_Mat[_idx][j] != _Zero)
					this->_ElemRowTy3(_idx, i, -_Mat[_idx][j], j);
			}
		}
		inline constexpr auto _MakeRightRestZero(const _Int i, const _Int j)noexcept {

		}

		inline constexpr auto _MakeUpDownRestZero(const _Int i, const _Int j)noexcept {
			this->_ElemRowTy2(i, _Unity / _Mat[i][j], j);
			for (_Int _idx = 0; _idx < i; ++_idx) {
				if (_Mat[_idx][j] != _Zero)
					_ElemRowTy3(_idx, i, (-_Mat[_idx][j]), j);
			}
			for (_Int _idx = i + 1; _idx < _Row; ++_idx) {
				if (_Mat[_idx][j] != _Zero)
					_ElemRowTy3(_idx, i, -_Mat[_idx][j], j);
			}
		}
		inline constexpr auto _MakeLeftRightRestZero(const _Int i, const _Int j)noexcept {

		}

		inline constexpr auto _RREFInverseHelper(const _Int i, const _Int j, Array<_RowOperation>& _Data)noexcept {
			_Ty _Scalar = _Unity / _Mat[i][j];
			this->_ElemRowTy2(i, _Scalar, j);
			_Data.emplace_back(i, _Scalar);
			_Int _idx = 0;
			for (; _idx < i; ++_idx) {
				if (_Mat[_idx][j] != _Zero) {
					_Scalar = -_Mat[_idx][j];
					this->_ElemRowTy3(_idx, i, _Scalar, j);
					_Data.emplace_back(_idx, i, _Scalar);
				}
			}
			for (++_idx; _idx < _Row; ++_idx) {
				if (_Mat[_idx][j] != _Zero) {
					_Scalar = -_Mat[_idx][j];
					this->_ElemRowTy3(_idx, i, _Scalar, j);
					_Data.emplace_back(_idx, i, _Scalar);
				}
			}
		}

		inline constexpr auto _StoreOperationData(Array<_RowOperation>& _Data)noexcept {
			_Int _Cpivot = 0, _Rpivot = 0, _idx;
			for (_Cpivot = 0; _Cpivot < _Row; ++_Cpivot) {
				if (_Mat[_Rpivot][_Cpivot] == _Zero) {
					_idx = _Rpivot + 1;
					for (; _idx < _Row; ++_idx) {
						if (_Mat[_idx][_Cpivot] != _Zero) {
							this->_ElemRowTy1(_idx, _Rpivot);
							_Data.emplace_back(_idx, _Rpivot); break;
						}
					}
					if (_idx < _Row) {
						this->_RREFInverseHelper(_Rpivot, _Cpivot, _Data);
						++_Rpivot;
					} else return false;
				} else {
					this->_RREFInverseHelper(_Rpivot, _Cpivot, _Data);
					++_Rpivot;
				}
			} return true;
		}
	private:
		template<class _Fn1, class _Fn2>
		constexpr void _REDUCE(_Fn1 _Func1, _Fn2 _Func2)noexcept {
			_Int _Cpivot = 0, _Rpivot = 0;
			for (_Cpivot = 0; _Cpivot < _Row; ++_Cpivot) {
				if (_Mat[_Rpivot][_Cpivot] == _Zero) {
					auto i = _Rpivot + 1;
					for (; i < _Row; ++i)
						if (_Mat[i][_Cpivot] != _Zero) {
							_Func1(i, _Rpivot); break;
						}
					if (i < _Row) {
						_Func2(_Rpivot, _Cpivot);
						++_Rpivot;
					}
				} else {
					_Func2(_Rpivot, _Cpivot);
					++_Rpivot;
				}
			}
		}
	public:
		constexpr auto& rrf()noexcept {
			_Int _Cpivot = 0, _Rpivot = 0;
			for (_Cpivot = 0; _Cpivot < _Row; ++_Cpivot) {
				if (_Mat[_Rpivot][_Cpivot] == _Zero) {
					auto i = _Rpivot + 1;
					for (; i < _Row; ++i)
						if (_Mat[i][_Cpivot] != _Zero) {
							_ElemRowTy1(i, _Rpivot); break;
						}
					if (i < _Row) {
						_MakeDownRestZero(_Rpivot, _Cpivot);
						++_Rpivot;
					}
				} else {
					_MakeDownRestZero(_Rpivot, _Cpivot);
					++_Rpivot;
				}
			}
			return *this;
		}

		constexpr auto& rref()noexcept {
			_Int _Cpivot = 0, _Rpivot = 0;
			for (_Cpivot = 0; _Cpivot < _Row; ++_Cpivot) {
				if (_Mat[_Rpivot][_Cpivot] == _Zero) {
					auto i = _Rpivot + 1;
					for (; i < _Row; ++i) {
						if (_Mat[i][_Cpivot] != _Zero) {
							_ElemRowTy1(i, _Rpivot); break;
						}
					}
					if (i < _Row) {
						_MakeUpDownRestZero(_Rpivot, _Cpivot);
						++_Rpivot;
					}
				} else {
					_MakeUpDownRestZero(_Rpivot, _Cpivot);
					++_Rpivot;
				}
			}
			return *this;
		}

		constexpr bool Inverse()noexcept {
			Array<_RowOperation> OperationDataList(static_cast<size_t>(_Row) * _Row);
			if (_StoreOperationData(OperationDataList)) {
				for (auto& OperationData : OperationDataList) {
					switch (OperationData._Id) {
						case _RowOperationId::_Type1:
							this->ElementaryRowOperationType1(OperationData._Arg1, OperationData._Arg2);
							break;
						case _RowOperationId::_Type2:
							this->ElementaryRowOperationType2(OperationData._Arg1, OperationData._Arg3);
							break;
						case _RowOperationId::_Type3:
							this->ElementaryRowOperationType3(OperationData._Arg1, OperationData._Arg2, OperationData._Arg3);
							break;
						default: break;
					}
				} return true;
			} return false;
		}

	private:
		inline constexpr auto _RREFinverseHelper(const _Int i, const _Int j, SquareMat& _Imat)noexcept {
			_Ty _Scalar = _Unity / _Mat[i][j];
			this->_ElemRowTy2(i, _Scalar, j);
			_Imat._ElemRowTy2(i, _Scalar);
			_Int _idx = 0;
			for (; _idx < i; ++_idx) {
				if (_Mat[_idx][j] != _Zero) {
					_Scalar = -_Mat[_idx][j];
					this->_ElemRowTy3(_idx, i, _Scalar, j);
					_Imat._ElemRowTy3(_idx, i, _Scalar);
				}
			}
			for (++_idx; _idx < _Row; ++_idx) {
				if (_Mat[_idx][j] != _Zero) {
					_Scalar = -_Mat[_idx][j];
					this->_ElemRowTy3(_idx, i, _Scalar, j);
					_Imat._ElemRowTy3(_idx, i, _Scalar);
				}
			}
		}
		inline constexpr auto _ChangeToIndentity()noexcept {
			_Memset(_Zero);
			for (_Int i = 0; i < _Row; ++i)
				_Mat[i][i] = _Unity;
		}
		inline constexpr auto _InverseGreater3_3()noexcept {
			static SquareMat _Imat(_Row, MatrixType::IdentityMatrix);
			_Imat.resize(_Row);
			_Int _Cpivot = 0, _Rpivot = 0, _idx;
			for (_Cpivot = 0; _Cpivot < _Row; ++_Cpivot) {
				if (_Mat[_Rpivot][_Cpivot] == _Zero) {
					for (_idx = _Rpivot + 1; _idx < _Row; ++_idx) {
						if (_Mat[_idx][_Cpivot] != _Zero) {
							this->_ElemRowTy1(_idx, _Rpivot);
							_Imat._ElemRowTy1(_idx, _Rpivot); break;
						}
					} if (_idx < _Row) {
						_RREFinverseHelper(_Rpivot, _Cpivot, _Imat);
						++_Rpivot;
					} else {
						_Imat._ChangeToIndentity();
						return false;
					}
				} else {
					_RREFinverseHelper(_Rpivot, _Cpivot, _Imat);
					++_Rpivot;
				}
			} this->swap(_Imat); return true;
		}
	public:
		constexpr bool inverse()noexcept {
			if (_Row > 3) return _InverseGreater3_3();
			else {
				switch (_Row) {
				case 1: {

				}
				}
			}
		}

		constexpr bool inverse(bool _Flag)noexcept {
			if (_Flag) {
				SquareMat _Copy = *this;
				if (!inverse()) {
					this->swap(_Copy);
					return false;
				} else return true;
			} else return inverse();
		}

	private:
		constexpr inline auto _DetGreater4_4()const noexcept{
			_Int i = 0; _Ty _Sum = _Zero, _Val;
			for (_Int j = 0; j < _Row; ++j) {
				if (_Mat[i][j] != _Zero) {
					_Val = _Mat[i][j];
					_Val *= SquareMat(*this, i, j).det();
					(i + j) & 1 ? _Sum -= _Val : _Sum += _Val;
				}
			}
			return _Sum;
		}
	public:
		constexpr auto det()const noexcept {
			if (_Row <= 3)
				if (_Row == 3) return
					_Mat[0][0] * _Mat[1][1] * _Mat[2][2] + _Mat[0][1] * _Mat[1][2] * _Mat[2][0] +
					_Mat[0][2] * _Mat[1][0] * _Mat[2][1] - _Mat[0][2] * _Mat[1][1] * _Mat[2][0] -
					_Mat[0][1] * _Mat[1][0] * _Mat[2][2] - _Mat[0][0] * _Mat[1][2] * _Mat[2][1];
				else if (_Row == 2) return
					_Mat[0][0] * _Mat[1][1] - _Mat[0][1] * _Mat[1][0];
				else if (_Row == 1) return
					_Mat[0][0];
				else __debugbreak;

			return _DetGreater4_4();
		}

		constexpr auto determinate()const noexcept {
			
		}

		constexpr auto isZeroRow(const unsigned int i)const noexcept {
			for (_Int k = 0; k < _Row; ++k)
				if (_Mat[i][k] != _Zero) 
					return false;
			return true;
		}

		constexpr auto isRRF()const noexcept{
			for (_Int k = 0; k < _Row; ++k) {
				if (_Mat[k][k] == _Unity || _Mat[k][k] == _Zero) {
					for (_Int i = k + 1; i < _Row; ++i)
						if (_Mat[i][k] != _Zero)
							return false;
				} else return false;
			} return true;
		}

		/*constexpr auto IsRREF()const noexcept {
			_Int i, j;
			for (i = 0; i < _Row; ++i)
				for (j = i + 1; j < _Row; ++j)
					if (_Mat[i][j] != _Zero)
						return false;
			for (i = 0; i < _Row; ++i)
				if (_Mat[i][i] != _Zero && _Mat[i][i] != _Unity)
					return false;
			for (i = 1; i < _Row; ++i )
				for(j = 1;)
		}*/

	private:
		constexpr auto _CountNonZeroRow()noexcept {
			_Int _Count = 0;
			for (_Int i = 0; i < _Row; ++i)
				if (!isZeroRow(i)) ++_Count;
			return _Count;
		}
		
	public:
		constexpr auto rank()noexcept {
			if (isRRF()) return _CountNonZeroRow();
			SquareMat _Tmp = *this;
			_Tmp.rrf(); return _Tmp._CountNonZeroRow();
		}

	public:

		constexpr SquareMat operator()(const Array<_Int>& _Data1, const Array<_Int>& _Data2)noexcept {
			if constexpr (_debug) {
				if (_Data1.size() != _Data2.size()) __debugbreak();
				//check array is sorted or not
			}
			SquareMat _SubMatrix(static_cast<_Int>(_Data1.size()));
			_Int i = 0, j = 0;
			for (const auto _Posi : _Data1) {
				j = 0;
				for (const auto _Posj : _Data2) { 
					_SubMatrix._Mat[i][j++] = _Mat[_Posi][_Posj];
				} 
				++i;
			}
			return _SubMatrix;
		}

		constexpr _Ty minor(const Array<_Int>& I, const Array<_Int>& J)noexcept {
			return this->operator()(I, J).det();
		}

		constexpr _Ty signedMinor(const Array<_Int>& I, const Array<_Int>& J)noexcept {
			auto _Total = accumulate(I.begin(), I.end(), 0U) + accumulate(J.begin(), J.end(), 0U);
			auto _Signed = _Total & 1 ? -_Unity : _Unity;
			return this->operator()(I, J).det() * _Signed;
		}

		
	public:
		constexpr auto operator*=(const SquareMat& _That)noexcept {

		}

		constexpr auto operator+=(const SquareMat& _That)noexcept {
			if constexpr (_debug) {
				if (_Rowcheck(_That)) __debugbreak();
			}

			for (_Int i = 0; i < _Row; ++i) {
				for (_Int j = 0; j < _Row; ++j) {
					_Mat[i][j] += _That._Mat[i][j];
				}
			}
		}

		
		constexpr auto operator-=(const SquareMat& _That)noexcept {
			if constexpr (_debug) {
				if (_Rowcheck(_That)) __debugbreak();
			}

			for (_Int i = 0; i < _Row; ++i) {
				for (_Int j = 0; j < _Row; ++j) {
					_Mat[i][j] -= _That._Mat[i][j];
				}
			}
		}

		constexpr auto operator*(const SquareMat& _That)const noexcept {
			SquareMat _TmpMat(_Row);
			_Int i, j, k; _Ty _Sum;
			for (i = 0; i < _Row; ++i) {
				for (j = 0; j < _Row; ++j) {
					_Sum = _Zero;
					for (k = 0; k < _Row; ++k)
						_Sum += _Mat[i][k] * _That._Mat[k][j];
					_TmpMat._Mat[i][j] = _Sum;
				}
			}
			return _TmpMat;
		}

		constexpr auto operator+(const SquareMat& _That)const noexcept {
			SquareMat _TmpMat(_Row);
			for (_Int i = 0; i < _Row; ++i) {
				if constexpr (is_same_v<_Ty, float>)
					alpha::avx_add_vecf(_Mat[i], _That._Mat[i], _TmpMat._Mat[i], _Row);
				else if constexpr (is_same_v<_Ty, double>)
					alpha::avx_add_vecd(_Mat[i], _That._Mat[i], _TmpMat._Mat[i], _Row);
				else {
					for (_Int j = 0; j < _Row; ++j) {
						_TmpMat._Mat[i][j] = _Mat[i][j] + _That._Mat[i][j];
					}
				}
			}
			return _TmpMat;
		}

		constexpr auto operator-(const SquareMat& _That)const noexcept {
			SquareMat _TmpMat(_Row);
			for (_Int i = 0; i < _Row; ++i) {
				if constexpr (is_same_v<_Ty, float>)
					alpha::avx_sub_vecf(_Mat[i], _That._Mat[i], _TmpMat._Mat[i], _Row);
				else if constexpr (is_same_v<_Ty, double>)
					alpha::avx_sub_vecd(_Mat[i], _That._Mat[i], _TmpMat._Mat[i], _Row);
				else {
					for (_Int j = 0; j < _Row; ++j) {
						_TmpMat._Mat[i][j] = _Mat[i][j] - _That._Mat[i][j];
					}
				}
			}
			return _TmpMat;
		}






























	private:
		inline constexpr bool _Rowcheck(const SquareMat& _That)noexcept {
			return _Row == _That._Row;
		}

		inline constexpr void _AllocateExactly(const _Int _Siz)noexcept {
			_Row = _Siz;
			_Arr = static_cast<_Ty*> (malloc(((_Int)_Siz) * _Siz * sizeof(_Ty)));
			_Mat = static_cast<_Ty**>(malloc(_Siz * sizeof(_Ty*)));
			if constexpr (_debug) {
				if (_Mat == 0 || _Arr == 0)
					__debugbreak();
			}
			for (_Int i = 0; i < _Siz; ++i)
				_Mat[i] = _Arr + i * _Siz;
		}
		inline constexpr void _Free()noexcept {
			if constexpr (!is_trivially_destructible_v<_Ty>)
				for (_Int _idx = 0; _idx < _Row; ++_idx)
					_Arr[_idx].~_Ty();
			free(_Arr);
			free(_Mat);

			_Mat = nullptr;
			_Arr = nullptr;
			_Row = 0;
		}
		inline constexpr void _Copy(const SquareMat& _That)noexcept {
			_Row = _That._Row;
			for (_Int i = 0; i < _Row; ++i)
				for (_Int j = 0; j < _Row; ++j)
					_Mat[i][j] = _That._Mat[i][j];
		}
		inline constexpr void _Memset(const _Ty& _Val)noexcept {
			_Int _Siz = _Row * _Row;
			if constexpr (is_fundamental_v<_Ty>) {
				if constexpr (is_same_v<_Ty, char>) {
					memset(_Arr, _Val, _Siz);
				} else {
					if (_Val != 0) {
						for (_Int _idx = 0; _idx < _Siz; ++_idx)
							_Arr[_idx] = _Val;
					} else {
						memset(_Arr, 0, _Siz * sizeof(_Ty));
					}
				}
			} else {
				for (_Int _idx = 0; _idx < _Siz; ++_idx)
					_Arr[_idx] = _Val;
			}
		}
	private:
		inline static const _Ty _Zero = static_cast<_Ty>(0);
		inline static const _Ty _Unity = static_cast<_Ty>(1);

		_Ty** _Mat;
		_Ty* _Arr;
		_Int _Row;
	};

	template<class _Ty>
	inline constexpr auto _print(const SquareMat<_Ty>& _Mat)noexcept {
		_print('\n');
		_Mat.display();
	}
	template<class _Ty>
	inline constexpr auto _input(SquareMat<_Ty>& _Mat)noexcept {
		_Mat.input();
	}
	template<class _Ty>
	inline constexpr auto det(const SquareMat<_Ty>& _Mat)noexcept {
		return _Mat.det();
	}
	
	/*template<class _Ty> class DiagonalMatrix {
	private:

	public:
		constexpr DiagonalMatrix(size_t _Siz)noexcept {
			_Arr = static_cast<_Ty*>(malloc(_Siz * sizeof(_Ty)));
			_Row = _Siz;
		}
	private:
		using size_t = size_t;
		_Ty* _Arr;
		size_t _Row;

	};*/

	/*template<class _Ty> class ScalarMatrix : public DiagonalMatrix {

	};

	template<class _Ty> class UnitMatrix : public ScalarMatrix {

	};

	template<class _Ty> class TriangularMatrix : public SquareMatrix {

	};

	template<class _Ty> class UpperTriangularMatrix : public TriangularMatrix {

	};

	template<class _Ty> class SymmetricMatrix : public SquareMatrix {

	};

	template<class _Ty> class SkewSymmetricMatrix : public SquareMatrix {

	};

	template<class _Ty> class SingularMatrix : public SquareMatrix {

	};

	template<class _Ty> class UnitaryMatrix : public SquareMatrix {

	};

	template<class _Ty> class NilpotantMatrix : public SquareMatrix {

	};

	template<class _Ty> class HermitianMatrix : public SquareMatrix {

	};

	template<class _Ty> class OrthogonalMatrix : public SquareMatrix {

	};

	template<class _Ty> class ProjectionMatrix : public SquareMatrix {

	};

	template<class _Ty> class ReflectionMatrix : public SquareMatrix {

	};

	template<class _Ty> class BiDiagonalMatrix : public SquareMatrix {

	};

	template<class _Ty> class BlockMatrix : public SquareMatrix {

	};

	template<class _Ty> class BlockDiagonalMatrix : public SquareMatrix {

	};

	template<class _Ty> class BooleanMatrix : public SquareMatrix {

	};

	template<class _Ty> class PermutationMatrix : public SquareMatrix {

	};

	template<class _Ty> class VandermondeMatrix : public SquareMatrix {

	};

	template<class _Ty> class CompanionMatrix : public SquareMatrix {

	};

	template<class _Ty> class NullMatrix {

	};*/
};

#endif