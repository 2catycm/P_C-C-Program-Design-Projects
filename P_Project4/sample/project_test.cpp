//
// Created by 叶璨铭 on 2021/11/23.
//
#include <iostream>
#include <fstream>
#include <vector>
void read();
//#include "sustechdef.hpp"
#include "mat.hpp"
//#include "types.hpp"
//#include <array>
//using VarScalar = std::vector<double>;
//std::ostream& operator<<(std::ostream& os, const VarScalar& vec){
//    for(const auto& e:vec)
//        os << e;
//    return os;
//}
int main(){
//    size_t size = 32;
//    char path_a_in[256];
//    sprintf(path_a_in,"../resources/input_data/mat-A-%d.txt", size);
//    std::ifstream fA{path_a_in};
//    SUSTech::Mat matA {size/4, size/4, SUSTech::SUSTech_64FC4};
//    fA>>matA;
//    std::cout<<matA;
//    read();
//    VarScalar matA{1,2,3};
//    std::cout<<matA<<std::endl;


    std::cout<<(int)SUSTech::SUSTech_32FC<<std::endl;
    SUSTech::Mat matA;
//    SUSTech::Scalar a{1};
//    std::array<double, 4> matA{1,2,4,6};
//    SUSTech::operator<<(std::cout, matA)<<std::endl;
    std::cout<< matA <<std::endl;

//    SUSTech::Rect b{1,2, 3,3};
//    std::cout<< b<<std::endl;
//    std::cout<< b.area()<<std::endl;

//    std::cout<<SUSTech::Mat::dims<<std::endl;

    SUSTech::Point2i a = {1,2};
    std::cout<< matA <<std::endl;
    auto b = matA;
    b.y = 3;
    std::cout<< matA <<std::endl;
    auto c = std::move(matA);
    c.y = 3;
//    std::cout<<matA<<std::endl;
//    std::cout<<c<<std::endl;
}
void read() {
    auto matA = SUSTech::Mat::matRead(R"(D:\EnglishStandardPath\Practice_File\P_C_Cpp\P_Project3_C\resources\input_data\mat-A-2.txt)");
    auto matB = SUSTech::Mat::matRead(R"(D:\EnglishStandardPath\Practice_File\P_C_Cpp\P_Project3_C\resources\input_data\mat-B-2.txt)");
    std::cout<<matA<<std::endl;
    std::cout<<matB<<std::endl;
//    std::cout<<matA[0]<<std::endl;
//    std::cout<<matA[1][1]<<std::endl;
//    matA+=matA;
//    std::cout<<matA+matA<<std::endl;
    std::cout<<matA.matMul(matB)<<std::endl;

}
