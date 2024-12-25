#pragma once
#ifndef __DYNAMIC_ARRAY__
#define __DYNAMIC_ARRAY__

#include "../Core/core.h"
#include "../Io/io.h"
#include "../Core/utility.h"
#include "../Memory/memory.h"
#include "simd.h"

typedef unsigned long long size_t;

namespace alpha {
	template<class Array>
	class ConstArrayIterator {
	public:
		using _Ty = Array::ArgType;
		using _Ref = const _Ty&;
	public:
		constexpr ConstArrayIterator(_Ty* _Ptr)noexcept : _Ptr(_Ptr) {}
		[[nodiscard]] constexpr _Ref operator*()const noexcept {
			return *_Ptr;
		}
		[[nodiscard]] constexpr const _Ty* operator->()const noexcept {
			return _Ptr;
		}
		constexpr ConstArrayIterator& operator++()noexcept {
			++_Ptr;
			return *this;
		}
		constexpr ConstArrayIterator operator++(int) noexcept {
			ConstArrayIterator iterator = *this;
			++(*this);
			return iterator;
		}
		constexpr ConstArrayIterator& operator--() noexcept {
			--_Ptr;
			return *this;
		}
		constexpr ConstArrayIterator operator--(int) noexcept {
			ConstArrayIterator iterator = *this;
			--(*this);
			return iterator;
		}
		constexpr ConstArrayIterator& operator+=(size_t _Off) noexcept {
			_Ptr += _Off;
			return *this;
		}
		[[nodiscard]] constexpr ConstArrayIterator operator+(size_t _Off) noexcept {
			ConstArrayIterator _Tmp = *this;
			_Tmp += _Off;
			return _Tmp;
		}
		constexpr ConstArrayIterator& operator-=(size_t _Off) noexcept {
			_Ptr -= _Off;
			return *this;
		}
		[[nodiscard]] constexpr ConstArrayIterator operator-(size_t _Off) noexcept {
			ConstArrayIterator _Tmp = *this;
			_Tmp -= _Off;
			return _Tmp;
		}
		[[nodiscard]] constexpr size_t operator-(ConstArrayIterator _That)noexcept {
			return _Ptr - _That._Ptr;
		}
		[[nodiscard]] constexpr _Ref operator[] (size_t index)const noexcept {
			return *(_Ptr + index);
		}
		[[nodiscard]] constexpr bool operator==(const ConstArrayIterator& that)const {
			return _Ptr == that._Ptr;
		}
		[[nodiscard]] constexpr bool operator!=(const ConstArrayIterator& that)const {
			return _Ptr != that._Ptr;
		}
		[[nodiscard]] constexpr const _Ty* _Unwrapped() const noexcept {
			return this->_Ptr;
		}
		_Ty* _Ptr;
	};

	template<class Array>
	class ArrayIterator : public ConstArrayIterator<Array> {
	public:
		using _Mybase = ConstArrayIterator<Array>;
		using _Ty = typename Array::ArgType;
		using reference = _Ty&;
		using pointer = _Ty*;
	public:
		[[nodiscard]] constexpr reference operator*() const noexcept {
			return const_cast<reference>(_Mybase::operator*());
		}

		[[nodiscard]] constexpr pointer operator->() const noexcept {
			return this->_Ptr;
		}

		constexpr ArrayIterator& operator++() noexcept {
			_Mybase::operator++();
			return *this;
		}

		constexpr ArrayIterator operator++(int) noexcept {
			ArrayIterator _Tmp = *this;
			_Mybase::operator++();
			return _Tmp;
		}

		constexpr ArrayIterator& operator--() noexcept {
			_Mybase::operator--();
			return *this;
		}

		constexpr ArrayIterator operator--(int) noexcept {
			ArrayIterator _Tmp = *this;
			_Mybase::operator--();
			return _Tmp;
		}

		constexpr ArrayIterator& operator+=(const size_t _Off) noexcept {
			_Mybase::operator+=(_Off);
			return *this;
		}

		[[nodiscard]] constexpr ArrayIterator operator+(const size_t _Off) const noexcept {
			ArrayIterator _Tmp = *this;
			_Tmp += _Off;
			return _Tmp;
		}

		constexpr ArrayIterator& operator-=(const size_t _Off) noexcept {
			_Mybase::operator-=(_Off);
			return *this;
		}

		using _Mybase::operator-;

		[[nodiscard]] constexpr ArrayIterator operator-(const size_t _Off) const noexcept {
			ArrayIterator _Tmp = *this;
			_Tmp -= _Off;
			return _Tmp;
		}

		[[nodiscard]] constexpr reference operator[](const size_t _Off) const noexcept {
			return const_cast<reference>(_Mybase::operator[](_Off));
		}


		[[nodiscard]] constexpr _Ty* _Unwrapped() const noexcept {
			return this->_Ptr;
		}
	};

	template<class _Ty, class Allocator = allocator<_Ty>>
	class Array {
	public:
		using ArgType = _Ty;
		using Iterator = ArrayIterator<Array<_Ty>>;
		using ConstIterator= ConstArrayIterator<Array<_Ty>>;
		using Pointer = _Ty*;
		using Reference = _Ty&;
		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
		using ConstPointer = const _Ty*;
		using ValueType = _Ty;
		using SizeType = size_t;

		explicit constexpr Array(const Allocator& allocator = Allocator())noexcept : _Siz(0), _Cap(0), _Ptr(nullptr), _Allocator(allocator) {}

		explicit constexpr Array(const SizeType _Size, ConstReference _Val = _Ty(), const Allocator& allocator = Allocator())noexcept : _Siz(_Size), _Allocator(allocator) {
			_Allocate(_Size);
			_Memset(_Ptr, _Ptr + _Siz, _Val);
		}

		constexpr Array(const initializer<_Ty>& list)noexcept : _Siz(list.size()) {
			_Allocate(_Siz);
			Pointer _pointer = _Ptr;
			for (const auto& _Val : list)
				construct(_pointer++, _Val);
		}

		constexpr Array(const Array& _That)noexcept : _Allocator(_That._Allocator) {
			_Allocate(_That._Siz);
			_ConstructCopy(_That);
		}

		constexpr Array(Array&& _That)noexcept {
			_Move(_That);
		}
		
		constexpr ~Array()noexcept {
			if (_Ptr != nullptr)
				_Free();
		}

		constexpr Array& operator=(const SizeType _Size)noexcept = delete;

		constexpr Array& operator=(const Array& _That)noexcept {
			if (this != &_That) {
				if (_Cap < _That._Siz)
					_ReallocateNoCopy(_That._Siz);
				_Siz = _That._Siz;
				_Copy(_That);
			}
			return *this;
		};

		constexpr Array& operator=(const initializer<_Ty>& list)noexcept {
			const auto _NewSiz = list.size();
			if (_NewSiz > _Cap)
				_ReallocateNoCopy(_NewSiz);
			_Siz = _NewSiz;
			Pointer _pointer = _Ptr;
			for (const auto& _Val : list)
				_Allocator.construct(_pointer++, _Val);
			return *this;
		};

		constexpr Array& operator=(Array&& _That)noexcept {
			if (this != &_That) {
				_Free();
				_Move(_That);
			}
			return *this;
		}

		[[nodiscard]] constexpr bool operator==(const Array& _That)noexcept {
			if (_Siz != _That._Siz) return false;
			for (SizeType i = 0; i < _Siz; ++i) {
				if (_Ptr[i] != _That._Ptr[i]) return false;
			}
			return true;
		}
		
		[[nodiscard]] constexpr bool operator!=(const Array& _That)noexcept {
			return !(*this == _That);
		}

		constexpr const Reference operator[](const SizeType _Idx)const {
			if constexpr (_debug) {
				if(_Idx >= _Siz)
					__debugbreak();
			}
			return _Ptr[_Idx];
		}

		constexpr Reference operator[](const SizeType _Idx) {
			if constexpr (_debug) {
				if(_Idx >= _Siz)
					__debugbreak();
			}
			return _Ptr[_Idx];
		}

		constexpr void push_back(ConstReference _Arg3)noexcept {
			if (_Siz >= _Cap)
				_Reallocate(_Siz);
			if constexpr (is_fundamental<_Ty>::value)
				_Ptr[_Siz] = _Arg3;
			else construct(_Ptr + _Siz, _Arg3);
			++_Siz;
		}

		constexpr void push_back(_Ty&& _Arg3)noexcept {
			if (_Siz >= _Cap)
				_Reallocate(_Siz);
			if constexpr (is_fundamental<_Ty>::value || is_pointer<_Ty>::value)
				_Ptr[_Siz] = _Arg3;
			else _Allocator.construct(_Ptr + _Siz, static_cast<remove_reference_t<_Ty>&&>(_Arg3));
			++_Siz;
		}

		template<class... _Args>
		constexpr void emplace_back(_Args&&... _Val) {
			if (_Siz >= _Cap)
				_Reallocate(_Siz);
			construct(_Ptr + _Siz, forward<_Args>(_Val)...);
			++_Siz;
		}

		template<class... _Args>
		constexpr void emplace(size_t _Pos, _Args&&... _Val) {
			if constexpr (_debug)
				if (_Pos > _Siz)
					__debugbreak();

			if (_Pos == _Siz) {
				this->emplace_back(forward<_Args>(_Val)...);
				return;
			}

			_RightShift(_Pos, 1);
			construct(_Ptr + _Pos , forward<_Args>(_Val)...);
		}

		constexpr void pop_back()noexcept {
			if (_Siz > 0) {
				--_Siz;
				if constexpr (!is_trivially_destructible_v<_Ty>)
					(_Ptr + _Siz)->~_Ty();
			}
		}

		virtual constexpr Array& operator+=(ConstReference _Arg)noexcept {
			push_back(_Arg);
			return *this;
		}

		virtual constexpr Array& operator+=(_Ty&& _Arg)noexcept {
			push_back(_Arg);
			return *this;
		}

		virtual constexpr Array& operator+=(const Array& _That)noexcept {
			const auto _NewSiz = _Siz + _That._Siz;
			if (_NewSiz > _Cap)
				_Reallocate(_NewSiz);
			_ConstructCopy(_Ptr + _Siz, _That._Ptr, _That._Siz);
			_Siz = _NewSiz;
			return *this;
		}

		virtual constexpr Array& operator*=(const SizeType _Int)noexcept {
			const auto _NewSiz = _Int * _Siz;
			const auto _Range = _NewSiz >> 1;
			if (_NewSiz > _Cap)
				_Reallocate(_NewSiz);
			if constexpr (!is_fundamental_v<_Ty>)
				_Memset(_Ptr + _Siz, _Ptr + _NewSiz, _Ty());
			for (; _Siz <= _Range; _Siz <<= 1)
				_Copy(_Ptr + _Siz, _Ptr, _Siz);
			if (_NewSiz > _Siz)
				_Copy(_Ptr + _Siz, _Ptr, _NewSiz - _Siz);
			_Siz = _NewSiz;
			return *this;
		}

		[[nodiscard]] virtual constexpr Array operator+(ConstReference _Arg)const noexcept {
			Array _Tmp(*this);
			_Tmp.push_back(_Arg);
			return _Tmp;
		}

		[[nodiscard]] virtual constexpr Array operator+(const Array& _That)const noexcept {
			const auto _NewSiz = _Siz + _That._Siz;
			Array _Tmp;  _Tmp._Allocate(_NewSiz);
			_ConstructCopy(_Tmp._Ptr, _Ptr, _Siz);
			_ConstructCopy(_Tmp._Ptr + _Siz, _That._Ptr, _That._Siz);
			_Tmp._Siz = _NewSiz;
			return _Tmp;
		}


		// virtual constexpr void print(FILE* file)const noexcept {
		// 	_print("\n[", file);
		// 	for (SizeType i = 0; i < _Siz; ++i) {
		// 		_print(' ', file);
		// 		_printL(_Ptr[i], file);
		// 	}
		// 	_print(" ]", file);
		// }

		[[nodiscard]] constexpr SizeType size()const {
			return _Siz;
		}

		[[nodiscard]] constexpr Pointer data() noexcept {
			return _Ptr;
		}

		[[nodiscard]] constexpr const Pointer data() const noexcept {
			return _Ptr;
		}

		[[nodiscard]] constexpr SizeType capacity()const {
			return _Cap;
		}

		[[nodiscard]] constexpr Iterator begin()noexcept {
			return Iterator(_Ptr);
		}

		[[nodiscard]] constexpr Iterator end()noexcept {
			return Iterator(_Ptr + _Siz);
		}

		[[nodiscard]] constexpr ConstIterator begin()const noexcept {
			return ConstIterator(_Ptr);
		}

		[[nodiscard]] constexpr ConstIterator end()const noexcept {
			return ConstIterator(_Ptr + _Siz);
		}

		constexpr void reserve(const SizeType _Siz) {
			if (_Siz <= _Cap) return;
			_Reallocate(_Siz);
		}

		constexpr void resize(const SizeType _Size, ConstReference _Val = _Ty()) {
			if(_Size != _Cap)
				_Reallocate(_Size);
			if (_Size > _Siz)
				_Memset(_Ptr + _Siz, _Ptr + _Size, _Val);
			_Siz = _Size;
		}

		constexpr Array& insert(const SizeType _Pos, ConstReference _Val)noexcept{
			if constexpr (_debug)
				if (_Pos > _Siz) __debugbreak();
			_RightShift(_Pos, 1);
			_Allocator.construct(_Ptr + _Pos, _Val);
			return *this;
		}

		constexpr Array& insert(const SizeType _Pos, Array& _That)noexcept {
			if constexpr (_debug)
				if (_Pos > _Siz || this == &_That) __debugbreak();
			_RightShift(_Pos, _Siz);
			_ConstructCopy(_Ptr + _Pos, _That._Ptr, _That._Siz);
			return *this;
		}

		constexpr Array& selfInsert(const SizeType _Pos)noexcept {
			if constexpr (_debug)
				if (_Pos > _Siz) __debugbreak();
			const auto _OldSiz = _Siz;
			_RightShift(_Pos, _Siz);
			_ConstructCopy(_Ptr + _Pos, _Ptr, _Pos);
			_ConstructCopy(_Ptr + _Pos + _Pos, _Ptr + _Pos + _OldSiz, _OldSiz - _Pos);
			return *this;
		}

		constexpr Array& insert(const SizeType _Pos, const SizeType _Len, ConstReference _Val)noexcept {
			if constexpr (_debug)
				if (_Pos > _Siz) __debugbreak();
			_RightShift(_Pos, _Len);
			_Memset(_Ptr + _Pos, _Ptr + _Pos + _Len, _Val);
			return *this;
		}

		constexpr Array& insert(const SizeType _Pos, const Array& _That, const SizeType _Subpos, const SizeType _Sublen)noexcept {
			if constexpr (_debug) {
				if (_Subpos + _Sublen > _That._Siz || _Pos > _Siz || this == &_That)
					__debugbreak();
			}
			_RightShift(_Pos, _Sublen);
			_ConstructCopy(_Ptr + _Pos, _That._Ptr + _Subpos, _Sublen);
			return *this;
		}

		constexpr void swap(Array& _Arg3)noexcept {
			if (this != &_Arg3) {
				_Ty*   Tmp_ptr = _Ptr;
				size_t Tmp_siz = _Siz;
				size_t Tmp_cap = _Cap;

				_Ptr = _Arg3._Ptr;
				_Siz = _Arg3._Siz;
				_Cap = _Arg3._Cap;

				_Arg3._Ptr = Tmp_ptr;
				_Arg3._Siz = Tmp_siz;
				_Arg3._Cap = Tmp_cap;
			}
		} 

		constexpr void erase(const SizeType _Pos)noexcept {
			_LeftShift(_Pos, 1);
		}

		constexpr void erase(ConstIterator _It)noexcept {
			_LeftShift(_It - ConstIterator(_Ptr),1);
		}

	protected:
		inline constexpr auto _RecommendCapacity(const SizeType _Size)noexcept {
			SizeType size = 8;
			while (size <= _Size) {
				size <<= 1;
			}
			return size;
		}

		inline constexpr auto _AllocateExactly(const SizeType _Size)noexcept {

		}

		inline constexpr auto _Allocate(const SizeType _Size)noexcept {
			_Cap = _RecommendCapacity(_Size);
			_Ptr = _Allocator.allocate(_Cap);
		}

		inline constexpr auto _ReallocateExactly(const SizeType _Size)noexcept {

		}

		inline constexpr auto _Reallocate(const SizeType _Size)noexcept {
			const auto  _NewCap = _RecommendCapacity(_Size);
			auto* _NewPtr = _Allocator.allocate(_NewCap);

			if (_Ptr != nullptr) {
				auto _NewSiz = _Siz;
				if (_Size < _Siz) {
					if constexpr (!is_trivially_destructible<_Ty>::value)
						_DestroyRange(_Ptr + _Size, _Ptr + _Siz);
					_NewSiz = _Size;
				}
				memcpy(_NewPtr, _Ptr, _NewSiz * sizeof(_Ty));
				_Allocator.deallocate(_Ptr, _Cap);
			}

			_Ptr = _NewPtr;
			_Cap = _NewCap;
		}

		inline constexpr auto _ReallocateNoCopy(SizeType _Size)noexcept {
			const auto  _NewCap = _RecommendCapacity(_Size);
			auto* _NewPtr = _Allocator.allocate(_NewCap);

			if (_Size < _Siz) {
				if constexpr (!is_trivially_destructible<_Ty>::value)
					_DestroyRange(_Ptr + _Size, _Ptr + _Siz);
			}

			_Allocator.deallocate(_Ptr, _Cap);
			_Ptr = _NewPtr;
			_Cap = _NewCap;
		}

		inline constexpr auto _Free()noexcept {
			if constexpr (!is_trivially_destructible<_Ty>::value)
				_DestroyRange(_Ptr, _Ptr + _Siz);
			_Allocator.deallocate(_Ptr, _Cap);
			_Ptr = nullptr;
			_Siz = 0;
			_Cap = 0;
		}

		inline constexpr auto _Memset(Pointer _Start, Pointer _End, ConstReference _Val)noexcept {
			if constexpr (is_fundamental<_Ty>::value) {
				if constexpr (is_same<_Ty, char>::value) {
					alpha::memset(_Start, _Val, _End - _Start);
				} else {
					if (_Val != 0) {
						while (_Start != _End)
							*(_Start++) = _Val;
					} else { alpha::memset(_Start, 0, (_End  - _Start) * sizeof(_Ty));  }
				}
			} else {
				while(_Start != _End)
					construct(_Start++ ,_Val);
			}
		}

		static inline constexpr auto _ConstructCopy(Pointer _Dst, ConstPointer _Src, SizeType _Size)noexcept{
			if constexpr (is_fundamental<_Ty>::value || is_pointer<_Ty>::value)
				alpha::memcpy(_Dst, _Src, _Size * sizeof(_Ty));
			else
				for (size_t _idx = 0; _idx < _Size; ++_idx)
					construct(_Dst + _idx, _Src[_idx]);
		}

		inline constexpr auto _ConstructCopy(const Array& _That)noexcept {
			_Siz = _That._Siz;
			_ConstructCopy(_Ptr, _That._Ptr, _That._Siz);
		}

		inline constexpr auto _DestroyRange(Pointer _Start, Pointer _End)noexcept {
			while (_Start != _End) {
				_Start->~_Ty();
				++_Start;
			}
		}

		inline constexpr void _Move(Array& _That)noexcept {
			_Ptr = _That._Ptr;
			_Siz = _That._Siz;
			_Cap = _That._Cap;
			_Allocator = _That._Allocator;

			_That._Ptr = nullptr;
			_That._Siz = 0;
			_That._Cap = 0;
		}

		static inline constexpr auto _Copy(Pointer _Dst, ConstPointer _Src, SizeType _Size)noexcept {
			if constexpr (is_fundamental<_Ty>::value || is_pointer<_Ty>::value) {
				alpha::memcpy(_Dst, _Src, _Size * sizeof(_Ty));
			} else {
				for (size_t _idx = 0; _idx < _Size; ++_idx) {
					_Dst[_idx] = _Src[_idx];
				}
			}
		}

		inline constexpr auto _Copy(const Array& _That)noexcept {
			_Siz = _That._Siz;
			_Copy(_Ptr, _That._Ptr, _That._Siz);
		}

		inline constexpr auto _RightShift(const SizeType _Pos, const SizeType _Count)noexcept {
			SizeType _NewSiz = _Siz + _Count;
			if (_NewSiz > _Cap) {
				auto _NewCap = _RecommendCapacity(_NewSiz);
				auto _NewPtr = _Allocator.allocate(_NewCap);
				memcpy(_NewPtr, _Ptr, _Pos * sizeof(_Ty));
				memcpy(_NewPtr + _Pos + _Count, _Ptr + _Pos, (_Siz - _Pos) * sizeof(_Ty));
				_Allocator.deallocate(_Ptr, _Cap);
				_Ptr = _NewPtr; _Cap = _NewCap;
			} else {
				memmove(_Ptr + _Pos + _Count, _Ptr + _Pos, (_Siz - _Pos) * sizeof(_Ty));
			}
			_Siz = _NewSiz;
		}

		inline constexpr auto _LeftShift(const size_t _Pos, const size_t _Count)noexcept {
			size_t _NewSiz = _Siz - _Count;
			for (size_t i = _Pos + 1; i < _Siz; ++i)
				// you can check here for template type is movable or not.
				_Ptr[i - _Count] = move(_Ptr[i]);
			if constexpr (!is_trivially_destructible<_Ty>::value) {
				for (size_t i = _NewSiz; i < _Siz; ++i)
					_Ptr[i].~_Ty();
			}
			_Siz = _NewSiz;
		}

		Pointer	 _Ptr;
		SizeType _Siz;
		SizeType _Cap;
		Allocator _Allocator;
	};
	
	// template<class _Ty, class _Allocator>
	// void _print(const Array<_Ty, _Allocator>& _Obj)noexcept {
	// 	_Obj.print(stdout);
	// }

	template<class _Ty, class _Allocator = allocator<_Ty>> using LinearContainer = Array<_Ty, _Allocator>;

}

#endif








