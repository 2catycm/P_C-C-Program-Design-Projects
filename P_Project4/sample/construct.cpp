//
// Created by 叶璨铭 on 2021/11/26.
//
#include <iostream>
#include "mat.hpp"
//template SUSTech::Mat& SUSTech::Mat::operator=<float, 3>(const std::array<float, 3>);
//    std::cout<<SUSTech::SUSTech_MAT_DEPTH_MASK<<std::endl;
//    int d = 2; int cn = 3;
//    std::cout<<SUSTech::SUSTech_MAKE_TYPE(d, cn)<<std::endl;
//    std::cout<<SUSTech_32FC3<<std::endl;
using namespace SUSTech;

int main(){
    SUSTech::Mat matA;
    SUSTech::Mat matB {3, 3, SUSTech::SUSTech_32FC3, {1,2,3}};
    SUSTech::Mat matC = std::move(matB);


    std::cout<<(matC((size_t)0,(size_t)1))<<std::endl;
    std::cout<<(matC(0,1))<<std::endl;
    std::cout<<matC<<std::endl;

//    SUSTech::Mat matD(matC, {0,1, 2, 1});
    SUSTech::Mat matD(matC, {0,1, 1, 2});
    std::cout<<matD<<std::endl;


//    matD.SUSTech::Mat::operator= (std::array<float, 3> {3,2,1});  //link error 离谱
    matD.SUSTech::Mat::operator= (std::array<double, 4> {3,2,1});
    std::cout<<matC<<std::endl;
    SUSTech::Mat matE(matD, {0,0,1,1});
    matE = Scalar{100,200,300};
    std::cout<<matC<<std::endl;

    //
    matC[0][0] = Scalar{50, 50, 50};
    std::cout<<matC<<std::endl;

    std::cout<<matC.channel(1)<<std::endl;
}