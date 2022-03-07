//
// Created by 叶璨铭 on 2021/12/11.
//
#include "FaceClassifier/FaceClassifier.h"
#include <opencv2/opencv.hpp>
int main(int argc, char* argv[]) {
    auto& s = SUSTech::FaceClassifier::getInstance();
    //    s.test();
    const std::string default_path = "../resources/samples/face.jpg";
    const auto image = cv::imread((argc > 1) ? argv[1] : default_path);
    SUSTech::sustechDynamicAssert<std::ios::failure>(!image.empty(), "read image failed.");
    auto result = s.classify(image);
    std::cout<<"bg score: "<<result[0]<<", face score: "<<result[1]<<std::endl;

    cv::namedWindow("Face Classifier");
    cv::imshow("Face Classifier", s.visualize_classify(image));
    cv::waitKey(0);
    return 0;
}