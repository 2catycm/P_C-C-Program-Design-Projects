//
// Created by 叶璨铭 on 2021/12/11.
//
#include "FaceClassifier/FaceClassifier.h"
#include <opencv2/opencv.hpp>
int main(int argc, char* argv[]) {
    auto& s = SUSTech::FaceClassifier::getInstance();
    cv::VideoCapture capture(0);
    cv::namedWindow("Face Classifier");
    cv::Mat frame;
    for(capture>>frame; !frame.empty(); capture>>frame) {
        cv::imshow("Face Classifier", s.visualize_classify(frame));
        cv::waitKey(1000);
    }
    return 0;
}