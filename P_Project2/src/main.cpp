
#include <cstdio>
#include <cstring>
#include <iostream>
#include "matrix_multiplication.hpp"

#ifdef WIN32
//windows
#include <Windows.h>
#define str_case_cmp _stricmp
#define str_cat strcat_s
#define str_cpy strcpy_s
#define _CRT_SECURE_NO_WARNINGS
#elif __linux__
// linux
#define str_case_cmp strcasecmp
#define str_cat strcat
#define str_cpy strcpy
#endif
namespace matrix_multiplication_main
{
    void show_help()
    {
        puts(
R"xx(用法: ./file_matrix_multiplication {input1.txt} {input2.txt} {output.txt} [-type float|double] [-help]
        选项:
            -type:  指定运算对象的类型是浮点数还是双精度浮点数。本程序默认是双精度浮点数。
            -help:  显示帮助信息。 )xx"); //msvc编译器会有问题，因为msvc的编码是gbk，对utf-8代码页识别不良。改下cmakelist就好
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
    bool float_mode = false;
    int main(int argc, char* argv[])
    {
        int k = 0;
        char *in_out_filenames[3];
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
                    case 't':case 'T': {
                            arg = argv[++i];
                            float_mode = str_case_cmp(arg, "float") == 0 || str_case_cmp(arg, "f") == 0;
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
//        matrix_multiplication::file_matrix_multiplication(in_out_filenames, float_mode ? 1.0f : 1.0);//三目表达式有问题
        if(float_mode)
            matrix_multiplication::file_matrix_multiplication<float>(in_out_filenames);
        else
            matrix_multiplication::file_matrix_multiplication<double>(in_out_filenames);
        return 0;
    }
}

int main(int argc, char*argv[])
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    return matrix_multiplication_main::main(argc, argv);
}
