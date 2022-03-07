//
// Created by 叶璨铭 on 2021/10/15.
//
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifdef WIN32
//windows
#include <Windows.h>
#define str_case_cmp _stricmp
#define str_cat strcat
#define str_cpy strcpy
#define _CRT_SECURE_NO_WARNINGS
#elif __linux__
// linux
#define str_case_cmp strcasecmp
#define str_cat strcat
#define str_cpy strcpy
#endif
#include "matrix_multiplication.h"
#include "util.h"
char* multiplication_mode_patterns[6] = {"trivial","openblas","strassen","simd","openmp","combined"};
void show_help()
{
    //C语言不支持R"xx()xx"
    //但是两个字符串可以拼在一起，编译时处理
    puts("用法: ./matmul {input1.txt} {input2.txt} {output.txt} [-mode trivial|openblas|strassen|simd|openmp|combined] [-help]");
    puts("功能: 从input1.txt和input2.txt读入两个矩阵matA和matB，计算矩阵乘法matA*matB，并将结果放入output.txt中。");
    puts("选项:");
    puts("-mode:  指定矩阵乘法的运算策略。可以为{TRIVIAL,OPENBLAS,STRASSEN,SIMD,CUDA,SPARSE}，不区分大小写");
    puts("   trivial已经包含了默认的内存连续性优化, 但是不包括openmp的使用。基本上是上一个Project的C版本移植。\n"
                "   openblas调用OpenBlas库来计算。是性能优化的目标。\n"
                "   strassen使用strassen算法来计算。\n"
                "   simd使用了avx2指令集来计算。暂不支持不是某些尺寸不是8的倍数的矩阵。\n"
                "   openmp使用了OpenMP并行接口来多线程计算。\n"
                "   combined尽可能结合以上模式的优点，提供一个接近于openblas的模式。\n"
                "   未来的计算可能还支持cuda和sparse模式。\n"
                "   cuda使用cuda指令来计算。sparse对于稀疏矩阵进行一定的优化。");
    puts("-help:  显示帮助信息。");
    puts("-t:  统计计算用时。注意");
}
void error(const char*message, const char* argv0, const char* level)
{
    char buf[150];
    str_cpy(buf, argv0);
    str_cat(buf, level);
    str_cat(buf,message);
    puts(buf);
}
void fatalError(const char *message, const char* argv0)
{
    error(message, argv0, ": Fatal Error: ");
    puts("matrix multiplication terminated. ");
    show_help();
}
void optionError(const char *message, char* argv0)
{
    error(message, argv0, ": Invalid arguments: ");
}
int main(int argc, char* argv[])
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    enum multiplication_mode computationMode;
    int k = 0;
    char *in_out_filenames[3]; //连续的三个char*，那么这三个char*需要malloc，如果在原地是不行的
    bool timeBench = false;
    for (size_t i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        if(arg[0]=='-')
        {
            if(strlen(arg)>1)
                switch (arg[1]) {
                    case 'h':case 'H': {
                        show_help();
                        return 0;
                    }
                    case 't':case 'T':{
                        timeBench = true;
                        continue;
                    }
                    case 'm':case 'M': {
                        arg = argv[++i];
                        computationMode = 0;
                        for (size_t j = 0; j < 6; j++)
                        {
                            if (str_case_cmp(arg, multiplication_mode_patterns[j]) == 0)//不加0，后果严重
                            {
                                computationMode = j;
                                break;
                            }
                        }
                        continue;
                    }
                    case '-': {
                        char *temp = &arg[2];
                        char buf1[40];
                        str_cpy(buf1, "unrecognized command-line option: --");
                        str_cat(buf1, temp);
                        optionError(buf1, argv[0]);
                        char buf2[40];
                        str_cpy(buf2, "Did you mean -");
                        str_cat(buf2, temp);
                        puts(buf2);
                        puts("Matrix multiplication terminated. ");
                        return -1;
                    }
                    default:{
                        char buf[20];
                        str_cpy(buf, "No option named -");
                        str_cat(buf, &arg[1]);
                        optionError(buf, argv[0]);
                        puts("Matrix multiplication terminated. ");
                        return -1;
                    }
                }
            else{
                optionError("invalid option: -", argv[0]);
                return -1;
            }
        }else
        {
            if (k<3)
                in_out_filenames[k++] = arg;
            else
            {
                fatalError("Too many arguments!", argv[0]);
                return -1;
            }
        }
    }
    if(k<3)
    {
        fatalError("Too few arguments!", argv[0]);
        return -1;
    }
    if(timeBench){
        TimerStart;
    }
    file_matrix_multiplication(in_out_filenames[0],in_out_filenames[1], in_out_filenames[2], computationMode);
    if(timeBench){
        TimerEndPrintln(multiplication_mode_patterns[computationMode]);
    }
}


