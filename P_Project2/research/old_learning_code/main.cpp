// #include <string
#include <cstdio>
#include <cstring>

#ifdef WIN32
    //windows
#include <Windows.h>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define str_case_cmp _stricmp
#elif __linux__
    // linux
    #define str_case_cmp strcasecmp
#endif

#include "matrix_multiplication.cpp"
namespace matrix_multiplication
{
    void show_help()
    {
//        puts(
//R"xx(用法: ./file_matrix_multiplication (input1.txt) (input2.txt) (output.txt) [-type float|double] [-mode c++] [-help]
//        选项:
//            -type:  指定运算对象的类型是浮点数还是双精度浮点数。本程序默认是双精度浮点数。)xx"); //是R有问题，R把那个。识别不出来！
        puts("用法: ./file_matrix_multiplication (input1.txt) (input2.txt) (output.txt) [-type float|double] [-mode c++] [-help]");
        puts("选项:");
//        puts("\t-type:  指定运算对象的类型是浮点数还是双精度浮点数。本程序默认是双精度浮点数。");   //字符串字面量可以拼接? 到底可不可以？
//        std::cout<<"\t-type:  指定运算对象的类型是浮点数还是双精度浮点数。本程序默认是双精度浮点数。";
//        printf(
//                R"(Usage: ./file_matrix_multiplication (input1.txt) (input2.txt) (output.txt) [-type float|double] [-mode c++] [-help]
//        Options:
//            -type:  指定运算对象的类型是浮点数还是双精度浮点数。本程序默认是双精度浮点数。
//        )");
    }
    void error(char*message, char* argv0, char* level)
    {
        //                    puts();
//                    puts(strcat(argv[0], ": Fatal Error: too many input output arguments!")); //bad!
        char buf[150];
        strcpy_s(buf, argv0); //对的
        strcat_s(buf, ": Fatal Error: "); //挂了
        strcat_s(buf,message);
        puts(buf);
        show_help();
    }
    void fatalError(char *message, char* argv0)
    {
        error(message, argv0, ": Fatal Error: ");
    }
    bool float_mode = false;
    int main(int argc, char* argv[])
    {
        int k = 0;
        char *in_out_filenames[3];
        for (size_t i = 1; i < argc; i++)
        {
            char* arg = argv[i];
//            if (str_case_cmp(arg, "-help") == 0 || str_case_cmp(arg, "-h") == 0)
//            {
//                show_help();
//                return 0;
//            }else if (str_case_cmp(arg, "-type") == 0 || str_case_cmp(arg, "-t") == 0)
//            {
//                arg = argv[++i];
//                float_mode = str_case_cmp(arg, "float") == 0 || str_case_cmp(arg, "f") == 0;
//            } else
            if(arg[0]=='-')
            {
                if(strlen(arg)>1)
                    switch (arg[1]) {
                    case 'h':case 'H':
                        show_help();
                        return 0;
                    case 't':case 'T':
                        arg = argv[++i];
                        float_mode = str_case_cmp(arg, "float") == 0 || str_case_cmp(arg, "f") == 0;
                        continue;
                    case '-':
                        char* temp = &arg[2];
                        char buf[100];
                        strcpy_s(buf, "unrecognized command-line option: --");
                        strcat_s(buf, temp);
                        strcat_s(buf,"\n");
                        strcat_s(buf,"Did you mean -");
                        strcat_s(buf,temp);
                        puts(buf);
                        puts("matrix multiplication terminated. ");
                        return -1;
                    }
                else{
                    puts("invalid option: -");
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

//        for (auto & in_out_filename : in_out_filenames) {
//            printf("%s\n", in_out_filename);
//        }
        file_matrix_multiplication(in_out_filenames, float_mode ? 1.0f : 1.0);
//        file_matrix_multiplication(in_out_filenames, 1.0f);
//        if (float_mode)
//            file_matrix_multiplication_float(in_out_filenames);
//        else
//            file_matrix_multiplication_double(in_out_filenames);
        return 0;
    }
}

int main(int argc, char*argv[])
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    return matrix_multiplication::main(argc, argv);
}
