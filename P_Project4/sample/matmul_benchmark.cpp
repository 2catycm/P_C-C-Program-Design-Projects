#include <iostream>
#include <sstream>
#include <fstream>
#include <benchmark/benchmark.h>
#include "mat.hpp"
constexpr int MATRIX_DEPTH = SUSTech::SUSTech_32F;
constexpr int CHANNELS = 1;
constexpr int MATRIX_TYPE = SUSTech::SUSTech_MAKE_TYPE(MATRIX_DEPTH, CHANNELS);
static void readMatrix(benchmark::State& state){
    size_t size = state.range(0);
    char path_a_in[256];
    sprintf(path_a_in,"../resources/input_data/mat-A-%d.txt", size);
    std::ifstream fA{path_a_in};
    SUSTech::Mat matA {size, size/CHANNELS, MATRIX_TYPE};
    for (auto _: state) {
        fA>>matA;
    }
}
BENCHMARK(readMatrix)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);
static void writeMatrix(benchmark::State& state){
    size_t size = state.range(0);
    char path_a_in[256];
    char path_a_out[256];
    sprintf(path_a_in,"../resources/input_data/mat-A-%d.txt", size);
    sprintf(path_a_out,"../resources/output_data/mat-A-%d.txt", size);
    std::ifstream fA{path_a_in};
    std::ofstream fAo{path_a_out};
    SUSTech::Mat matA {size, size/CHANNELS, MATRIX_TYPE};         fA>>matA;
    for (auto _: state) {
        fAo<<matA;
    }
}
BENCHMARK(writeMatrix)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);

//测试乘法所需时间
static void testMultiplication(benchmark::State& state){
    //    数据准备
    size_t size = state.range(0);
    char path_a_in[256];
    char path_b_in[256];
    char path_c_out[256];
    sprintf(path_a_in,"../resources/input_data/mat-A-%d.txt", size);
    sprintf(path_b_in,"../resources/input_data/mat-A-%d.txt", size);
    sprintf(path_c_out,"../resources/output_data/mat-C-%d.txt", size);
    std::ifstream fA{path_a_in};
    SUSTech::Mat matA {size, size/CHANNELS, MATRIX_TYPE}; fA>>matA;
    std::ifstream fB{path_b_in};
    SUSTech::Mat matB {size, size/CHANNELS, MATRIX_TYPE}; fB>>matB;
    std::ofstream fC{path_c_out};
    SUSTech::Mat matC {};
    //    开始测试
    for (auto _: state) {
        matC = matA.matMul(matB);
    }
    fC<<matC;
}
BENCHMARK(testMultiplication)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(5)->Complexity(benchmark::oN);

BENCHMARK_MAIN();