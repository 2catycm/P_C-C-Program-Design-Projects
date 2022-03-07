//
// Created by 叶璨铭 on 2021/9/25.
//
//首先，这个文件时utf-8存储的源代码
#include <cstring>
#include<string>
#include<cstdio>
#include<iostream>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **argv){
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using namespace std;
    puts("我是中文\n"); //字符串
    cout<<"我才是中文"<<endl;

//    char a = '中';//中不是char类型的，认为是一个大整数
//    unsigned char a = '中';//仍然不够
    char16_t 中 = u'中';
    cout<<sizeof(中)<<endl;
    cout<<中<<endl;
    printf("我是char类型的输出，%c;\n而我，是int类型的：%d; 你说，我的二进制是什么样的?我打不出来", 中, 中);
    printf("其实，我才是char16_t类型的输出，%u;\n", 中);
//    cout<<"其实，我才是char16_t类型的输出: "<<string(&中)<<endl; //无法构造！
//    cout<<"其实，我才是char16_t类型的输出: "<<static_cast<string>(&中)<<endl;
    cout<<"其实，我才是char16_t类型的输出: "<<to_string(static_cast<char>(中))<<endl;


    char a = 'A';
    printf("%x %c\n\n", a, a); //correct！

    printf("%x %s", 中, &中); //correct！

}