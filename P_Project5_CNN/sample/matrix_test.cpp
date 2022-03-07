//
// Created by 叶璨铭 on 2021/12/10.
//
#include <matrix/Matrix.h>
#include <opencv2/opencv.hpp>
void test1();
void test2();
void test3();
void test4();
int main() {
//    test1();
//    test2();
//    test3();
    test4();
    return 0;
}
void test4() {
    SUSTech::ChannelMajorMatrix2D<float> a{100, 100, {20}};
    SUSTech::ChannelMajorMatrix2D<float> b{100, 100, {80}};
    std::cout << a.inner_product(b) <<std::endl;
}
void test3() {
    using namespace SUSTech;
    cv::Mat cvMat{5, 5, CV_32FC1, cv::Scalar{19}};
    auto mat = ChannelMajorMatrix<float, 1>{std::move(cvMat)};
    std::cout << mat << std::endl;
}
void test2() {
    using namespace SUSTech;
    ChannelMajorMatrix<float, 1> mat1(3, 3, { -1 });
    ChannelMajorMatrix<float, 1> mat2(3, 3, { 2 });
//    ChannelMajorMatrix<float, 1> mat2(3, 3, { 0 });//ambiguous？
    ChannelMajorMatrix<float, 1> mat3(3, 3, { 1 });
    auto a = stackMatrices(std::array<ChannelMajorMatrix<float, 1>, 3>{ mat1, mat2, mat3 });
    std::cout<<a<<std::endl;
    auto b = a.copySplit();
//    auto b = a.sharedSplit();
    for(const auto& e:b) {
        std::cout<<e<<std::endl;
    }
}
void test1() {
    using namespace SUSTech;
    ChannelMajorMatrix<float, 2> mat(3, 3, { -1 });

    mat(0, 0, 1) = 3.14;


    std::cout << mat << std::endl;
    std::cout << mat.operator()({ 0, 2, 1 }, { 1, 2, 1 }) << std::endl;
    mat({ 0, 2, 1 }, { 1, 2, 1 }) = 15;
    std::cout << mat << std::endl;
}
