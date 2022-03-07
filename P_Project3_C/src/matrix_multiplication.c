//
// Created by 叶璨铭 on 2021/10/19.
//

#include "matrix_multiplication.h"
#define TrivialModelMultiplication(matmul_fun)                       \
    struct TrivialMatrix* a = read_trivial_matrix_from_file(input1); \
    struct TrivialMatrix* b = read_trivial_matrix_from_file(input2); \
    struct TrivialMatrix* c = matmul_fun(a, b);                      \
    write_trivial_matrix_to_file(c, output);                         \
    delete_trivial_matrix(a);                                        \
    delete_trivial_matrix(b);                                        \
    delete_trivial_matrix(c);
void file_matrix_multiplication(char* input1, char* input2, char* output, enum multiplication_mode mode) {
    switch(mode) {
        case TRIVIAL: {
            TrivialModelMultiplication(matmul_trivial);
            break;
        }
        case OPENBLAS: {
            TrivialModelMultiplication(matmul_blas);
            break;
        }
        case STRASSEN: {
            TrivialModelMultiplication(matmul_strassen);
            break;
        }
        case SIMD: {
            TrivialModelMultiplication(matmul_simd);
            break;
        }
        case OPENMP: {
            TrivialModelMultiplication(matmul_openmp2);
            break;
        }
        case COMBINED: {
            TrivialModelMultiplication(matmul_simd_openmp2);
            break;
        }
        default:{
            abort_with_message("No such computation mode.");
            break;
        }
    }
}