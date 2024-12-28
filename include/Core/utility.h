#pragma once
#ifndef __UTILITY__
#define __UTILITY__

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>
#include <initializer_list>
#include <typeinfo>

namespace alpha {
    // From <type_traits>
	using ::std::enable_if;
	using ::std::enable_if_t;
    using ::std::conditional;
    using ::std::conditional_t;
    using ::std::is_same;
    using ::std::is_same_v;
    using ::std::is_integral;
    using ::std::is_integral_v;
    using ::std::is_floating_point;
    using ::std::is_floating_point_v;
    using ::std::is_arithmetic;
    using ::std::is_arithmetic_v;
    using ::std::is_void;
    using ::std::is_void_v;
    using ::std::is_pointer;
    using ::std::is_pointer_v;
    using ::std::is_null_pointer;
    using ::std::is_null_pointer_v;
    using ::std::is_fundamental;
    using ::std::is_fundamental_v;
    using ::std::true_type;
    using ::std::false_type;
    using ::std::is_reference;
    using ::std::is_reference_v;
    using ::std::is_lvalue_reference;
    using ::std::is_lvalue_reference_v;
    using ::std::remove_reference;
    using ::std::remove_reference_t;
    using ::std::remove_cv;
    using ::std::remove_cv_t;
    using ::std::forward;
    using ::std::move;
    using ::std::decay;
    using ::std::decay_t;
    using ::std::enable_if;
    using ::std::enable_if_t;
    using ::std::add_rvalue_reference;
    using ::std::declval;
    using ::std::is_trivially_destructible;
    using ::std::is_trivially_destructible_v;
    using ::std::aligned_storage;
    using ::std::aligned_union;
    using ::std::common_type;
    using ::std::common_type_t;
    using ::std::underlying_type;
    using ::std::underlying_type_t;
    using ::std::invoke_result;
    using ::std::invoke_result_t;
    using ::std::is_convertible;
    using ::std::is_convertible_v;
    using ::std::is_base_of;
    using ::std::is_base_of_v;
    using ::std::is_trivial;
    using ::std::is_trivially_copyable;
    using ::std::is_standard_layout;
    using ::std::is_default_constructible;
    using ::std::is_trivially_default_constructible;
    using ::std::is_nothrow_default_constructible;
    using ::std::is_copy_constructible;
    using ::std::is_trivially_copy_constructible;
    using ::std::is_nothrow_copy_constructible;
    using ::std::is_move_constructible;
    using ::std::is_trivially_move_constructible;
    using ::std::is_nothrow_move_constructible;
    using ::std::is_assignable;
    using ::std::is_trivially_assignable;
    using ::std::is_nothrow_assignable;
    using ::std::is_copy_assignable;
    using ::std::is_trivially_copy_assignable;
    using ::std::is_nothrow_copy_assignable;
    using ::std::is_move_assignable;
    using ::std::is_trivially_move_assignable;
    using ::std::is_nothrow_move_assignable;
    using ::std::is_destructible;
    using ::std::is_trivially_destructible;
    using ::std::is_nothrow_destructible;
    using ::std::has_virtual_destructor;
    using ::std::alignment_of;
    using ::std::extent;
    using ::std::rank;
    using ::std::remove_extent;
    using ::std::remove_all_extents;
    using ::std::type_info;
    using ::std::bad_typeid;

    // Detection idiom utilities (C++17 and beyond)
    using ::std::conjunction;
    using ::std::disjunction;
    using ::std::negation;

    // From <utility>
	using ::std::declval;
    using ::std::pair;
    using ::std::make_pair;
    using ::std::tuple;
    using ::std::move;
    using ::std::forward;
    using ::std::swap;
    using ::std::exchange;
    using ::std::integer_sequence;
    using ::std::index_sequence;
    using ::std::make_integer_sequence;
    using ::std::make_index_sequence;
    using ::std::index_sequence_for;
    using ::std::declval;
    using ::std::get;

	template<class _Ty>
	struct less {
		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
		bool operator()(ConstReference _Right, ConstReference _Left) {
			return _Right < _Left;
		}

	};
	template<class _Ty>
	struct greater {
		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
		bool operator()(ConstReference _Right, ConstReference _Left) {
			return _Right > _Left;
		}
	};

	template<class _Ty>
	inline constexpr _Ty& maximum(_Ty& a, _Ty& b) {
		return a < b ? b : a;
	}
	template<class _Ty, class _Comparator>
	inline constexpr _Ty& maximum(_Ty& a, _Ty& b, _Comparator comp) {
		return comp(a, b) ? b : a;
	}

	template <typename U>
	inline constexpr auto _Unwrappable(int) -> decltype(alpha::declval<U>()._Unwrapped(), bool{}) {
		return true;
	}
	template <typename U>
	inline constexpr auto _Orderable(int) -> decltype(alpha::declval<U>().operator>(U()), bool{}) {
		return true;
	}

	template <typename U>
	inline constexpr bool _Unwrappable(...) {
		return false;
	}
	template <typename U>
	inline constexpr bool _Orderable(...) {
		return false;
	}

	template <typename _Ty>
	inline constexpr bool _Unwrappable_v = _Unwrappable<_Ty>(0);
	template <typename _Ty>
	inline constexpr bool _Orderable_v = _Orderable<_Ty>(0);

	template <class _Iter>
	[[nodiscard]] constexpr decltype(auto) _GetUnwrapped(_Iter&& _It) noexcept {
		if constexpr (is_pointer<decay_t<_Iter>>::value) {
			return _It + 0;
		}
		else if constexpr (_Unwrappable_v<_Iter>) {
			return static_cast<_Iter&&>(_It)._Unwrapped();
		}
		else {
			return static_cast<_Iter&&>(_It);
		}
	}
}

namespace alpha {
	 using :: div_t;
	 using :: ldiv_t;
	 using :: abort;
	 using :: abs;
	 using :: atexit;
	 using :: atof;
	 using :: atoi;
	 using :: atol;
	 using :: bsearch;
	 using :: calloc;
	 using :: div;
	 using :: exit;
	 using :: free;
	 using :: labs;
	 using :: ldiv;
	 using :: malloc;
	 using :: mblen;
	 using :: mbstowcs;
	 using :: mbtowc;
	 using :: qsort;
	 //using :: rand;
	 using :: realloc;
	 //using :: srand;
	 using :: strtod;
	 using :: strtol;
	 using :: strtoul;
	 using :: wcstombs;
	 using :: wctomb;
	 using :: lldiv_t;
	 using :: getenv;
	 using :: system;
	 using :: atoll;
	 using :: llabs;
	 using :: lldiv;
	 using :: strtof;
	 using :: strtold;
	 using :: strtoll;
	 using :: strtoull;
	 using :: _Exit;
	 using :: at_quick_exit;
	 using :: quick_exit;
	 using :: memchr;
	 using :: memcmp;
	 using :: memcpy;
	 using :: memmove;
	 using :: memset;
	 using :: strcat;
	 using :: strchr;
	 using :: strcmp;
	 using :: strcoll;
	 using :: strcpy;
	 using :: strcspn;
	 using :: strerror;
	 using :: strlen;
	 using :: strncat;
	 using :: strncmp;
	 using :: strncpy;
	 using :: strpbrk;
	 using :: strrchr;
	 using :: strspn;
	 using :: strstr;
	 using :: strtok;
	 using :: strxfrm;
}

namespace alpha {
	using ::std::align_val_t;
	using ::std::nothrow_t;
	using ::std::nothrow;

	using ::operator new;
	using ::operator new[];
	using ::operator delete;
	using ::operator delete[];

#ifdef __cpp_aligned_new
	using ::std::align_val_t;
	using ::operator new;
	using ::operator new[];
	using ::operator delete;
	using ::operator delete[];
#endif

	template<class _Ty, class... _Args>
	inline void construct(_Ty* _Ptr, _Args&&... _Val) {
		new(_Ptr) _Ty(forward<_Args>(_Val)...);
	}

}
namespace alpha {
	template<class _Ty> using initializer = std::initializer_list<_Ty>;
	template <class _Ty> const char* type_name = typeid(_Ty).name();
}

#endif