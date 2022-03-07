#include <iostream>
#include <opencv2/opencv.hpp>
#include <array>
void channel_test();
void constructor();
void scalar_vec_matx();
void new_opencv();
void roi();
void mat_mul();
void moving();
using cv::Mat;
int main(int argc, char const *argv[])
{

    constructor();
//    channel_test();
//    scalar_vec_matx();
//    new_opencv();
//    roi();
//    mat_mul();
//    std::array<int, 4> a{1,2,3,4}; //没有构造函数
//    std::cout<<a[1]<<std::endl;
//    std::cout<<a[2]<<std::endl;
//    multidimension 暂时不写，opencv是支持的。

    moving();
    return 0;
}
cv::Mat newMat(){
    float a[6]={1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f};
    cv::Mat A(2, 3, CV_32FC1, a);
    return A;
}
void moving() {
    auto a = newMat();
    cv::Mat b;
    b = a;
    b = std::move(a);
}
void mat_mul() {
    float a[6]={1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f};
    float b[6]={1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    {
        cv::Mat A(2, 3, CV_32FC1, a);
        cv::Mat B(3, 2, CV_32FC1, b);
        cv::Mat D(3, 2, CV_32FC1, {1,2});

        cv::Mat C = A * B;

        std::cout << "Matrix C = " << std::endl << C << std::endl;
    }
    std::cout<<b[2]<<std::endl;
    std::cout<<b[3]<<std::endl;
}
void roi() {
    cv::Mat M(4,4, CV_32FC1, cv::Scalar(1));
    std::cout << M <<std::endl;
    M = {2};
    Mat M1 = M.col(1);
    M1 = {3};
    std::cout << M <<std::endl;

//    M1 = cv::Scalar(1);
    M.col(3).copyTo(M1);
    std::cout << M1 <<std::endl;
    // create a new 320x240 image
    // select a ROI

    Mat roi(M, cv::Rect(0,1,2,3));
    // the original image will be modified
    std::cout << roi <<std::endl;
    roi = cv::Scalar(2);
    std::cout << M <<std::endl;

    //Scalar是为了channel用的。
    roi = cv::Scalar(2,3);
    std::cout << M <<std::endl;
    roi = cv::Scalar(2,3, 4);
    std::cout << M <<std::endl;
    roi = cv::Scalar(2,3, 4, 5);
    std::cout << M <<std::endl;

}
void new_opencv() {
    cv::Mat_<float> a();
}
void scalar_vec_matx() {
//    //Matx
//    cv::Matx33f m(1, 2, 3,
//              4, 5, 6,
//              7, 8, 9);
//    std::cout << sum(Mat(m*m.t())) << std::endl;
//    float values[] = { 1, 2, 3};
//    cv::Matx31f q(values);
//    std::cout <<q<<std::endl;
//    std::cout <<q.t()<<std::endl;
//    std::cout <<q.reshape<1, 3>()<<std::endl;
////    std::cout <<q.reshape<3, 2>()<<std::endl;//静态检查出来了

    //Vec
    //The vectors are allocated on stack, as opposite to std::valarray, std::vector, cv::Mat etc., which elements are dynamically
    //It is possible to convert Vec<T,2> to/from Point_, Vec<T,3> to/from Point3_ , and Vec<T,4> to CvScalar or Scalar_. Use operator[] to access the elements of Vec.
    cv::Vec3f v(1, 2, 255);
    std::cout <<v<<std::endl;
    cv::Vec<float, 1000> a, b;
    for(int i = 0; i < 1000; ++i) {
        a[i] = i;
        b[i] = 999-i;
    }
    std::cout <<a+b<<std::endl;
    std::cout <<Mat(a.t())*Mat(b)<<std::endl;//点乘
//    std::cout <<Mat(a)*Mat(b.t())<<std::endl;
}
void constructor() {
    //这都是二维的矩阵，但是支持多通道。我觉得足够了。
    Mat a(2,3, CV_32FC3);
    a = cv::Scalar(3.f, 1.5f, 4.8f);
    std::cout <<"a: "<< a<<std::endl;

    double b[6]={1.0, 1.0, 1.0, 2.0, 2.0, 2.0};
    cv::Mat B(cv::Size {2,3}, CV_64FC1, b);
    std::cout <<"B: "<< B<<std::endl;
}
void channel_test() {
    Mat a(2,3, CV_32FC3, cv::Scalar(1.0f, 2.0f));
    Mat b(3,2, CV_16FC2, cv::Scalar(2.0f, 1.0f));
//    Mat c(a);
    Mat c(2,3, CV_32FC3, cv::Scalar(1.0f, 2.0f));
    std::cout <<"a: "<< a<<std::endl;
    std::cout <<"b: "<< b<<std::endl;
    std::cout <<"c: "<< c<<std::endl;
    //both following operations are forbidden.
//    std::cout<< Mat(a+a.clone()) <<std::endl;
        std::cout<< a.mul(c) <<std::endl;
//        std::cout<< a+c <<std::endl;
//    std::cout<< a*b <<std::endl;
}
