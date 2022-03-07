//
// Created by 叶璨铭 on 2021/12/10.

#include <SUSTechDefinitions.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
int main() {
    //    Mat src = imread("resources\\samples\\bg.jpg");
    try {
        Mat src = imread("D:\\EnglishStandardPath\\Practice_File\\P_C_Cpp\\P_Project5_CNN\\resources\\samples\\bg.jpg");
//        src.convertTo(src, CV_32F, 255, 255);
//        imshow("test", src);
//        waitKey(0);
//        abort();
        SUSTech::sustechDynamicAssert(!src.empty());
        namedWindow("src", WINDOW_AUTOSIZE);
        imshow("src", src);
        namedWindow("conv", WINDOW_AUTOSIZE);
        Mat kernel{3,3, CV_32FC3, cv::Scalar(1.f/9, 1.f/9, 1.f/9)};
        kernel.reshape(0,9); //第一个参数是channel，第二个参数是rows
        std::cout << kernel << std::endl;
        Mat dst = src.clone();
        for(int i = 1; i < dst.rows-1; ++i) {
            for(int j = 1; j < dst.cols-1; ++j) {
                Mat roi = dst(cv::Range(i-1, i+2), cv::Range(j-1, j+2)).clone();
                std::cout << roi << std::endl;
                roi.reshape(0,1);
                SUSTech::sustechDynamicAssert(roi.rows == 1 && roi.cols == 9, "bad reshape");
                roi.convertTo(roi, CV_32F);
                const Mat r = roi*kernel;
                SUSTech::sustechDynamicAssert(r.rows == 1 && r.cols == 1, "bad reshape");
                dst.at<Vec3b>(i, j) = r.at<Vec3f>(0,0);
            }
        }
        imshow("conv", dst);
        waitKey(0);
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}