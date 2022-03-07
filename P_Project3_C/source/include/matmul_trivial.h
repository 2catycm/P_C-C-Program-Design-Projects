//
// Created by 叶璨铭 on 2021/10/19.
//

#ifndef P_PROJECT3_C_MATMUL_TRIVIAL_H
#define P_PROJECT3_C_MATMUL_TRIVIAL_H
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "util.h"
//common for struct
struct TrivialMatrix{
    float* data;
    size_t rowCount;
    size_t columnCount;
};
#define TrivialMatrixGetElement(matrix, i, j) ((matrix)->data[(matrix)->rowCount*(i)+(j)])
struct TrivialMatrix* _trivial_matrix(float* data, const size_t rowCount, const size_t columnCount);//完全从已有数据构造矩阵 //不建议调用
struct TrivialMatrix *create_zero_trivial_matrix(const size_t rowCount, const size_t columnCount) ;
struct TrivialMatrix* create_trivial_matrix(const size_t rowCount, const size_t columnCount);//会申请float*的空间,不过不会初始化为0
struct TrivialMatrix* create_from_existing_trivial_matrix(const struct TrivialMatrix* existing_matrix);//return a copy of the existing existing_matrix
void delete_trivial_matrix(struct TrivialMatrix* matrix);
struct StringBuilder* trivial_matrix_to_string(const struct TrivialMatrix* matrix);
struct TrivialMatrix* read_trivial_matrix_from_file(const char* file_name);//malloc a new one and return the address.
void write_trivial_matrix_to_file(const struct TrivialMatrix* matrix, const char* file_name);
//multiplication
struct TrivialMatrix* matmul_trivial(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
//addition and subtraction.
/**
 * Subtraction should not be treated as plus negative other, since negating a matrix takes time.
 * So I designed this function to do such θ(n^2) thing together.
 * @param matrixA
 * @param matrixB
 * @param alpha
 * @param beta
 * @return alpha*A + beta*B
 */
struct TrivialMatrix* matrix_element_by_element_linear_combinations(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB,
                                                                    float alpha, float beta);
struct TrivialMatrix* trivial_matrix_addition(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix* trivial_matrix_subtraction(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB);
struct TrivialMatrix* trivial_matrix_scalar_multiplication(const struct TrivialMatrix* matrixA, float scalar);
#endif //P_PROJECT3_C_MATRIX_TRIVIAL_H
