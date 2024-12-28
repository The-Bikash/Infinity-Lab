#pragma once
#ifndef __MEMORY__
#define __MEMORY__

#include "../Core/core.h"
#include "../Core/utility.h"
#include "xmemory.h"

namespace alpha{
    template<class _Ty>
    class allocator {
    public:
        using _From_primary = allocator;

        using value_type = _Ty;
        using size_type = _SizeType;
        using propagate_on_container_move_assignment = true_type;
        using is_always_equal = true_type;
        using Pointer = _Ty*;

        [[nodiscard]] Pointer allocate(_SizeType _Count) {
            static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
            auto _Size = _Count * sizeof(_Ty);
            return static_cast<Pointer>(alpha::allocate(_Size));
        }
        void deallocate(_Ty* const _Ptr, const _SizeType _Count) {
            if (_Ptr != nullptr) {
                auto _Size = _Count * sizeof(_Ty);
                alpha::destroy(_Ptr, _Size);
            }
        }
        static constexpr _SizeType _Minimum_allocation_alignment = 8;
    };
}

#endif