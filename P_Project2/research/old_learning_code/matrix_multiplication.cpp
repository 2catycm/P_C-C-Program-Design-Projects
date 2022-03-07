//
// Created by 叶璨铭 on 2021/9/25.
//

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
        ~Matrix(){
            for (int i = 0; i < rowCount; ++i) {
                delete [] data[i];
            }
            delete[] data;
        }
    };

//    template<typename E>
//    E _dot_product(const E*& vector_a, const E*& vector_b){
//
//    }
    template<typename E>
    Matrix<E>* _trivial_muliplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b){
        if (matrix_a.columnCount != matrix_b.rowCount)
            throw std::runtime_error("Matrix multiplication of whose sizes do not match is not supported");
        //初始化二维数组
        E** result = new E*[matrix_a.rowCount];
        for (int i = 0; i < matrix_a.rowCount; ++i) {
            result[i] = new E[matrix_b.columnCount];
        }
        for (int i = 0; i < matrix_a.rowCount; ++i) {
            for (int j = 0; j < matrix_b.columnCount; ++j) {
                E sum = 0;
                for (int k = 0; k < matrix_a.columnCount; ++k) {
                    sum += matrix_a.data[i][k]*matrix_b.data[k][j];
                }
                result[i][j] = sum;
            }
        }
        return new Matrix<E>{result, matrix_a.rowCount, matrix_b.columnCount};
    }
    template<typename E>
    Matrix<E>* operator*(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b)
    {
        return _trivial_muliplication(matrix_a, matrix_b);
    }
    template<typename E>
    Matrix<E>* readMatrixFromFile(char* filename, E type) //“谁说泛型没有代价？看吧，这就是人愚蠢的证据。“  //不能转换，也不能引用
    {
        FILE* file = fopen(filename, "r");
        int rowCount;
        char buffer[262144];//每一行

        if (file == nullptr) {
            throw std::runtime_error("file not found.");
        }
//        E** data;//太愚蠢了，我哭了，我讨厌c语言。
//        E lines[matrix_max_size][matrix_max_size]; //假设有4096行 4096列 //栈上的空间会被释放掉。
//        E* lines[matrix_max_size]; //假设有4096行 4096列
        E** lines = new E*[matrix_max_size];
        E element; std::stringstream ss;
        int columnCountMax = 0;
        for(rowCount = 0;fgets(buffer, sizeof(buffer), file) != nullptr;++rowCount){
//            for (int i = 0, j = 0; i < strlen(buffer); ++i) {//fori使用strlen是愚蠢的选择，请用while not \0
//                if (buffer[i]!=' ')
//                    continue;
                //先复制出一个子数组，然后读取为double
//                char* temp = &buffer[i];//指针右移到i，表示读取完成
//                sscanf()
            int columnCount = 0;
//            E line[4096];
            E* line = new E[matrix_max_size];
            char* token = strtok(buffer," "); //不能正则的。而且还是个指针，只能一步步走。
            while(token!=nullptr){
//                //下面，我们实在是难以忍受c语言了，改用c++来写吧!为了泛型干杯!
//                ss<<token;
//                ss>>element;
                if (std::is_same<E, double>::value){  //泛型对具体类型执行不同操作.
                    //不是fscanf，是sscanf
                    sscanf(token, "%lf", &element);
                } else{
                    sscanf(token, "%f", &element);
                }
//                lines[rowCount][columnCount++] = element;//栈上数组不可取。
                line[columnCount++] = element;
                token = strtok(nullptr, " ");
//                }
            }
//            lines[rowCount] = line; //不加E*就不行
//            lines[rowCount] = static_cast<E*>(line); //不加E*就不行 //不能这样搞，这样搞每次地址都一样。
            lines[rowCount] = line;

            if (columnCount>columnCountMax)
                columnCountMax = columnCount;
        }
        fclose(file);
        return new Matrix<E>{lines, rowCount, columnCountMax};
    }
    template<typename E>
    void writeMatrixToFile(const Matrix<E>* matrix, char* filename)
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
     }
    
//    void file_matrix_multiplication_float(char* in_out_filenames[])
//    {
//        Matrix<float> matrix_a = readMatrixFromFile(in_out_filenames[0],1.0f);
//        Matrix<float> matrix_b = readMatrixFromFile(in_out_filenames[1],1.0f);
//        Matrix<float>& matrix_c = matrix_a;
//        writeMatrixToFile(matrix_c, in_out_filenames[2]);
//    }
//    void file_matrix_multiplication_double(char* in_out_filenames[])
//    {
//        Matrix<double> matrix_a = readMatrixFromFile(in_out_filenames[0],1.0);
//        Matrix<double> matrix_b = readMatrixFromFile(in_out_filenames[1],1.0);
//        Matrix<double>& matrix_c = matrix_a;
//        writeMatrixToFile(matrix_c, in_out_filenames[2]);
//    }
}

