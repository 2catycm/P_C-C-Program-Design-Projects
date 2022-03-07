//
// Created by 叶璨铭 on 2021/9/25.
//
#include <cstdio>
int main(int argc, char **argv){
    char* buffer = "1234 567 890\n 124 124 123";
    while(true){
        int temp;
//        scanf("%d\n",&temp);//遇到\n才会读取
//        scanf("%d",&temp);//遇到\n会有反应，但是对于\n中的每一个空格隔开的数字都有反应，相当于是缓存了。回车只是一个刷新
//        printf("%d\n", temp);
//        char* buffer = "1234 567 890\n 124 124 123";
//        sscanf(buffer, "%d %d %d\n", &temp);//爆炸了
        sscanf(buffer, "%d\n", &temp);
                printf("%d\n", temp); //卡的不行
    }
}