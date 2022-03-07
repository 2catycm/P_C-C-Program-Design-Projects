//
// Created by 叶璨铭 on 2021/10/19.
//
#include <time.h>
#include "matrix_multiplication.h"
#define pathA "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-A-2048.txt"
#define pathA2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-A-2.txt"
#define pathB "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-B-2048.txt"
#define pathB2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\input_data\\mat-B-2.txt"
#define out_path "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-B-2048.txt"
#define out_path2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-A-2.txt"
#define pathC "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-C-2048.txt"
#define pathC2 "D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\resources\\output_data\\mat-C-2.txt"

void test1();

void test2();

void test3();


int main(int argc, char **argv){
//    test1();
//    test2();
    //debug use:
//    stringBuilderToString(trivial_matrix_to_string())
    test3();
}
void test3() {
    struct TrivialMatrix* a = read_trivial_matrix_from_file(pathA);
    struct TrivialMatrix* b = read_trivial_matrix_from_file(pathB);
    clock_t t = clock();
    struct TrivialMatrix* c = matmul_trivial(a, b);//4033 for 1024   //31121 for 2048 openmp inside  //30869 for outside //30728 inside no lock //
    //明显是openmp没有用上
    printf("%ld\n",(clock()-t));
    write_trivial_matrix_to_file(c, pathC);
}

void test2() {
    struct TrivialMatrix* a = read_trivial_matrix_from_file(pathA);
    struct TrivialMatrix* b = read_trivial_matrix_from_file(pathB);
    clock_t t = clock();
    struct TrivialMatrix* c = matmul_blas(a, b);
    printf("%ld\n",(clock()-t));  //281 for 1024   //1106 for 2048
    write_trivial_matrix_to_file(c, pathC);
}

void test1() {
    clock_t t = clock();
    struct TrivialMatrix* a = read_trivial_matrix_from_file(pathA);
    printf("%d\n",(clock()-t)/*/CLOCKS_PER_SEC*/); //读3秒

    write_trivial_matrix_to_file(a, out_path);
    printf("%d\n",(clock()-t)/*/CLOCKS_PER_SEC*/); //读写15秒

    fprintf(fopen(out_path, "w"), "%s", stringBuilderToString(trivial_matrix_to_string(a)));
    printf("%d",(clock()-t)/*/CLOCKS_PER_SEC*/); //读写龟速，还不如上面那个
    fprintf_string_builder(stdout, trivial_matrix_to_string(a));
}

