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
//#include <amp.h> //仅Windows可用 amp并发计算
#include <omp.h> //openmp 并发计算
namespace matrix_multiplication
{
    template<typename E>
    void file_matrix_multiplication(char* in_out_filenames[], E type);//对外暴露的接口只有这一个函数。其他实现隐藏。
}


#endif //P_PROJECT2_MATRIX_MULTIPLICATION_H
