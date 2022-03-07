//
// Created by 叶璨铭 on 2021/12/11.
//
#pragma once
#define FaceClassifier_WEIGHTS_PATH "weights/face_binary_cls.cpp"
#include "Matrix/Matrix.h"
#include "NeuronNetwork/NeuronNetwork.h"
#include "SUSTechDefinitions.h"
#include <array>
#include <opencv2/opencv.hpp>
namespace SUSTech {
    /**
     * 人脸分类器。
     * 设计为单例，因为模型数据只加载一次。
     */
    class FaceClassifier {
    public:
        //对外接口
        [[nodiscard]] std::array<float, 2> classify(const cv::Mat& bgr_image) const ;
        cv::Mat visualize_classify(const cv::Mat& bgr_image) const ;
        static FaceClassifier& getInstance() {
            static FaceClassifier instance;
            return instance;
        }
        //需要对benchmark测试时间友好，因此暴露一定的内部接口：
        static std::array<ChannelMajorMatrix<float>, 3> tensorFromImage(const cv::Mat& raw_image);
    private:
        //导入训练好的模型数据。注意这里用到 C++11 特性：成员变量类内初始化。
#include FaceClassifier_WEIGHTS_PATH
        // Convolution<in_ch, out_ch, ksz, str, pad, dType, activator>
        SUSTech::Layer::Convolution<3, 16, 3, 2, 1, float, SUSTech::ActFun::ReLU<float>> conv0{ conv0_weight, conv0_bias };
        // Pooling<in_ch, ksz, str, dType, Aggregate>
        SUSTech::Layer::Pooling<16, 2, 2, float, SUSTech::AggFun::MaxFunction<float>> max_pool0{};
        //Convolution<in_ch, out_ch, ksz, str, pad, dType, activator>
        SUSTech::Layer::Convolution<16, 32, 3, 1, 0, float, SUSTech::ActFun::ReLU> conv1{ conv1_weight, conv1_bias };
        // Pooling<in_ch, ksz, str, dType, Aggregate>
        SUSTech::Layer::Pooling<32, 2, 2, float, SUSTech::AggFun::MaxFunction<float>> max_pool1{};
        //Convolution<in_ch, out_ch, ksz, str, pad, dType, activator>
        SUSTech::Layer::Convolution<32, 32, 3, 2, 1, float, SUSTech::ActFun::ReLU> conv2{ conv2_weight, conv2_bias };
        SUSTech::Layer::FullConnection<32*8*8, 2, float, SUSTech::ActFun::SoftMaxAndSumFunction<float>> fc0{fc0_weight, fc0_bias};
    private:
        //单例隐藏构造器
        FaceClassifier() = default;   //第一次请求FaceClassifier时构造，加载参数到内存。
        ~FaceClassifier() = default;  //程序结束自动调用
        FaceClassifier(const FaceClassifier&) {
            sustechDynamicAssert(false, "Trying to copy Singleton: \"FaceClassifier.\"");
        }
        FaceClassifier& operator=(const FaceClassifier&) {
            sustechDynamicAssert(false, "Trying to copy Singleton: \"FaceClassifier.\"");
            return *this;
        }
    };
}  // namespace SUSTech
#include "FaceClassifier.ipp"