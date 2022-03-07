//
// Created by 叶璨铭 on 2021/9/28.
//
#include <iostream>
#include <omp.h>
void nested_test(){
    using namespace std;
    int sum[10'00][10'00] = {};
#pragma omp parallel for
    for (int i = 0; i < 10'00; ++i) {
#pragma omp parallel for
        for (int j = 0; j < 10'00; ++j){
//            cout<<"i = "<<i<<" j = "<<j<<endl;
#pragma omp atomic
            sum[i][j] += i*j;
        }
    }
//    cout<<"sum = "<<sum<<endl;
}
int main(){
    nested_test();
//#pragma omp parallel
//{
//    std::cout<<"Hello world!"<<std::endl;
//}
//#pragma omp parallel
//    {
//        std::cout <<"Hello world!" << std::endl; //cout似乎无效
//    }
//    int sum = 0;
//#pragma omp parallel for
//    for (int i = 0; i < 100; ++i) {
//        sum+=i;
//    }
//    std::cout<<sum << std::endl;//结果错误

//    int sum = 0;
//#pragma omp parallel for
//    for (int i = 0; i < 100; ++i) {
//        sum+=i;
//    }
//#pragma omp barrier
//    std::cout<<sum << std::endl;//结果依然错误

//    int sum = 0;
//#pragma omp parallel for
//    for (int i = 1; i <= 100; ++i) {
//#pragma omp atomic
//        sum+=i;
//    }
//    std::cout<<sum << std::endl;//结果正确！ //不需要指定barrier
//
//
//    sum = 0;
//#pragma omp parallel for
//    for (int i = 1; i <= 100; ++i) {
//#pragma omp atomic
//        sum+=i;
//    }
//#pragma omp barrier
//    std::cout<<sum << std::endl;

}