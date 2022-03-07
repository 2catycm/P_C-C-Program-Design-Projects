//
// Created by 叶璨铭 on 2021/9/29.
//
#include "Matrix.h"
template<typename E>
Matrix<E>::Matrix(E **data, size_t rowCount, size_t columnCount) 
: data(data), rowCount(rowCount), columnCount(columnCount) {} //外部构造E**,然后让我们进行构造
template<typename E>
Matrix<E>::Matrix(size_t rowCount, size_t columnCount):rowCount(rowCount), columnCount(columnCount){ //初始化普通构造，其实只是把手动的构造一个确定大小的二维数组的过程进行了合理的封装。
    data = new E *[rowCount];
    for (size_t i = 0; i < rowCount; ++i) {
        //            data[i] = new E[columnCount];
        //            memset(data[i], 0, sizeof(E) * columnCount);//不需要这样
        data[i] = new E[columnCount]{}; //c++11
    }
}
template<typename E>
Matrix<E>::~Matrix() {
    if (fatherMatrix != nullptr) //为了子数组的析构，设计成这样。
    {
        return;
    }
    for (int i = 0; i < rowCount; ++i) {
        delete[] data[i];
    }
    delete[] data;
}
template<typename E>
Matrix<E> Matrix<E>::*sub_matrix(size_t firstRow, size_t firstColumn, size_t rowCount, size_t columnCount) {
    auto result = this;
    return result;
}

template<typename E>
const E &Matrix<E>::get(size_t rowIndex, size_t columnIndex) {
    return data[firstRow + rowIndex][firstColumn + columnIndex];
}






static template
<typename E>
        Matrix<E>
*

operator*(const Matrix<E> &matrix_a, const Matrix<E> &matrix_b) //因为我们不实现乘等于，也没用上c++11的移动构造，所以就没办法了,只能返回指针。
{
    if (matrix_a.columnCount != matrix_b.rowCount)
        throw std::runtime_error("Matrix multiplication of whose sizes do not match is not supported.");
    return _concurrent_continuous_multiplication(matrix_a, matrix_b);
}

static template
<typename E>
        Matrix<E>
*

_concurrent_continuous_multiplication(const Matrix<E> &matrix_a, const Matrix<E> &matrix_b) {
    auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount);
#pragma omp parallel for
    for (int i = 0; i < matrix_a.rowCount; ++i) {
        for (int k = 0; k < matrix_a.columnCount; ++k) {
            E temp = matrix_a.data[i][k];
            for (int j = 0; j < matrix_b.columnCount; ++j) {
                result->data[i][j] += temp * matrix_b.data[k][j];  //友元，应该不用改代码
            }
        }
    }
    return result;
}

static template
<typename E>
        Matrix<E>
*

_continuous_multiplication(const Matrix<E> &matrix_a, const Matrix<E> &matrix_b) {
    auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount); //auto只是为了防止抄太多遍。
    //i和k如果是固定的，那么只是对j进行连续操作。
    for (int i = 0; i < matrix_a.rowCount; ++i) {
        for (int k = 0; k < matrix_a.columnCount; ++k) {
            E temp = matrix_a.data[i][k];
            for (int j = 0; j < matrix_b.columnCount; ++j) {
                result->data[i][j] += temp * matrix_b.data[k][j];
            }
        }
    }
    return result; //复制地址，而内存因为在堆空间上，所以没有被释放。释放的责任移交给了外面调用我这个函数的函数。避免了对内容进行复制。
}

static template
<typename E>
        Matrix<E>
*

_trivial_multiplication(const Matrix<E> &matrix_a, const Matrix<E> &matrix_b) {
    auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount); //auto只是为了防止抄太多遍。
    for (int i = 0; i < matrix_a.rowCount; ++i) {
        for (int j = 0; j < matrix_b.columnCount; ++j) {
            for (int k = 0; k < matrix_a.columnCount; ++k) {
                result->data[i][j] += matrix_a.data[i][k] * matrix_b.data[k][j]; //可以用+=的理由就在于我们前面new的时候调用了构造函数来初始化为0.
            }
        }
    }
    return result;
}

template<typename E>
static
Matrix<E> *readMatrixFromFile(const char *filename, E type) {
    FILE *file = fopen(filename, "r");
    int rowCount;
    char buffer[262144];//每一行

    if (file == nullptr) {
        throw std::runtime_error("file not found.");
    }
    E **lines = new E *[matrix_max_size];
    E element;
    std::stringstream ss;
    int columnCountMax = 0;
    for (rowCount = 0; fgets(buffer, sizeof(buffer), file) != nullptr; ++rowCount) {
        int columnCount = 0;
        E *line = new E[matrix_max_size];
        char *token = strtok(buffer, " "); //不能正则的。而且还是个指针，只能一步步走。
        while (token != nullptr) {
            if (std::is_same<E, double>::value) {  //泛型对具体类型执行不同操作.
                //不是fscanf，是sscanf
                sscanf(token, "%lf", &element);
            } else {
                sscanf(token, "%f", &element);
            }
//                lines[rowCount][columnCount++] = element;//栈上数组不可取。
            line[columnCount++] = element;
            token = strtok(nullptr, " ");
        }
        lines[rowCount] = line; //一行的数据读完了。
        if (columnCount > columnCountMax)
            columnCountMax = columnCount;
    }
    fclose(file);
    return new Matrix<E>{lines, rowCount, columnCountMax};
}

static
void writeMatrixToFile(const Matrix<E> *matrix, const char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < matrix->rowCount; ++i) {
        int j;
        for (j = 0; j < matrix->columnCount - 1; ++j) {
//                fprintf("%f ", matrix)
            if (std::is_same<E, double>::value) {  //泛型对具体类型执行不同操作
                fprintf(file, "%.1lf ", matrix->data[i][j]);
            } else {
                fprintf(file, "%.1f ", matrix->data[i][j]);
            }
        }
        if (std::is_same<E, double>::value) {  //泛型对具体类型执行不同操作
            fprintf(file, "%.1lf\n", matrix->data[i][j]);
        } else {
            fprintf(file, "%.1f\n", matrix->data[i][j]);
        }
    }
    fclose(file);
}

static
void file_matrix_multiplication(char *in_out_filenames[]) //为什么不能是const
{
    Matrix<E> matrix_a = *readMatrixFromFile(in_out_filenames[0];
    Matrix < E > matrix_b = *readMatrixFromFile(in_out_filenames[1]);
    Matrix < E > *matrix_c = matrix_a * matrix_b;
    writeMatrixToFile(matrix_c, in_out_filenames[2]);
    delete matrix_c;
}
