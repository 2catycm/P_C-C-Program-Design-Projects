//
// Created by 叶璨铭 on 2021/9/25.
//
#include <cstdio>
#include <stdexcept>
int main(int argc, char **argv){
    FILE* file = fopen("test.txt", "r");//无法支持utf-8， c语言就是垃圾
    int rowCount;
    char buffer[4096]; //一行的长度，但是似乎也可以小于一行长度, 那样的话，当行剩下的将在下一次被读入。
    if (file == nullptr) {
        throw std::runtime_error("file not found.");
    }
    for(rowCount = 0;fgets(buffer, sizeof(buffer), file) != nullptr;++rowCount){
        printf(buffer);
        printf("\n");
    }
    fclose(file);
}