//
// Created by 叶璨铭 on 2021/9/26.
//
#include <cstdio>
int main(int argc, char **argv){
    int i = 0;
    scanf_s("%d", &i); //仅msvc支持。gcc不支持。c11标准支持。是边界检查的最快实现。
    printf("%d",i);
}
