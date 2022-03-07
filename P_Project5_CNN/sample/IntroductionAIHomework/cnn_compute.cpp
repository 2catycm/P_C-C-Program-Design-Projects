//
// Created by 叶璨铭 on 2022/1/12.
//
#include "NeuronNetwork/NeuronNetwork.h"
#include <SUSTechDefinitions.h>
#include <memory>
#include <iostream>
int main() {
//#define int float
#define int double
    using namespace SUSTech;
    int conv1_weights[] = { 1, -3, 0, -1, 2, -1, 0, -3, 1 };
    int conv1_bias[] = { 0 };
    Layer::Convolution<1, 1, 3, 1, 0, int, ActFun::ReLU<int>> conv1{ conv1_weights, conv1_bias };
    int conv2_weights[] = { 3, -1, -1, 3, 2, 0, 0, 2, 1, 0, 0, 1 };
    int conv2_bias[] = { 0, 0, 0 };
    Layer::Convolution<1, 3, 2, 1, 0, int, ActFun::ReLU<int>> conv2{ conv2_weights, conv2_bias };
    int fc_weights[] = { -2, 1, 1, -2, 2, -1 };
    int fc_bias[] = { 0, 0 };
    Layer::FullConnection<3 * 1 * 1, 2, int, ActFun::SoftMaxAndSumFunction<int>> fc{ fc_weights, fc_bias };
    int x[] = { 6, 5, 3, -2, -4, 7, 0, 8, 1, -2, 4, 1, 3, 3, -4, 0 };
    ChannelMajorMatrix2D<int> matX{ std::shared_ptr<int>{ x, noFree }, 4, 4 };
    std::cout<<matX.toPrettyString()<<std::endl;
    const auto conv1_re = conv1.forward<4,4>(matX);
    std::cout<<conv1_re.toPrettyString()<<std::endl;
    const auto conv2_re = conv2.forward<2,2>(conv1_re);
    std::cout<<conv2_re.toPrettyString()<<std::endl;
    const auto fc_re = fc.forward<3>(conv2_re);
    std::cout<<fc_re.toPrettyString()<<std::endl;
}