#pragma once
#ifndef __SIMD__
#define __SIMD__
#include "../Core/core.h"
namespace alpha {
   void avx_add_vecf (const float* const A, const float* const B, float* const C, const _SizeType _Count)noexcept;
   void avx_sub_vecf (const float* const A, const float* const B, float* const C, const _SizeType _Count)noexcept;
   void avx_madd_vecf(const float* const A, const float* const B, float* const C, const _SizeType _Count, const float _Lamda = 1)noexcept;
   void avx_msub_vecf(const float* const A, const float* const B, float* const C, const _SizeType _Count, const float _Lamda = 1)noexcept;
   void avx_mul_vecf (float* const _Vec, const float _Scalar, const _SizeType _Count)noexcept;
   void avx_add_vecd (const double* const A, const double* const B, double* const C, const _SizeType _Count)noexcept;
   void avx_sub_vecd (const double* const A, const double* const B, double* const C, const _SizeType _Count)noexcept;
   void avx_madd_vecd(const double* const A, const double* const B, double* const C, const _SizeType _Count, const double _Lamda = 1)noexcept;
   void avx_msub_vecd(const double* const A, const double* const B, double* const C, const _SizeType _Count, const double _Lamda = 1)noexcept;
   void avx_mul_vecd(double* const _Vec, const double _Scalar, const _SizeType _Count)noexcept;
   void avx_sadd_vecf(float* a, float* b, _SizeType size)noexcept;
}

#endif