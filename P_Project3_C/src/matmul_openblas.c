//
// Created by 叶璨铭 on 2021/10/19.
//

#include "matmul_openblas.h"
#include <cblas.h>
struct TrivialMatrix* matmul_blas(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if(matrixA->columnCount != matrixB->rowCount) {
        fprintf(stderr, "Matrix multiplication of whose sizes do not match is not supported.");
        abort();
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount);
    const float alpha = 1;
    const float beta = 0;
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, matrixA->rowCount, matrixB->columnCount,  // M,N
                matrixA->columnCount,                                                                // K
                alpha, matrixA->data, matrixA->columnCount,                                          // A的数据, A的列数
                matrixB->data, matrixB->columnCount,                                                 // B的数据, B的列数
                beta, result->data, matrixB->columnCount);
    return result;
}
