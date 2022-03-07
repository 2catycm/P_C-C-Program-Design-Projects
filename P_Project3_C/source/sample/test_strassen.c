//
// Created by 叶璨铭 on 2021/10/22.
//
#include <time.h>
#include "matrix_multiplication.h"
#define SIZE_OF_PATH "2048"
#define pathA "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-A-" SIZE_OF_PATH ".txt"
#define pathA2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-A-2.txt"
#define pathB "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-B-" SIZE_OF_PATH ".txt"
#define pathB2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-B-2.txt"
#define out_path "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-B-" SIZE_OF_PATH ".txt"
#define out_path2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-A-2.txt"
#define pathC "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-C-" SIZE_OF_PATH ".txt"
#define pathC2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-C-2.txt"
//time_t start;
//#define CLOCK_START (start = time(NULL))
//#define CLOCK_END(msg) (printf(msg": %f""s\n", difftime(time(NULL),start)))
clock_t start;
#define CLOCK_START (start = clock())
#define CLOCK_END(msg) (printf(msg": %ld""ms\n", (clock()-start)))
//    stringBuilderToString(trivial_matrix_to_string())
int main(){
    struct TrivialMatrix* a = read_trivial_matrix_from_file(pathA);
    struct TrivialMatrix* b = read_trivial_matrix_from_file(pathB);
    struct TrivialMatrix* c = matmul_strassen(a, b);
    c = matmul_strassen(a, b);
//    CLOCK_START;
//    c = matmul_strassen(a, b);
////    struct TrivialMatrix* c = matmul_trivial(a, b);
//    CLOCK_END("matmul_strassen");
//    CLOCK_START;
//    c = matmul_trivial(a, b);
//    CLOCK_END("matmul_trivial");
//512 测试
    //21s 512 阈值300
    //17s O2
    //15s trivial O2

    //没有openmp  0s trivial O2
    //没有openmp 0s strassen O2
//2048测试
    //没有openmp  10s trivial O2
    //没有openmp  8s strassen O2

//    CLOCK_START;
//    c = matmul_simd(a, b);
//    //    struct TrivialMatrix* c = matmul_trivial(a, b);
//    CLOCK_END("matmul_simd");
//    //512 快5倍
//    //2048：matmul_trivial: 31830ms
//    //matmul_simd: 32353ms

    //新版strassen
    CLOCK_START;
    c = matmul_strassen(a, b);
//    struct TrivialMatrix* c = matmul_trivial(a, b);
    CLOCK_END("matmul_strassen");
    // 2048下：matmul_trivial: 31813ms
    // matmul_strassen: 6883ms大有进步  //O2优化

    CLOCK_START;
    c = matmul_openmp(a, b);
    CLOCK_END("matmul_openmp");
    //终于用对了 512下
    // matmul_trivial: 533ms
    // matmul_simd: 75ms
    // matmul_openmp: 53ms
    //matmul_trivial: 31764ms
    //matmul_strassen: 2621ms
    //matmul_openmp: 2624ms
    CLOCK_START;
    c = matmul_openmp2(a, b);
    CLOCK_END("matmul_openmp2");
//matmul_strassen: 5562ms
//        matmul_openmp: 2812ms
//        matmul_openmp2: 2442ms
    CLOCK_START;
    c = matmul_simd_openmp2(a, b);
    CLOCK_END("matmul_simd_openmp");
//matmul_strassen: 7948ms
//        matmul_openmp: 9083ms
//        matmul_openmp2: 8277ms
//        matmul_simd_openmp: 19190ms
    write_trivial_matrix_to_file(c, pathC);
}