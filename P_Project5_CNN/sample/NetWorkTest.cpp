//
// Created by 叶璨铭 on 2021/12/12.
//
#define FaceClassifier_WEIGHTS_PATH "weights/face_binary_cls.cpp"
#include FaceClassifier_WEIGHTS_PATH
#include "NeuronNetwork/NeuronNetwork.h"
int main(){
    using namespace SUSTech;
    Convolution conv1{conv0_weight, conv0_bias};
//    conv1.forward({});
    return 0;
}