//
// Created by 叶璨铭 on 2021/10/19.
//

#ifndef P_PROJECT3_C_MATMUL_OPENBLAS_H
#define P_PROJECT3_C_MATMUL_OPENBLAS_H
//#include <cblas.h>
//同时要在matmul_openblas.c include，否则链接不到。这是因为本项目中的链接依赖关系是，让sample的main依赖于matmul_blas.dll, 但是不直接依赖于openblas。这样可以将调库的过程封装到统一的接口，便于测试。
#include "matmul_trivial.h"  //需要借用平凡矩阵的结构体定义
struct TrivialMatrix* matmul_blas(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
#endif  // P_PROJECT3_C_MATMUL_OPENBLAS_H
