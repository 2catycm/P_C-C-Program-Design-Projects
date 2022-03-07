//
// Created by 叶璨铭 on 2021/9/27.
//
#include <cstdio>
int main(int argc, char **argv){
    FILE *file = fopen("../data/mat-A-32.txt", "r");
    while (char c = getc(file) != EOF){
        if (c==' ')
            putchar(',');
        else
            putchar(c);
    }
    fclose(file);
}