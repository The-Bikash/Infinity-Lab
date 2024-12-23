#pragma once
#ifndef __POLYNOMIAL__
#define __POLYNOMIAL__

#include "../DataStructures/string.h"


//////////////////////////////////////////////////////////////////////
#define WriteCodeForSearching(Method)								\
template<typename, typename T>										\
struct Has##Method {												\
	static_assert(integral_constant<T, false>::value,				\
	"Second template parameter needs to be of function type.");		\
};																	\
																	\
template<typename ClassType, typename ReturnType, typename... Args>	\
struct Has##Method<ClassType, ReturnType(Args...)> {				\
private:															\
	template<typename T>											\
	static constexpr auto check(T*)									\
		-> typename is_same<										\
			ReturnType,												\
			decltype(declval<T>().##Method(declval<Args>()...))		\
		>::type;													\
																	\
	template<typename>												\
	static constexpr false_type check(...);							\
																	\
	typedef decltype(check<ClassType>(0)) type;						\
																	\
public:																\
	static constexpr bool value = type::value;						\
};																	
/////////////////////////////////////////////////////////////////////

namespace alpha {
	WriteCodeForSearching(_EvalutePolynomial)

	template<class _Ty>
	class Polynomial : public Array<_Ty> {
	public:
		using Array<_Ty>::Array; // Take all constructors from base class
		using Array<_Ty>::operator=;

		constexpr _Ty operator()(const _Ty& x)const noexcept {
			if constexpr (_debug) if (this->_Siz == 0) __debugbreak();
			if (this->_Siz > 2) {
				if constexpr (Has_EvalutePolynomial<_Ty, _Ty(_Ty*, _Ty*)>::value) {
					return x._EvalutePolynomial(this->_Ptr, this->_Ptr + this->_Siz);
				} else {
					long long i = this->_Siz - 1;
					auto _Result = this->_Ptr[i];
					--i;
					for (; i >= 0; --i) {
						_Result *= x;
						_Result += this->_Ptr[i];
					} return _Result;
				}
			} else if (this->_Siz == 2) {
				auto _Result = this->_Ptr[1] * x;
				_Result += this->_Ptr[0];
				return _Result;
			} else return this->_Ptr[0];
		}
	private:
		inline constexpr void _RemoveTrailingZeros()noexcept {
			while (this->_Ptr[this->_Siz - 1] == static_cast<_Ty>(0))
				--(this->_Siz);
		}
		inline constexpr void _FFT_COEFF_TO_VALUE_TRANSFORM(_Ty*)noexcept {
			
		}
		inline constexpr void _MULTIPLY_USING_VALUE_REPRESENTATION()noexcept {

		}
		inline constexpr void _FFT_POLYNOMIAL_MUL()noexcept {

		}
	public:
		constexpr Polynomial& operator+=(const Polynomial& _Pol)noexcept override {
			auto _Size = (this->_Siz >= _Pol._Siz) ? (_Pol._Siz) : (this->_Siz);
	
			for (size_t _idx = 0; _idx < _Size; ++_idx) {
				this->_Ptr[_idx] += _Pol._Ptr[_idx];
			}
			for (size_t _idx = _Size; _idx < _Pol._Siz; ++_idx) {
				this->emplace_back(_Pol._Ptr[_idx]);
			}
			this->_RemoveTrailingZeros();
			return *this;
		}

		constexpr Polynomial& operator-=(const Polynomial& _Pol)noexcept {
			auto _Size = (this->_Siz >= _Pol._Siz) ? (_Pol._Siz) : (this->_Siz);
	
			for (size_t _idx = 0; _idx < _Size; ++_idx) {
				this->_Ptr[_idx] -= _Pol._Ptr[_idx];
			}
			for (size_t _idx = _Size; _idx < _Pol._Siz; ++_idx) {
				this->emplace_back(-_Pol._Ptr[_idx]);
			}
			this->_RemoveTrailingZeros();
			return *this;
		}

		constexpr Polynomial& operator*=(const Polynomial& _Pol)noexcept override {
			return *this;
		}

	};

	template<class _Ty>
	void _print(const Polynomial<_Ty>& _Pol)noexcept {
		_Pol.print();
	}
}

//export namespace alpha {
//	using _Sz = int32_t;
//
//	template<class _Ty>
//	class Polynomial {
//
//	};
//
//	template<class _Ty>
//	class MultivariatePolynomial{
//	public:
//
//		constexpr MultivariatePolynomial()noexcept
//		{
//
//		}
//
//		constexpr MultivariatePolynomial(const _Ty& coeff, const Array<string>& vL)noexcept :vL(vL)
//		{
//			fx.emplace_back(coeff, dynamic_array<_Sz>(vL.size(), 0ll));
//		}
//
//		constexpr MultivariatePolynomial(const _Ty& coeff, const Array<_Sz>& expo, const Array<std::string>& vL)noexcept :vL(vL)
//		{
//			fx.emplace_back(coeff, expo);
//		}
//
//		constexpr MultivariatePolynomial(const std::list<std::pair<_Ty, dynamic_array<_Sz>>>& fx, const dynamic_array<std::string> vL)noexcept : vL(vL)
//		{
//			//for (const auto& pair_ : fx)
//			//{
//			//	if (pair_.second.size() != vL.size()) throw("ERROR");
//			//}
//
//			//std::set<std::string> TmpSet(vL.begin(), vL.end());
//
//			//if (TmpSet.size() != vL.size())
//			//{
//			//	throw("ERROR");  // Searching duplicates Variable;
//			//}
//
//			this->vL = vL;
//
//			for (const auto& pair_ : fx)
//			{
//				this->fx.emplace_back(pair_.first, pair_.second);
//			}
//
//			this->fx.sort();
//		}
//
//		constexpr MultivariatePolynomial(const MultivariatePolynomial& pol)noexcept
//		{
//			this->vL = pol.vL;
//			this->fx = pol.fx;
//		}
//
//		constexpr MultivariatePolynomial(MultivariatePolynomial&& pol)noexcept
//		{
//			this->vL = std::move(pol.vL);
//			this->fx = std::move(pol.fx);
//		}
//
//		constexpr MultivariatePolynomial& operator = (MultivariatePolynomial&& pol)noexcept
//		{
//			this->vL = std::move(pol.vL);
//			this->fx = std::move(pol.fx);
//
//			return *this;
//		}
//
//		constexpr _Ty operator()(const Array<_Ty>& _Val)const
//		{
//
//			if (_Val.size() != vL.size()) throw("ERROR");
//
//			_Sz n = fx.size(), m = _Val.size(), i = 0, j = 0;
//
//			_Ty sum = 0ll, Tmp = 1ll;
//
//			static dynamic_array<std::pair<_Sz, _Ty>> Tmpcatch;
//
//			Tmpcatch = m;
//
//			for (i = 0; i < m; ++i)
//			{
//				// reset Tmpcatch
//
//				Tmpcatch[i].first = 0ll;
//
//				Tmpcatch[i].second = 1ll;
//			}
//
//			for (auto rit = fx.rbegin(); rit != fx.rend(); ++rit)
//			{
//				Tmp = 1ll;
//
//				for (j = 0; j < m; ++j)
//				{
//					if (rit->expo[j] >= Tmpcatch[j].first)
//					{
//						Tmpcatch[j].second *= pow(_Val[j], rit->expo[j] - Tmpcatch[j].first);
//
//						Tmpcatch[j].first = rit->expo[j];
//
//						Tmp *= Tmpcatch[j].second;
//
//						continue;
//					}
//
//					Tmp *= pow(_Val[j], rit->expo[j]);
//				}
//
//				sum += Tmp * rit->coeff;
//			}
//
//			return sum;
//		}
//
//		constexpr MultivariatePolynomial operator + (const MultivariatePolynomial& f)const noexcept
//		{
//			MultivariatePolynomial TmpPoly;
//
//			if (f.vL == vL)
//			{
//				auto iterator1 = fx.begin();
//
//				auto iterator2 = f.fx.begin();
//
//				auto iterator1e = fx.end();
//
//				auto iterator2e = f.fx.end();
//
//				while (iterator1 != iterator1e && iterator2 != iterator2e)
//				{
//					if (*iterator1 == *iterator2)
//					{
//						TmpPoly.fx.emplace_back(*iterator1 + *iterator2);
//
//						++iterator1;
//						++iterator2;
//
//						continue;
//					}
//
//					if (*iterator1 < *iterator2)
//					{
//						TmpPoly.fx.push_back(*iterator1);
//
//						++iterator1;
//
//						continue;
//					}
//
//					TmpPoly.fx.push_back(*iterator2);
//
//					++iterator2;
//
//				}
//
//				while (iterator1 != iterator1e)
//				{
//					TmpPoly.fx.push_back(*iterator1);
//
//					++iterator1;
//				}
//
//				while (iterator2 != iterator2e)
//				{
//					TmpPoly.fx.push_back(*iterator2);
//
//					++iterator2;
//				}
//
//				TmpPoly.vL = vL;
//
//				return TmpPoly;
//			}
//
//			MultivariatePolynomial P1 = *this, P2 = f;
//
//			upgrade_poly(P1, P2);
//
//			auto iterator1 = P1.fx.begin();
//
//			auto iterator2 = P2.fx.begin();
//
//			auto iterator1e = P1.fx.end();
//
//			auto iterator2e = P2.fx.end();
//
//			while (iterator1 != iterator1e && iterator2 != iterator2e)
//			{
//				if (*iterator1 == *iterator2)
//				{
//					TmpPoly.fx.emplace_back(*iterator1 + *iterator2);
//
//					++iterator1;
//					++iterator2;
//
//					continue;
//				}
//
//				if (*iterator1 < *iterator2)
//				{
//					TmpPoly.fx.push_back(*iterator1);
//
//					++iterator1;
//
//					continue;
//				}
//
//				TmpPoly.fx.push_back(*iterator2);
//
//				++iterator2;
//
//			}
//
//			while (iterator1 != iterator1e)
//			{
//				TmpPoly.fx.push_back(*iterator1);
//
//				++iterator1;
//			}
//
//			while (iterator2 != iterator2e)
//			{
//				TmpPoly.fx.push_back(*iterator2);
//
//				++iterator2;
//			}
//
//			TmpPoly.vL = P1.vL;
//
//			return TmpPoly;
//
//		}
//
//		constexpr MultivariatePolynomial& operator += (const MultivariatePolynomial& f)noexcept {
//
//			if (f.vL == vL)
//			{
//				auto iterator1 = fx.begin();
//
//				auto iterator2 = f.fx.begin();
//
//				while (iterator1 != fx.end() && iterator2 != f.fx.end())
//				{
//
//					if (*iterator1 == *iterator2)
//					{
//						*iterator1 += *iterator2;
//
//						++iterator1;
//						++iterator2;
//
//						continue;
//					}
//
//					if (*iterator1 < *iterator2)
//					{
//						// Actually this is *ptr1 > *ptr2.
//
//						++iterator1;
//
//						continue;
//					}
//
//					iterator1 = fx.insert(iterator1, *iterator2);
//
//					++iterator2;
//
//				}
//
//				while (iterator2 != f.fx.end())
//				{
//					fx.push_back(*iterator2);
//
//					++iterator2;
//				}
//
//				return *this;
//			}
//
//			MultivariatePolynomial p2 = f;
//
//			upgrade_poly(*this, p2);
//
//			auto iterator1 = fx.begin();
//
//			auto iterator2 = p2.fx.begin();
//
//			while (iterator1 != fx.end() && iterator2 != p2.fx.end())
//			{
//
//				if (*iterator1 == *iterator2)
//				{
//					*iterator1 += *iterator2;
//
//					++iterator1;
//					++iterator2;
//
//					continue;
//				}
//
//				if (*iterator1 < *iterator2)
//				{
//					// Actually this is *ptr1 > *ptr2.
//
//					++iterator1;
//
//					continue;
//				}
//
//				iterator1 = fx.insert(iterator1, *iterator2);
//
//				++iterator2;
//
//			}
//
//			while (iterator2 != p2.fx.end())
//			{
//				fx.push_back(*iterator2);
//
//				++iterator2;
//			}
//
//			return *this;
//		}
//
//		constexpr MultivariatePolynomial operator * (const MultivariatePolynomial& f)const
//		{
//			MultivariatePolynomial TmpPoly;
//
//			if (f.vL == vL)
//			{
//				auto iterator1 = fx.begin();
//				auto iterator2 = f.fx.begin();
//
//				while (iterator1 != fx.end())
//				{
//					iterator2 = f.fx.begin();
//
//					while (iterator2 != f.fx.end())
//					{
//						TmpPoly.fx.emplace_back(*iterator1 * *iterator2);
//
//						++iterator2;
//					}
//
//					++iterator1;
//				}
//
//				TmpPoly.fx.sort();
//
//				auto iteratorx = TmpPoly.fx.begin();
//				auto iterator3 = ++TmpPoly.fx.begin();
//
//				bool Delete = false;
//
//				while (iterator3 != TmpPoly.fx.end())
//				{
//					if (*iteratorx == *iterator3)
//					{
//						Delete = true;
//
//						*iteratorx += *iterator3;
//					}
//					else
//					{
//						++iteratorx;
//
//						if (Delete)
//						{
//							iteratorx = TmpPoly.fx.erase(iteratorx, iterator3);
//
//							Delete = false;
//						}
//					}
//
//					++iterator3;
//				}
//
//				TmpPoly.vL = vL;
//
//				return TmpPoly;
//
//			}
//
//			MultivariatePolynomial P1 = *this, P2 = f;
//
//			upgrade_poly(P1, P2);
//
//			auto iterator1 = P1.fx.begin();
//			auto iterator2 = P2.fx.begin();
//
//			while (iterator1 != P1.fx.end())
//			{
//				iterator2 = P2.fx.begin();
//
//				while (iterator2 != P2.fx.end())
//				{
//					TmpPoly.fx.emplace_back(*iterator1 * *iterator2);
//
//					++iterator2;
//				}
//
//				++iterator1;
//			}
//
//			TmpPoly.fx.sort();
//
//			auto iteratorx = TmpPoly.fx.begin();
//			auto iterator3 = ++TmpPoly.fx.begin();
//
//			bool Delete = false;
//
//			while (iterator3 != TmpPoly.fx.end())
//			{
//				if (*iteratorx == *iterator3)
//				{
//					Delete = true;
//
//					*iteratorx += *iterator3;
//				}
//				else
//				{
//					++iteratorx;
//
//					if (Delete)
//					{
//						iteratorx = TmpPoly.fx.erase(iteratorx, iterator3);
//
//						Delete = false;
//					}
//				}
//
//				++iterator3;
//			}
//
//			TmpPoly.vL = P1.vL;
//
//			return TmpPoly;
//		}
//
//		constexpr Array<std::string> var_list()const
//		{
//			return vL;
//		}
//
//	private:
//
//		constexpr void upgrade_poly(MultivariatePolynomial& p1, MultivariatePolynomial& p2)const noexcept
//		{
//			//std::cout << "Upgrade poly cost\n";
//
//			Array<std::string> TmpVarlist = p1.vL;
//
//			_Sz n1 = p1.vL.size(), n2 = p2.vL.size(), i, j;
//
//			Array<_Sz> TmpPosition; _Sz TmpIndex = n1;
//
//			// Code for upgrade variable list
//
//			for (j = 0; j < n2; ++j)
//			{
//				for (i = 0; i < n1; ++i)
//				{
//					if (p1.vL[i] == p2.vL[j])
//					{
//						TmpPosition.push_back(i);
//
//						break;
//					}
//				}
//
//				if (i == n1)
//				{
//					TmpPosition.push_back(TmpIndex++);
//
//					TmpVarlist.push_back(p2.vL[j]);
//				}
//			}
//
//			// Code for upgrade P1 Polynomial
//
//			_Sz n3 = TmpVarlist.size() - p1.vL.size();
//
//			for (auto& pair_ : p1.fx)
//			{
//				for (_Sz i = 0; i < n3; ++i)
//				{
//					pair_.expo.push_back(0);
//				}
//			}
//
//			p1.vL = TmpVarlist;
//
//			// Code for upgrade P2 Polynomial
//
//			Array<_Sz> TmpExpo(TmpVarlist.size(), 0);
//
//			for (auto& pair_ : p2.fx)
//			{
//				for (_Sz i = 0; i < pair_.expo.size(); ++i)
//				{
//					TmpExpo[TmpPosition[i]] = pair_.expo[i];
//				}
//
//				pair_.expo = TmpExpo;
//
//				for (auto& e : TmpExpo)
//				{
//					e = 0;  // reset TmpExpo;
//				}
//			}
//
//			p2.vL = TmpVarlist;
//
//			return;
//		}
//
//	public:
//
//		/*constexpr void printExpo(const dynamic_array<int64>& x)const noexcept
//		{
//			int64 index = 0;
//
//			for (const auto& i : x)
//			{
//				if (i != 0)
//					if (i != 1)
//						out('(', vL[index], ')', '^', i);
//					else out('(', vL[index], ')');
//				index++;
//			}
//		}*/
//
//		/*constexpr void printdata()const noexcept
//		{
//			std::cout << "\nVarible List : ";
//
//			for (const auto& str : vL)
//				std::cout << str << " ";
//
//			std::cout << "\nF(";
//
//			for (const auto& var : vL)
//				std::cout <<'(' << var <<')' << " ";
//
//			std::cout << ") = ";
//
//			for (const auto& pair : fx)
//				std::cout << " + (" << pair.coeff << ")",
//				printExpo(pair.expo);
//		}*/
//
//		/*constexpr void print_obj(const bool yes = true)const noexcept
//		{
//			out("\nF(");
//			if (yes)
//				for (const auto& var : vL)
//				{
//					out('(', var, ") ");
//				}
//
//			out(") = ");
//
//			for (const auto& pair : fx)
//			{
//				if (pair.coeff == 0ll) continue;
//				out('\n');
//				out(" + (" , pair.coeff, ")");
//				printExpo(pair.expo);
//			}
//
//		}*/
//
//		void print(std::ofstream& file) {
//			file << vL.size() << '\n';
//			for (const auto& var : vL)
//				file << var << " ";
//			file << fx.size();
//			for (auto& nodex : fx) {
//				file << "\n" << nodex.coeff << "\n";
//
//				for (auto& i : nodex.expo) {
//					file << i << " ";
//				}
//			}
//
//		}
//		void scan(std::ifstream& file) {
//			_Sz size;
//			file >> size;
//			vL = size;
//			for (_Sz i = 0; i < size; ++i) {
//				file >> vL[i];
//			}
//			_Sz size_list;
//			file >> size_list;
//			fx.clear();
//			integer Tmp_int; Array<_Sz> Tmp_arr(size);
//			for (_Sz i = 0; i < size_list; ++i) {
//				file >> Tmp_int;
//				for (_Sz j = 0; j < size; ++j) {
//					file >> Tmp_arr[j];
//				}
//				fx.emplace_back(Tmp_int, Tmp_arr);
//			}
//		}
//
//		constexpr void printExpo(const Array<_Sz>& _Val)const noexcept
//		{
//			_Sz index = 0;
//
//			for (const auto& i : _Val)
//			{
//				if (i != 0)
//					if (i != 1)
//						std::cout << vL[index] << '^' << i;
//					else std::cout << vL[index];
//				index++;
//			}
//		}
//
//		constexpr void printdata()const noexcept
//		{
//			std::cout << "\nVarible List : ";
//
//			for (const auto& str : vL)
//				std::cout << str << " ";
//
//			std::cout << "\nF(";
//
//			for (const auto& var : vL)
//				std::cout << var << " ";
//
//			std::cout << ") = ";
//
//			for (const auto& pair : fx)
//				std::cout << " + (" << pair.coeff << ")",
//				printExpo(pair.expo);
//		}
//
//		constexpr void print(const bool yes = true)noexcept
//		{
//			std::cout << "\nF(";
//			if (yes)
//				for (const auto& var : vL)
//					std::cout << var << " ";
//
//			std::cout << ") = ";
//
//			for (auto& pair : fx)
//				std::cout << " + (" << pair.coeff << ")",
//				printExpo(pair.expo);
//		}
//
//		/*
//		constexpr void print(std::ostream& file)const {
//
//			for (const auto& var : vL)
//				cipher_file << var << " ";
//			cipher_file << "\n";
//
//			for (const auto& node : fx)
//			{
//				out("\n", coeff, "\n");
//
//				for (const auto& i : expo)
//				{
//					out(i, ' ');
//				}
//			}
//
//		}
//		constexpr void print(std::ostream& file)const {
//
//			for (const auto& var : vL)
//				cipher_file << var << " ";
//			cipher_file << "\n";
//
//			for (const auto& node : fx)
//			{
//				out("\n", coeff, "\n");
//
//				for (const auto& i : expo)
//				{
//					out(i, ' ');
//				}
//			}
//
//		}*/
//
//		private:
//			struct _Node {
//			public:
//				_Ty _Coeff;
//				Array<_Sz> _Expo;
//
//				constexpr _Node(const _Sz Size)noexcept {
//					_Expo.reserve(Size);
//				}
//				constexpr _Node(const _Ty& _Coeff, const Array<_Sz>& _Expo)noexcept : _Coeff(_Coeff), _Expo(_Expo){}
//
//				constexpr void print()const {
//					alpha::print("[", _Coeff, ",(");
//					for (const auto& i : _Expo)
//						alpha::print(i, ' ');
//					alpha::print(")]");
//				}
//
//				[[nodiscard]] constexpr bool operator <  (const _Node& that)const noexcept {
//					_Sz n = static_cast<_Sz>(_Expo.size());
//
//					const _Sz* expo_ptr = this->_Expo.data();
//					const _Sz* xexpo_ptr = that._Expo.data();
//
//					for (_Sz i = 0; i < n; ++i) {
//						if (expo_ptr[i] == xexpo_ptr[i]) continue;
//						if (expo_ptr[i] > xexpo_ptr[i]) return true;
//						return false;
//					}
//					return false;
//				}
//
//				[[nodiscard]] constexpr bool operator == (const _Node& _Val)const noexcept {
//					_Sz _Size = static_cast<_Sz>(_Expo.size());
//
//					if (_Size != _Val._Expo.size()) return false;
//
//					const _Sz* expo_ptr = this->_Expo.data();
//					const _Sz* xexpo_ptr = _Val._Expo.data();
//
//					for (_Sz i = 0; i < _Size; ++i) {
//						if (expo_ptr[i] != xexpo_ptr[i])
//							return false;
//					}
//					return true;
//				}
//
//				[[nodiscard]] constexpr _Node operator +  (const _Node& _Val)const noexcept {
//					if constexpr (_debug) {
//						if (*this != _Val) __debugbreak();
//					}
//					return { _Coeff + _Val._Coeff, _Expo };
//				}
//
//				[[nodiscard]] constexpr _Node operator *  (const _Node& _Val)const noexcept {
//					_Sz size = static_cast<_Sz>(_Expo.size());
//					_Node Tmp; Tmp._Expo = size;
//
//					const _Sz* expo_ptr = this->_Expo.data();
//					const _Sz* xexpo_ptr = _Val._Expo.data();
//
//					_Sz* Tmpexpo_ptr = Tmp._Expo.data();
//
//					Tmp._Coeff = this->_Coeff * _Val._Coeff;
//
//					for (_Sz i = 0; i < size; ++i)
//					{
//						*(Tmpexpo_ptr + i) = *(expo_ptr + i) + *(xexpo_ptr + i);
//					}
//
//					return Tmp;
//				}
//
//				constexpr _Node& operator += (const _Node& _Val)noexcept
//				{
//					_Coeff += _Val._Coeff;  // both pair's expo must be same
//
//					return *this;
//				}
//			};
//
//			Array<string> vL; // variable list
//
//			std::list<node> fx; // polynomial list representation
//	};
//
//
//	template<class _Ty> std::ofstream& operator << (std::ofstream& file, MultivariatePolynomial<_Ty>& _Val) {
//		_Val.print(file);
//		return file;
//	}
//	template<class _Ty> std::ifstream& operator >> (std::ifstream& file, MultivariatePolynomial<_Ty>& _Val) {
//		_Val.scan(file);
//		return file;
//	}
//	template<class _Ty> std::ostream& operator << (std::ostream& cout, MultivariatePolynomial<_Ty>& p)noexcept
//	{
//		p.print(false); return cout;
//	}
//}


#endif