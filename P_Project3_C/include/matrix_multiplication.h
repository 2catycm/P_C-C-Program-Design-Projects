//
// Created by 叶璨铭 on 2021/10/19.
//
#ifndef P_PROJECT3_C_MATRIX_MULTIPLICATION_H
#define P_PROJECT3_C_MATRIX_MULTIPLICATION_H
#ifdef __cplusplus
extern "C" {
#endif
#include "matmul_openblas.h"
#include "matmul_simd_openmp.h"
#include "matmul_strassen.h"
#include "matmul_trivial.h"
enum multiplication_mode {
    TRIVIAL,OPENBLAS,STRASSEN,SIMD,OPENMP,COMBINED
};//不研究cuda和sparse了
//不能放在这。全局变量有点难搞。main include会有两个变量出现。
//char* multiplication_mode_patterns[6] = {"trivial","openblas","strassen","simd","cuda","sparse"};
void file_matrix_multiplication(char *input1, char *input2, char *output, enum multiplication_mode mode);
#ifdef __cplusplus
}
#endif
#endif //P_PROJECT3_C_MATRIX_MULTIPLICATION_H
