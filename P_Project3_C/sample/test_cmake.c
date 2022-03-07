//
// Created by 叶璨铭 on 2021/10/19.
//
#include <stdio.h>
#include "util.h"
int main(){
    FILE * f = fopen("D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\CMakeLists.txt", "r");
//    printf("%zd",
//           file_len(f));
//    puts("用法: ./matmul {input1.txt} {input2.txt} {output.txt} [-mode trivial|cuda|sparse] [-help]");
//    puts("功能: 从input1.txt和input2.txt读入两个矩阵matA和matB，计算矩阵乘法matA*matB，并将结果放入output.txt中。");
//    puts("选项:");
//    puts("    -mode:  指定矩阵乘法的运算策略。可以为{TRIVIAL,OPENBLAS,STRASSEN,SIMD,CUDA,SPARSE}，不区分大小写");
//    puts("      trivial已经包含了默认的多线程、内存连续性等优化, 是上一个Project的C版本移植。\n"
//         "      cuda使用cuda指令来计算。\n"
//         "      sparse对于稀疏矩阵进行一定的优化。\n");
//    puts("    -help:  显示帮助信息。");

    // 会到end吗？ 指针只是复制了一份指针
    char buffer[1024];
    fgets(buffer, 1024, f);
    printf("%s\n", buffer);
    //答案，不会！
    fseek(f,0L, SEEK_END);//不可饶恕的修改
    fgets(buffer, 1024, f);
    printf("%s\n", buffer);
    //那怎么办？
    f = freopen("D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project3_C\\CMakeLists.txt", "r", f);
    fseek(f,0L, SEEK_SET);
    fgets(buffer, 1024, f);
    fgets(buffer, 1024, f);
    printf("%s\n", buffer);
}