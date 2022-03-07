//
// Created by 叶璨铭 on 2021/12/11.
//
#pragma once
#include "FaceClassifier.h"
#include <fstream>
namespace SUSTech {
    std::array<float, 2> FaceClassifier::classify(const cv::Mat& bgr_image) const {
        const auto tensor = tensorFromImage(bgr_image);
        const auto conv0_result = conv0.forward<128, 128>(stackMatrices(tensor));
        const auto max_pool0_result = max_pool0.forward<64, 64>(conv0_result);
        const auto conv1_result = conv1.forward<32, 32>(max_pool0_result);
        const auto max_pool1_result = max_pool1.forward<30, 30>(conv1_result);
        const auto conv2_result = conv2.forward<15, 15>(max_pool1_result);
        const auto fc0_result = fc0.forward<32>(conv2_result);

        //        std::ofstream out("../resources/middleResults/secondTime/conv2_result.txt");
        //        sustechDynamicAssert<std::ios::failure>(out.is_open(), "not opened");
        //        out<<conv2_result.sharedChannelFlatten().reshaped(1)<<std::endl;

        return { fc0_result(0, 0, 0), fc0_result(0, 1, 0) };
    }
    std::array<ChannelMajorMatrix<float>, 3> FaceClassifier::tensorFromImage(const cv::Mat& raw_image) {
        sustechDynamicAssert(raw_image.type() == CV_8UC3 && !raw_image.empty());
        // 1. 把图像放缩到128x128
        cv::Mat resized_image;
        constexpr int cnn_image_required_height = 128, cnn_image_required_width = 128;
        cv::resize(raw_image, resized_image, cv::Size(cnn_image_required_width, cnn_image_required_height), (0, 0), (0, 0),
                   cv::INTER_LINEAR);
        // 2. 把图像数据类型转换为float
        cv::Mat float_image;
        resized_image.convertTo(float_image, CV_32FC3, 1.0 / 255, 0);
        // 3. 进行通道分离，形成合适的输入张量。
        std::vector<cv::Mat> tensor;
        cv::split(float_image, tensor);
        // 4. 化三个cv::Mat为一个我的矩阵类型。
        //    auto a = fromCVMat<float,1ull>(tensor[0]);
        //    std::cout<<a<<std::endl;
        sustechDynamicAssert(std::all_of(tensor.begin(), tensor.end(), [](const cv::Mat& mat) { return mat.isContinuous(); }),
                             "not continuous cannot be stolen. ");
        ChannelMajorMatrix<float> b{ std::move(tensor[0]) };
        ChannelMajorMatrix<float> g{ std::move(tensor[1]) };
        ChannelMajorMatrix<float> r{ std::move(tensor[2]) };
        //        return { std::move(b), std::move(g), std::move(r) };
        return { b, g, r };
    }
    cv::Mat FaceClassifier::visualize_classify(const cv::Mat& bgr_image) const {
        cv::Mat visual_result;
        bgr_image.copyTo(visual_result);
        auto result = this->classify(bgr_image);
        std::stringstream ss;
        if(result[0] > 0.5) {
            ss << "Face Not Found."
               << "(" << result[0] << ")";
        } else {
            ss << "Face Detected."
               << "(" << result[1] << ")";
        }
        auto text = ss.str();
        constexpr int font_face = cv::FONT_HERSHEY_COMPLEX;
        const double font_scale = 0.25 * std::sqrt(visual_result.rows * visual_result.cols) / 128;
        constexpr int thickness = 0;
        cv::Point origin{ 20 * visual_result.rows / 128, 5 * visual_result.cols / 128 };
        cv::putText(visual_result, text, origin, font_face, font_scale, cv::Scalar(0, 0, 255), thickness, 8, 0);
        return visual_result;
    }
}  // namespace SUSTech
