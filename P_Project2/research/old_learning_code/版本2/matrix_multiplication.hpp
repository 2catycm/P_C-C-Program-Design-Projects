//
// Created by 叶璨铭 on 2021/9/25.
//
#pragma once
#include "matrix_multiplication.h"
namespace matrix_multiplication
{
    const int matrix_max_size = 4096;
    template<typename E>
    struct Matrix
    {
        E** data; //[行坐标][列坐标]
        int rowCount;
        int columnCount;
        Matrix(E **data, int rowCount, int columnCount) : data(data), rowCount(rowCount), columnCount(columnCount) {}
        Matrix(int rowCount, int columnCount):rowCount(rowCount), columnCount(columnCount){//初始化普通构造，其实只是把手动的构造一个确定大小的二维数组的过程进行了合理的封装。
            data = new E*[rowCount];
            for (int i = 0; i < rowCount; ++i) {
                data[i] = new E[columnCount] ; //{}
                memset(data[i], 0, sizeof(E)*columnCount);
            }
        }
        ~Matrix(){
            for (int i = 0; i < rowCount; ++i) {
                delete [] data[i];
            }
            delete[] data;
        }
    };
    template<typename E>
    Matrix<E>* _concurrent_continuous_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b){
        auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount);
#pragma omp parallel for
        for (int i = 0; i < matrix_a.rowCount; ++i) {
            for (int k = 0; k < matrix_a.columnCount; ++k){
                E temp = matrix_a.data[i][k];
                for (int j = 0; j < matrix_b.columnCount; ++j) {
                    result->data[i][j] += temp*matrix_b.data[k][j];
                }
            }
        }
        return result;
    }
    template<typename E>
    Matrix<E>* _continuous_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b){
        //初始化二维数组
//        E** result = new E*[matrix_a.rowCount];
//        for (int i = 0; i < matrix_a.rowCount; ++i) {
//            result[i] = new E[matrix_b.columnCount] ; //{}
//            memset(result[i], 0, sizeof(E)*matrix_b.columnCount);
//        }
        auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount);//auto只是为了防止抄太多遍。
        //i和k如果是固定的，那么只是对j进行连续操作。
        for (int i = 0; i < matrix_a.rowCount; ++i) {
            for (int k = 0; k < matrix_a.columnCount; ++k){
                E temp = matrix_a.data[i][k];
                for (int j = 0; j < matrix_b.columnCount; ++j) {
                    result->data[i][j] += temp*matrix_b.data[k][j];
                }
            }
        }
        return result;//复制地址，而内存因为在堆空间上，所以没有被释放。释放的责任移交给了外面调用我这个函数的函数。避免了对内容进行复制。
//        return new Matrix<E>{result, matrix_a.rowCount, matrix_b.columnCount};
    }
    template<typename E>
    Matrix<E>* _trivial_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b){
        auto result = new Matrix<E>(matrix_a.rowCount, matrix_b.columnCount);//auto只是为了防止抄太多遍。
#pragma parallel for
        for (int i = 0; i < matrix_a.rowCount; ++i) {
            for (int j = 0; j < matrix_b.columnCount; ++j) {
                for (int k = 0; k < matrix_a.columnCount; ++k) {
                    result->data[i][j] += matrix_a.data[i][k]*matrix_b.data[k][j];
                }
            }
        }
        return new Matrix<E>{result, matrix_a.rowCount, matrix_b.columnCount};
    }
    template<typename E>
    Matrix<E>* operator*(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b) //因为我们不实现乘等于，所以就没办法了。
    {
        if (matrix_a.columnCount != matrix_b.rowCount)
            throw std::runtime_error("Matrix multiplication of whose sizes do not match is not supported.");
        return _concurrent_continuous_multiplication(matrix_a, matrix_b);
    }
    template<typename E>
    Matrix<E>* readMatrixFromFile(const char* filename, E type)
    {
        FILE* file = fopen(filename, "r");
        int rowCount;
        char buffer[262144];//每一行

        if (file == nullptr) {
            throw std::runtime_error("file not found.");
        }
        E** lines = new E*[matrix_max_size];
        E element; std::stringstream ss;
        int columnCountMax = 0;
        for(rowCount = 0;fgets(buffer, sizeof(buffer), file) != nullptr;++rowCount){
            int columnCount = 0;
            E* line = new E[matrix_max_size];
            char* token = strtok(buffer," "); //不能正则的。而且还是个指针，只能一步步走。
            while(token!=nullptr){
                if (std::is_same<E, double>::value){  //泛型对具体类型执行不同操作.
                    //不是fscanf，是sscanf
                    sscanf(token, "%lf", &element);
                } else{
                    sscanf(token, "%f", &element);
                }
//                lines[rowCount][columnCount++] = element;//栈上数组不可取。
                line[columnCount++] = element;
                token = strtok(nullptr, " ");
            }
            lines[rowCount] = line; //一行的数据读完了。
            if (columnCount>columnCountMax)
                columnCountMax = columnCount;
        }
        fclose(file);
        return new Matrix<E>{lines, rowCount, columnCountMax};
    }
    template<typename E>
    void writeMatrixToFile(const Matrix<E>* matrix, const char* filename)
    {
        FILE* file = fopen(filename, "w");
        for (int i = 0; i < matrix->rowCount; ++i) {
            int j;
            for (j = 0; j < matrix->columnCount-1; ++j) {
//                fprintf("%f ", matrix)
                if (std::is_same<E, double>::value){  //泛型对具体类型执行不同操作
                    fprintf(file, "%.1lf ", matrix->data[i][j]);
                } else{
                    fprintf(file, "%.1f ", matrix->data[i][j]);
                }
            }
            if (std::is_same<E, double>::value){  //泛型对具体类型执行不同操作
                fprintf(file, "%.1lf\n", matrix->data[i][j]);
            } else{
                fprintf(file, "%.1f\n", matrix->data[i][j]);
            }
        }
        fclose(file);
    }
     template<typename E>
     void file_matrix_multiplication(char* in_out_filenames[], E type) //为什么不能是const
     {
         Matrix<E> matrix_a = *readMatrixFromFile(in_out_filenames[0],type);
         Matrix<E> matrix_b = *readMatrixFromFile(in_out_filenames[1],type);
         Matrix<E>* matrix_c = matrix_a*matrix_b;
         writeMatrixToFile(matrix_c, in_out_filenames[2]);
         delete matrix_c;
     }
}

