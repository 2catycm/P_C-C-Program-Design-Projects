//
// Created by 叶璨铭 on 2021/9/25.
//

#ifndef P_PROJECT2_MATRIX_MULTIPLICATION_H
#define P_PROJECT2_MATRIX_MULTIPLICATION_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <type_traits> //判断模板的类型
//#include <amp.h>
namespace matrix_multiplication
{
     template<typename E>
     void file_matrix_multiplication(char* in_out_filenames[], E type);
//    template<typename E>
//    void file_matrix_multiplication(char* in_out_filenames[], double type);
//    template<typename E>
//    void file_matrix_multiplication(char* in_out_filenames[], float type);

//    void file_matrix_multiplication_float(char* in_out_filenames[]);
//    void file_matrix_multiplication_double(char* in_out_filenames[]);
}


#endif //P_PROJECT2_MATRIX_MULTIPLICATION_H
