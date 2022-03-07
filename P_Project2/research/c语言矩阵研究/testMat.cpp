//
// Created by 叶璨铭 on 2021/9/25.
//
#include <iostream>
int*& passTest(int* arg){
    int a[10] = {1,2,3,5,6,7,8};
    int*& b = a
    return a;
}
//int* passTest(int* arg){
////    int a[10] = {1,2,3,5,6,7,8,9,10,11,12};//error
//    int a[10] = {1,2,3,5,6};//error
//    return a;
//}
int main(int argc, char **argv){
//    int* a = new int[10]{};
    int* a = passTest(nullptr);
    for (int i = 0; i < 10; ++i) {
        std::cout <<a[i]<<std::endl;
    }
}