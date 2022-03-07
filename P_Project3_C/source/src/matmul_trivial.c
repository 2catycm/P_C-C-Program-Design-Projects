#include "matmul_trivial.h"

struct TrivialMatrix* _trivial_matrix(float* data, const size_t rowCount, const size_t columnCount) {
    struct TrivialMatrix* result = (struct TrivialMatrix*)malloc(sizeof(struct TrivialMatrix));
    if(result == NULL) {
        abort_with_message("matrix allocation failed.");
    }

    result->data = data;
    result->rowCount = rowCount;
    result->columnCount = columnCount;
    return result;
}

struct TrivialMatrix* create_zero_trivial_matrix(const size_t rowCount, const size_t columnCount) {
    size_t size = rowCount * columnCount * sizeof(float);
    float* data = (float*)malloc(size);
    if(data == NULL) {
        abort_with_message("matrix allocation failed.");
    }
    memset(data, 0, size);
    return _trivial_matrix(data, rowCount, columnCount);
}

// Deprecated
struct TrivialMatrix* create_trivial_matrix(const size_t rowCount, const size_t columnCount) {
    float* data = (float*)calloc(rowCount * columnCount, sizeof(float));
    return _trivial_matrix(data, rowCount, columnCount);
}

struct TrivialMatrix* create_from_existing_trivial_matrix(const struct TrivialMatrix* existing_matrix) {
    size_t size = existing_matrix->rowCount * existing_matrix->columnCount;
    float* newData = (float*)calloc(size, sizeof(float));
    if(newData == NULL) {
        abort_with_message("matrix data coping failed.");
    }
    memcpy(newData, existing_matrix->data, size);
    return _trivial_matrix(newData, existing_matrix->rowCount, existing_matrix->columnCount);
}

void delete_trivial_matrix(struct TrivialMatrix* matrix) {
    free(matrix->data);
    free(matrix);
}

struct StringBuilder* trivial_matrix_to_string(const struct TrivialMatrix* matrix) {
    //    char* strbuilder = (char*) calloc(matrix->columnCount, sizeof(char));
    //    strbuilder[0] = '\0';
    struct StringBuilder* stringBuilder = StringBuilder(matrix->rowCount * matrix->columnCount);
    for(size_t i = 0; i < matrix->rowCount; ++i) {
        char element_buffer[32];
        size_t j;
        for(j = 0; j < matrix->columnCount - 1; ++j) {
            size_t element_length;
            sprintf(element_buffer, "%f ", TrivialMatrixGetElement(matrix, i, j));
            element_length = strlen(element_buffer);
            //            result_string_length += element_length;
            append_string(stringBuilder, element_buffer);
        }
        sprintf(element_buffer, "%f\n", TrivialMatrixGetElement(matrix, i, j));
        append_string(stringBuilder, element_buffer);
    }
    return stringBuilder;
}

void write_trivial_matrix_to_file(const struct TrivialMatrix* matrix, const char* file_name) {
    FILE* file = fopen(file_name, "w");
    for(int i = 0; i < matrix->rowCount; ++i) {
        int j;
        for(j = 0; j < matrix->columnCount - 1; ++j) {
            fprintf(file, "%f ", TrivialMatrixGetElement(matrix, i, j));
        }
        fprintf(file, "%f\n", TrivialMatrixGetElement(matrix, i, j));
    }
    fclose(file);
}

struct TrivialMatrix* read_trivial_matrix_from_file(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if(file == NULL) {
        abort_with_message("Fatal Error: File not found.\nmatrix multiplication terminated.");
    }
    size_t file_length = file_len(file);
    size_t max_matrix_length = (file_length + 1) / 2;  //根据文件大小，解不等式得到
    float* data = (float*)calloc(max_matrix_length, sizeof(float));
    int k = 0;
    size_t rowCount = 0, columnCount = 0;
    size_t columnCountMax = 0;  //千万不能设置为INT32_MIN！！！
    size_t buffer_max_size = max_matrix_length * 32;
    //一行的信息，极端情况下一行有2048个数，每个数很长很长，假设字面量最长为32
    char* buffer = (char*)calloc(buffer_max_size, sizeof(char));
    for(rowCount = 0; fgets(buffer, buffer_max_size, file) != NULL; ++rowCount) {
        columnCount = 0;
        char* token = strtok(buffer, " ");  //不能正则的。而且还是个指针，只能一步步走。
        while(token != NULL) {
            columnCount++;
            sscanf(token, "%f", &data[k++]);
            token = strtok(NULL, " ");
        }
        if(columnCount > columnCountMax)
            columnCountMax = columnCount;
    }
    fclose(file);
    data = realloc(data, rowCount * columnCountMax * sizeof(float));
    return _trivial_matrix(data, rowCount, columnCountMax);
}

struct TrivialMatrix* matmul_trivial(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    if(matrixA->columnCount != matrixB->rowCount) {
        abort_with_message("Matrix multiplication of whose sizes do not match is not supported.");
    }
    struct TrivialMatrix* result = create_zero_trivial_matrix(matrixA->rowCount, matrixB->columnCount);  //不会默认重制为0
    //#pragma omp parallel for
    for(int i = 0; i < matrixA->rowCount; ++i) {
        for(int k = 0; k < matrixA->columnCount; ++k) {
            float temp = TrivialMatrixGetElement(matrixA, i, k), adder;
            int j;  // OpenMP的for在msvc的支持下是不允许初始化的。为了兼容写在外面。
//#pragma omp parallel for
            for(j = 0; j < matrixB->columnCount; ++j) {
                adder = temp * TrivialMatrixGetElement(matrixB, k, j);
//#pragma omp critical
                TrivialMatrixGetElement(result, i, j) += adder;   //bad use of OpenMP.
            }
        }
    }
    return result;
}
struct TrivialMatrix* matrix_element_by_element_linear_combinations(const struct TrivialMatrix* matrixA,
                                                                    const struct TrivialMatrix* matrixB, float alpha,
                                                                    float beta) {
    if(matrixA->rowCount != matrixB->rowCount || matrixA->columnCount != matrixB->columnCount) {
        abort_with_message("Matrix linear combinations of whose sizes do not match is not supported.\nError has occurred in "
                           "function Matrix_element_by_element_linear_combinations.");
    }
    struct TrivialMatrix* result = create_trivial_matrix(  //这一次不需要置零。
        matrixA->rowCount, matrixA->columnCount);
    for(int i = 0; i < matrixA->rowCount; i++) {  //同一行之内进行内层的for连续性更好
        for(int j = 0; j < matrixA->columnCount; j++) {
            TrivialMatrixGetElement(result, i, j) =
                alpha * TrivialMatrixGetElement(matrixA, i, j) + beta * TrivialMatrixGetElement(matrixB, i, j);
        }
    }
    return result;
}
struct TrivialMatrix* trivial_matrix_addition(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    return matrix_element_by_element_linear_combinations(matrixA, matrixB, 1, 1);
}
struct TrivialMatrix* trivial_matrix_subtraction(const struct TrivialMatrix* matrixA, const struct TrivialMatrix* matrixB) {
    return matrix_element_by_element_linear_combinations(matrixA, matrixB, 1, -1);
}
struct TrivialMatrix* trivial_matrix_scalar_multiplication(const struct TrivialMatrix* matrixA, float scalar) {
    return matrix_element_by_element_linear_combinations(matrixA, matrixA, scalar, 0);
}