//
// Created by 叶璨铭 on 2021/9/29.
//
#include <iostream>
#include <sstream>
#include <benchmark/benchmark.h>
#include "matrix_multiplication.hpp"
#define matrix_type float
using namespace matrix_multiplication;
////测试乘法所需时间
//static void testMultiplication_trivial(benchmark::State& state){
////    数据准备
//    int size = state.range(0);
//    char path_a_in[256];
//    char path_b_in[256];
//    char path_c_out[256];
//    sprintf(path_a_in,"./data/mat-A-%d.txt", size);
//    sprintf(path_b_in,"./data/mat-A-%d.txt", size);
//    sprintf(path_c_out,"./data/mat-C-%d.txt", size);
//    Matrix<matrix_type> matrix_a = *readMatrixFromFile<matrix_type>(path_a_in);
//    Matrix<matrix_type> matrix_b = *readMatrixFromFile<matrix_type>(path_b_in);
//    Matrix<matrix_type>* matrix_c;
////    开始测试
//    for (auto _: state) {
//        matrix_c = _trivial_multiplication(matrix_a,matrix_b);
//    }
//    writeMatrixToFile(matrix_c, path_c_out);
//    delete matrix_c;
//}
//BENCHMARK(testMultiplication_trivial)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);
//
////测试乘法所需时间
//static void testMultiplication_continuous(benchmark::State& state){
////    数据准备
//    int size = state.range(0);
//    char path_a_in[256];
//    char path_b_in[256];
//    char path_c_out[256];
//    sprintf(path_a_in,"./data/mat-A-%d.txt", size);
//    sprintf(path_b_in,"./data/mat-A-%d.txt", size);
//    sprintf(path_c_out,"./data/mat-C-%d.txt", size);
//    Matrix<matrix_type> matrix_a = *readMatrixFromFile<matrix_type>(path_a_in);
//    Matrix<matrix_type> matrix_b = *readMatrixFromFile<matrix_type>(path_b_in);
//    Matrix<matrix_type>* matrix_c;
////    开始测试
//    for (auto _: state) {
//        matrix_c = _continuous_multiplication(matrix_a,matrix_b);
//    }
//    writeMatrixToFile(matrix_c, path_c_out);
//    delete matrix_c;
//}
//BENCHMARK(testMultiplication_continuous)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);

//测试乘法所需时间
static void testMultiplication(benchmark::State& state){
//    数据准备
    int size = state.range(0);
    char path_a_in[256];
    char path_b_in[256];
    char path_c_out[256];
    sprintf(path_a_in,"./data/mat-A-%d.txt", size);
    sprintf(path_b_in,"./data/mat-A-%d.txt", size);
    sprintf(path_c_out,"./data/mat-C-%d.txt", size);
    Matrix<matrix_type> matrix_a = *readMatrixFromFile<matrix_type>(path_a_in);
    Matrix<matrix_type> matrix_b = *readMatrixFromFile<matrix_type>(path_b_in);
    Matrix<matrix_type>* matrix_c;
//    开始测试
    for (auto _: state) {
        matrix_c = _concurrent_continuous_multiplication(matrix_a,matrix_b);
    }
    writeMatrixToFile(matrix_c, path_c_out);
    delete matrix_c;
}
BENCHMARK(testMultiplication)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);

////测试读入一个矩阵所需时间
//static void testInput(benchmark::State& state){
////    数据准备
//    int size = state.range(0);
//    char path_a_in[256];
//    sprintf(path_a_in,"./data/mat-A-%d.txt", size);
////    开始测试
//    for (auto _: state) {
//        Matrix<matrix_type> matrix_a = *readMatrixFromFile<matrix_type>(path_a_in);
//    }
//}
//BENCHMARK(testInput)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);
//
////测试写出一个矩阵所需时间
//static void testOutput(benchmark::State& state){
////    数据准备
//    int size = state.range(0);
//    char path_a_in[256];
//    char path_a_out[256];
//    sprintf(path_a_in,"./data/mat-A-%d.txt", size);
////    std::stringstream _path_a_out;
////    _path_a_out<<"./data/mat-A-"<<size<<"_output_test.txt";
////    char * path_a_out = _path_a_out.str().c_str();
//    sprintf(path_a_out,"./data/mat-A-%d_output_test.txt", size);
//    Matrix<matrix_type>* matrix_a = readMatrixFromFile<matrix_type>(path_a_in);
////    开始测试
//    for (auto _: state) {
//        writeMatrixToFile(matrix_a, path_a_out);
//    }
//}
//BENCHMARK(testOutput)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);
BENCHMARK_MAIN();