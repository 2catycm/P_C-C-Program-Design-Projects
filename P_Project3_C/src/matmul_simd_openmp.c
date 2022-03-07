//
// Created by 叶璨铭 on 2021/10/22.
//

#include "matmul_simd_openmp.h"


struct TrivialMatrix* matmul_simd_openmp(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if (matrixB->columnCount % 8 != 0) //A的行向量与B的列向量的长度需要是8的倍数
        return matmul_trivial(matrixA, matrixB);
    if (matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    __m256 b_line;
    __m256 c_results;
    float _a_element[8];
    __m256 a_element;
    struct TrivialMatrix* matrixC = create_trivial_matrix(matrixA->rowCount, matrixB->columnCount); //不会默认重制为0
    int i;
#pragma omp parallel for private(b_line, c_results, _a_element, a_element)
    for (i = 0; i < matrixA->rowCount; i++) {//固定A的一行
        for (size_t j = 0; j < matrixB->columnCount; j += 8) {//同时计算8个C的元素， 8个B的列也放一起
            c_results = _mm256_setzero_ps();
            for (size_t k = 0; k < matrixB->rowCount; k++) {
                _a_element[7] = _a_element[6] = _a_element[5] = _a_element[4] = _a_element[3] = _a_element[2] = _a_element[1] = _a_element[0] = TrivialMatrixGetElement(matrixA, i, k);
                a_element = _mm256_load_ps(_a_element);
                b_line = _mm256_load_ps(&TrivialMatrixGetElement(matrixB, k, j));
                c_results = _mm256_add_ps(c_results, _mm256_mul_ps(a_element, b_line));
            }
            _mm256_store_ps(&TrivialMatrixGetElement(matrixC, i, j), c_results);
        }
    }
    return matrixC;
}
__Matmul_Deprecated
struct TrivialMatrix* matmul_simd_openmp2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if (matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount); //不会默认重制为0
    int i;
#pragma omp parallel private(i)
    {
#pragma omp for
        for (i = 0; i < matrixA->rowCount; ++i) {
            for (size_t k = 0; k < matrixA->columnCount; ++k) {
                float temp = TrivialMatrixGetElement(matrixA, i, k), adder;
                int j;
#pragma omp simd
                for (j = 0; j < matrixB->columnCount; ++j) {
                    adder = temp * TrivialMatrixGetElement(matrixB, k, j);
                    TrivialMatrixGetElement(result, i, j) += adder;
                    // TrivialMatrixGetElement(result, i, j) += temp * TrivialMatrixGetElement(matrixB, k, j);
                }
            }
        }
    }
    return result;
}Matmul_Deprecated__
__Matmul_Deprecated
struct TrivialMatrix* matmul_simd2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if (matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount);
    int i;
    for (i = 0; i < matrixA->rowCount; ++i) {
        int j;
        for (j = 0; j < matrixB->columnCount; ++j) {
            int k;
#pragma omp simd
            for (k = 0; k < matrixA->columnCount; ++k) {
                TrivialMatrixGetElement(result, i, j) += TrivialMatrixGetElement(matrixA,i,k) * TrivialMatrixGetElement(matrixB,k,j);
            }
        }
    }
    return result;
}Matmul_Deprecated__

struct TrivialMatrix* matmul_simd(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if(matrixB->columnCount % 8 != 0) //A的行向量与B的列向量的长度需要是8的倍数
        return matmul_trivial(matrixA, matrixB);
    if (matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    __m256 b_line;
    __m256 c_results;
    float _a_element[8];
    __m256 a_element;
    struct TrivialMatrix* matrixC = create_trivial_matrix(matrixA->rowCount, matrixB->columnCount); //不会默认重制为0
    for (size_t i = 0; i < matrixA->rowCount; i++) {//固定A的一行
        for (size_t j = 0; j < matrixB->columnCount; j+=8) {//同时计算8个C的元素， 8个B的列也放一起
            c_results = _mm256_setzero_ps();
            for(size_t k=0; k <matrixB->rowCount; k++ ) {
                _a_element[7] = _a_element[6] = _a_element[5] = _a_element[4] = _a_element[3]
                    = _a_element[2] = _a_element[1] = _a_element[0] = TrivialMatrixGetElement(matrixA, i, k);
                a_element = _mm256_load_ps(_a_element);
                b_line = _mm256_load_ps(&TrivialMatrixGetElement(matrixB, k, j));
                c_results = _mm256_add_ps(c_results, _mm256_mul_ps(a_element, b_line));
            }
            _mm256_store_ps(&TrivialMatrixGetElement(matrixC, i, j), c_results);
        }
    }
    return matrixC;
}

//Deprecated
__Matmul_Deprecated
struct TrivialMatrix* matmul_simd_bad(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if(matrixA->columnCount % 8 != 0) //行向量与列向量的长度需要是8的倍数
        return matmul_trivial(matrixA, matrixB);
    if(matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    //    size_t groupCount = matrixB->columnCount/8;
    struct TrivialMatrix* result = create_trivial_matrix(matrixA->rowCount, matrixB->columnCount); //不会默认重制为0
    //元素Cij是行向量Ai与列向量Bj的点乘
    float element[8]; //用float 8位数组取回 256bit寄存器的结果。
    __m256 a, b;      // Ai的8个元素和Bj的8个元素
    __m256 sum;
    for(size_t i = 0; i < matrixA->rowCount; ++i) {
        for(size_t j = 0; j < matrixB->columnCount; ++j) {
            sum = _mm256_setzero_ps();
            for(size_t k = 0; k < matrixA->columnCount; k += 8) {
                //                result->data[i][j] += matrixA.data[i][k]*matrixB.data[k][j];
                a = _mm256_load_ps(&TrivialMatrixGetElement(matrixA, i, k));
                b = _mm256_load_ps(&TrivialMatrixGetElement(matrixB, k, j));
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
            }
            _mm256_store_ps(element, sum);
            TrivialMatrixGetElement(result, i, j) =
                element[0] + element[1] + element[2] + element[3] + element[4] + element[5] + element[6] + element[7];
        }
    }
    return result;
}Matmul_Deprecated__

struct TrivialMatrix* matmul_openmp(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    int numberOfThreads; //读取线程数。
#pragma omp parallel
    {
        numberOfThreads = omp_get_num_threads();
    }
    if(matrixA->rowCount % numberOfThreads != 0) //行向量与列向量的长度需要是8的倍数
        return matmul_trivial(matrixA, matrixB);
    if(matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount);
    //    for(int i = 0; i < matrixA->rowCount; ++i) {//做循环展开。展开的同时，发现不干扰，可以并行。于是可以omp parallel
    for(size_t i = 0; i < matrixA->rowCount; i += numberOfThreads) {
#pragma omp parallel firstprivate(i)
        {
            i +=
                omp_get_thread_num(); // i一次分配线程数，每个线程的i是私有的（从外层的i复制了一份）根据自己的线程编号来确定自己的i
            for(size_t k = 0; k < matrixA->columnCount; ++k) {
                float temp = TrivialMatrixGetElement(matrixA, i, k), adder;
                for(size_t j = 0; j < matrixB->columnCount; ++j) {
                    adder = temp * TrivialMatrixGetElement(matrixB, k, j);
                    TrivialMatrixGetElement(result, i, j) += adder;
                }
            }
        }
    }
    return result;
}

//如果是外层循环的parallel for呢？
struct TrivialMatrix* matmul_openmp2(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if(matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount); //不会默认重制为0
    int i;
#pragma omp parallel for /*firstprivate(i)*/
    for(i = 0; i < matrixA->rowCount; ++i) {
        for(size_t k = 0; k < matrixA->columnCount; ++k) {
            float temp = TrivialMatrixGetElement(matrixA, i, k), adder;
            for(size_t j = 0; j < matrixB->columnCount; ++j) {
                // adder = temp * TrivialMatrixGetElement(matrixB, k, j);
                // TrivialMatrixGetElement(result, i, j) += adder;
                TrivialMatrixGetElement(result, i, j) += temp * TrivialMatrixGetElement(matrixB, k, j);
            }
        }
    }
    return result;
}
