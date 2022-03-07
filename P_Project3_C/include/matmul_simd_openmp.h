//
// Created by 叶璨铭 on 2021/10/22.
//

#ifndef P_PROJECT3_C_MATMUL_SIMD_OPENMP_H
#define P_PROJECT3_C_MATMUL_SIMD_OPENMP_H
#include <intrin.h>
#include <omp.h>
#include "matmul_trivial.h"
struct TrivialMatrix* matmul_simd_openmp(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix* matmul_simd_openmp2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix *matmul_simd(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix *matmul_simd2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix *matmul_openmp(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix* matmul_openmp2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
#endif  // P_PROJECT3_C_MATMUL_SIMD_OPENMP_H
