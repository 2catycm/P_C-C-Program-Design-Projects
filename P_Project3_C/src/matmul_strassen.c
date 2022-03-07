//
// Created by 叶璨铭 on 2021/10/21.
//

#include "matmul_strassen.h"
#define SQUARE(n) ((n) * (n))
#define Strassen_Threshold SQUARE(512)
// #define Strassen_Threshold SQUARE(16)
struct TrivialMatrix* matmul_strassen(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    return view_view_strassen(ToView(matrixA), ToView(matrixB));
}
struct TrivialMatrix* view_view_strassen(const struct MatrixView* matrixViewA, const struct MatrixView* matrixViewB) {
    // 1.size computation and checking
    size_t M = matrixViewA->viewSize.rowIndex;
    size_t K = matrixViewA->viewSize.columnIndex;
    size_t N = matrixViewB->viewSize.columnIndex;  // M*K matrix times K*N matrix
#ifdef _DEBUG
    if(K != matrixViewB->viewSize.rowIndex)
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.\n Error has occurred in function "
                           "view_view_strassen.");
        // debug use: stringBuilderToString(trivial_matrix_to_string(matrixViewA->data))
#endif
    // 2.base case checking
    if(M * N <= Strassen_Threshold)
        return view_matmul_openmp(matrixViewA, matrixViewB);
    // 3.submatrices creating
    size_t halfM = M / 2;
    size_t halfK = K / 2;
    size_t halfN = N / 2;  // left up inclusive, right down exclusive. //not mathematical 1 startingCoordinate
    struct MatrixView* A11 =
        createViewFromView(matrixViewA, *createMatrixCoordinate(0, 0),
                           *createMatrixCoordinate(halfM, halfK));  //不用减一， 0,1,2行的rowCount是3而非2 //FIXED
    struct MatrixView* A12 =
        createViewFromView(matrixViewA, *createMatrixCoordinate(0, halfK), *createMatrixCoordinate(halfM, K - halfK));
    struct MatrixView* A21 =
        createViewFromView(matrixViewA, *createMatrixCoordinate(halfM, 0), *createMatrixCoordinate(M - halfM, halfK));
    struct MatrixView* A22 =
        createViewFromView(matrixViewA, *createMatrixCoordinate(halfM, halfK), *createMatrixCoordinate(M - halfM, K - halfK));
    struct MatrixView* B11 =
        createViewFromView(matrixViewB, *createMatrixCoordinate(0, 0), *createMatrixCoordinate(halfK, halfN));
    struct MatrixView* B12 =
        createViewFromView(matrixViewB, *createMatrixCoordinate(0, halfN), *createMatrixCoordinate(halfK, N - halfN));
    struct MatrixView* B21 =
        createViewFromView(matrixViewB, *createMatrixCoordinate(halfK, 0), *createMatrixCoordinate(K - halfK, halfN));
    struct MatrixView* B22 =
        createViewFromView(matrixViewB, *createMatrixCoordinate(halfK, halfN), *createMatrixCoordinate(K - halfK, N - halfN));
    // S1-S10 computing.
    struct TrivialMatrix* S[11] = { NULL,
                                    view_subtraction(B12, B22),
                                    view_addition(A11, A12),
                                    view_addition(A21, A22),
                                    view_subtraction(B21, B11),
                                    view_addition(A11, A22),
                                    view_addition(B11, B22),
                                    view_subtraction(A12, A22),
                                    view_addition(B21, B22),
                                    view_subtraction(A11, A21),
                                    view_addition(B11, B12) };
    // P1-P7 computing.
    struct TrivialMatrix* P[8] = { NULL,
                                   view_view_strassen(A11, ToView(S[1])),  //不要交换
                                   view_view_strassen(ToView(S[2]), B22),
                                   view_view_strassen(ToView(S[3]), B11),
                                   view_view_strassen(A22, ToView(S[4])),
                                   matmul_strassen(S[5], S[6]),
                                   matmul_strassen(S[7], S[8]),
                                   matmul_strassen(S[9], S[10]) };
    struct TrivialMatrix* C11 =
        trivial_matrix_addition(trivial_matrix_subtraction(trivial_matrix_addition(P[5], P[4]), P[2]), P[6]);
    struct TrivialMatrix* C12 = trivial_matrix_addition(P[1], P[2]);
    struct TrivialMatrix* C21 = trivial_matrix_addition(P[3], P[4]);
    struct TrivialMatrix* C22 =
        trivial_matrix_subtraction(trivial_matrix_subtraction(trivial_matrix_addition(P[5], P[1]), P[3]), P[7]);
    return merge_matrices(C11, C12, C21, C22);
}
struct TrivialMatrix* merge_matrices(const struct TrivialMatrix* first, const struct TrivialMatrix* second,
                                     const struct TrivialMatrix* third, const struct TrivialMatrix* fourth) {
#ifdef _DEBUG
    if(first->rowCount != second->rowCount || first->columnCount != third->columnCount ||
       second->columnCount != fourth->columnCount || third->rowCount != fourth->rowCount) {
        abort_with_message("Merging submatrices of whose sizes do not match is not supported.\n Error has occurred in function "
                           "merge_matrices. ");
    }
#endif  //正常来说外面矩阵的错误会被外面判断好了，这个函数如果Strassen算法写对了不会有错的。所以Release模式下要去除判断，提高速度。
    struct TrivialMatrix* result =
        create_trivial_matrix(first->rowCount + third->rowCount, second->columnCount + third->columnCount);  //不需要置零
    for(int i = 0; i < first->rowCount; ++i) {
        for(int j = 0; j < first->columnCount; ++j) {
            TrivialMatrixGetElement(result, i, j) = TrivialMatrixGetElement(first, i, j);
        }
    }
    for(int i = 0; i < second->rowCount; ++i) {
        for(int j = 0; j < second->columnCount; ++j) {
            TrivialMatrixGetElement(result, i, j + first->columnCount) = TrivialMatrixGetElement(second, i, j);
        }
    }
    for(int i = 0; i < third->rowCount; ++i) {
        for(int j = 0; j < third->columnCount; ++j) {
            TrivialMatrixGetElement(result, i + first->rowCount, j) = TrivialMatrixGetElement(third, i, j);
        }
    }
    for(int i = 0; i < fourth->rowCount; ++i) {
        for(int j = 0; j < fourth->columnCount; ++j) {
            TrivialMatrixGetElement(result, i + first->rowCount, j + first->columnCount) = TrivialMatrixGetElement(fourth, i, j);
        }
    }
    return result;
}
// view version of basic trivial matrix operations
__Matmul_Deprecated
struct TrivialMatrix* view_matmul_simd(const struct MatrixView* matrixViewA, const struct MatrixView* matrixViewB) {
    struct MatrixCoordinate sizeA = matrixViewA->viewSize;
    struct MatrixCoordinate sizeB = matrixViewB->viewSize;  //处理成变量，防止后面的指针访问元素操作过多
    if(sizeB.columnIndex % 8 != 0)                          //行向量与列向量的长度需要是8的倍数
        return view_matmul_openmp(matrixViewA, matrixViewB);
#ifdef _DEBUG
    if(sizeA.columnIndex != sizeB.rowIndex) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.\n Error has occurred in function "
                           "view_matmul_trivial. ");
    }
    // debug use: stringBuilderToString(trivial_matrix_to_string(matrixViewA->data))
#endif
    struct TrivialMatrix* matrixC = create_trivial_matrix(sizeA.rowIndex, sizeB.columnIndex);
    for (size_t i = 0; i < sizeA.rowIndex; i++) {//固定A的一行
        for (size_t j = 0; j < sizeB.columnIndex; j += 8) {//同时计算8个C的元素， 8个B的列也放一起
            __m256 b_line;
            __m256 c_results = _mm256_setzero_ps();
            float _a_element[8];
            __m256 a_element;
            for (size_t k = 0; k < sizeB.rowIndex; k++) {
                _a_element[7] = _a_element[6] = _a_element[5] = _a_element[4] = _a_element[3] = _a_element[2] = _a_element[1] = _a_element[0] = MatrixViewGetElement(matrixViewA, i, k);
                a_element = _mm256_load_ps(_a_element);
                b_line = _mm256_load_ps(&MatrixViewGetElement(matrixViewB, k, j));
                c_results = _mm256_add_ps(c_results, _mm256_mul_ps(a_element, b_line));
            }
            _mm256_store_ps(&TrivialMatrixGetElement(matrixC, i, j), c_results);
        }
    }
    return matrixC;
}Matmul_Deprecated__
struct TrivialMatrix* view_matmul_openmp(const struct MatrixView* matrixViewA, const struct MatrixView* matrixViewB) {
    struct MatrixCoordinate sizeA = matrixViewA->viewSize;
    struct MatrixCoordinate sizeB = matrixViewB->viewSize;  //处理成变量，防止后面的指针访问元素操作过多
#ifdef _DEBUG
    if (sizeA.columnIndex != sizeB.rowIndex) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.\n Error has occurred in function "
            "view_matmul_openmp. ");
    }
#endif
    struct TrivialMatrix* result = create_zero_trivial_matrix(sizeA.rowIndex, sizeB.columnIndex);
    int i;
#pragma omp parallel for
    for (i = 0; i < sizeA.rowIndex; ++i) {
        for (int k = 0; k < sizeB.columnIndex; ++k) {
            float temp = MatrixViewGetElement(matrixViewA, i, k), adder;
            for (int j = 0; j < sizeB.columnIndex; ++j) {
                adder = temp * MatrixViewGetElement(matrixViewB, k, j);
                TrivialMatrixGetElement(result, i, j) += adder;
            }
        }
    }
    return result;
}

struct TrivialMatrix* view_element_by_element_linear_combinations(const struct MatrixView* matrixViewA,
                                                                  const struct MatrixView* matrixViewB, float alpha, float beta) {
    struct MatrixCoordinate sizeA = matrixViewA->viewSize;
#ifdef _DEBUG
    struct MatrixCoordinate sizeB = matrixViewB->viewSize;
    if(sizeA.rowIndex != sizeB.rowIndex || sizeA.columnIndex != sizeB.columnIndex) {
        abort_with_message("Matrix linear combinations of whose sizes do not match is not supported.\nError has occurred in "
                           "function view_element_by_element_linear_combinations.");
    }
#endif
    struct TrivialMatrix* result = create_trivial_matrix(  //这一次不需要置零。
        sizeA.rowIndex, sizeA.columnIndex);
    for(int i = 0; i < sizeA.rowIndex; i++) {  //同一行之内进行内层的for连续性更好
        for(int j = 0; j < sizeA.columnIndex; j++) {
            TrivialMatrixGetElement(result, i, j) =
                alpha * MatrixViewGetElement(matrixViewA, i, j) + beta * MatrixViewGetElement(matrixViewB, i, j);
        }
    }
    return result;
}
struct TrivialMatrix* view_addition(const struct MatrixView* matrixA, const struct MatrixView* matrixB) {
    return view_element_by_element_linear_combinations(matrixA, matrixB, 1, 1);
}
struct TrivialMatrix* view_subtraction(const struct MatrixView* matrixA, const struct MatrixView* matrixB) {
    return view_element_by_element_linear_combinations(matrixA, matrixB, 1, -1);
}
struct TrivialMatrix* view_scalar_multiplication(const struct MatrixView* matrixA, float scalar) {
    return view_element_by_element_linear_combinations(matrixA, matrixA, scalar, 0);
}

struct MatrixCoordinate* createMatrixCoordinate(size_t rowIndex, size_t columnIndex) {
    struct MatrixCoordinate* newCoordinate = (struct MatrixCoordinate*)malloc(sizeof(struct MatrixCoordinate));
    newCoordinate->rowIndex = rowIndex;
    newCoordinate->columnIndex = columnIndex;
    return newCoordinate;
}
struct MatrixCoordinate* matrix_coordinate_addition(const struct MatrixCoordinate coordinateA,
                                                    const struct MatrixCoordinate coordinateB) {
    struct MatrixCoordinate* newCoordinate = (struct MatrixCoordinate*)malloc(sizeof(struct MatrixCoordinate));
    newCoordinate->rowIndex = coordinateA.rowIndex + coordinateB.rowIndex;
    newCoordinate->columnIndex = coordinateA.columnIndex + coordinateB.columnIndex;
    return newCoordinate;
}
struct MatrixView* createViewFromView(const struct MatrixView* view, struct MatrixCoordinate startingCoordinate,
                                      struct MatrixCoordinate viewSize) {
    struct MatrixView* newView = (struct MatrixView*)malloc(sizeof(struct MatrixView));
    newView->data = view->data;
    newView->startingCoordinate = *matrix_coordinate_addition(startingCoordinate, view->startingCoordinate);
    newView->viewSize = viewSize;
    return newView;
}
struct MatrixView* createViewFromTrivialMatrix(const struct TrivialMatrix* matrix, struct MatrixCoordinate startingCoordinate,
                                               struct MatrixCoordinate viewSize) {
    struct MatrixView* newView = (struct MatrixView*)malloc(sizeof(struct MatrixView));
    newView->data = matrix;
    newView->startingCoordinate = startingCoordinate;
    newView->viewSize = viewSize;
    return newView;
}
struct MatrixView* ToView(const struct TrivialMatrix* matrix) {
    struct MatrixCoordinate startingCoordinate = *createMatrixCoordinate(0, 0);
    struct MatrixCoordinate viewSize =
        *createMatrixCoordinate(matrix->rowCount, matrix->columnCount);  //行为应该是把堆上分配的新坐标复制到view上。
    return createViewFromTrivialMatrix(matrix, startingCoordinate, viewSize);
}
