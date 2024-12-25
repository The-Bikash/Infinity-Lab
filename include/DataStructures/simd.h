#pragma once
#ifndef __SIMD__
#define __SIMD__


#include <immintrin.h>
#include <intrin.h>

namespace alpha {

#ifdef __AVX512F__
#define __mxi          __m512i
#define __mx           __m512
#define __mxd          __m512d

#define _mmx_set1_ps   _mm512_set1_ps

#define _mmx_load_six  _mm512_loadu_si512
#define _mmx_load_ps   _mm512_load_ps
#define _mmx_load_pd   _mm512_load_pd

#define _mmx_add_ps    _mm512_add_ps
#define _mmx_add_pd    _mm512_add_pd
#define _mmx_sub_ps    _mm512_sub_ps
#define _mmx_sub_ps    _mm512_sub_pd

#define _mmx_store_si _mm512_storeu_si512
#define _mmx_store_ps  _mm512_storeu_ps
#define _mmx_store_pd  _mm512_storeu_pd

#define _mmx_fmadd_ps   _mm512_fmadd_ps
#define _mmx_fnmadd_ps  _mm512_fnmadd_ps

#elif __AVX2__
#define __mxi          __m256i
#define __mx           __m256
#define __mxd          __m256d

#define _mmx_set1_ps   _mm256_set1_ps
#define _mmx_set1_pd   _mm256_set1_pd

#define _mmx_load_six  _mm256_load_si256
#define _mmx_load_ps   _mm256_loadu_ps
#define _mmx_load_pd   _mm256_load_pd

#define _mmx_add_ps    _mm256_add_ps
#define _mmx_add_pd    _mm256_add_pd
#define _mmx_sub_ps    _mm256_sub_ps
#define _mmx_sub_pd    _mm256_sub_pd
#define _mmx_mul_ps    _mm256_mul_ps
#define _mmx_mul_pd    _mm256_mul_pd
#define _mmx_div_ps    _mm256_div_ps
#define _mmx_div_pd    _mm256_div_pd

#define _mmx_store_ps  _mm256_store_ps
#define _mmx_store_pd  _mm256_store_pd
#define _mmx_store_si  _mm256_store_si256

#define _mmx_fmadd_ps  _mm256_fmadd_ps
#define _mmx_fmadd_pd  _mm256_fmadd_pd
#define _mmx_fnmadd_ps _mm256_fnmadd_ps
#define _mmx_fnmadd_pd _mm256_fnmadd_pd

#else
#define NOSIMD
#endif
   void avx_add_vecf (const float* const A, const float* const B, float* const C, const size_t _Count);
   void avx_sub_vecf (const float* const A, const float* const B, float* const C, const size_t _Count);
   void avx_madd_vecf(const float* const A, const float* const B, float* const C, const size_t _Count, const float _Lamda = 1)noexcept;
   void avx_msub_vecf(const float* const A, const float* const B, float* const C, const size_t _Count, const float _Lamda = 1)noexcept;
   void avx_mul_vecf (float* const _Vec, const float _Scalar, const size_t _Count);
   void avx_add_vecd (const double* const A, const double* const B, double* const C, const size_t _Count);
   void avx_sub_vecd (const double* const A, const double* const B, double* const C, const size_t _Count);
   void avx_madd_vecd(const double* const A, const double* const B, double* const C, const size_t _Count, const double _Lamda = 1);
   void avx_msub_vecd(const double* const A, const double* const B, double* const C, const size_t _Count, const double _Lamda = 1)noexcept;
   void avx_mul_vecd(double* const _Vec, const double _Scalar, const size_t _Count);
   void avx_sadd_vecf(float* a, float* b, size_t size);

}

#endif