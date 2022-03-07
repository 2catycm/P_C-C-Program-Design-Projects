//
// Created by 叶璨铭 on 2021/12/12.
//
#include <memory>
#include <opencv2/opencv.hpp>
void myfree(void* ptr){
    std::cout<<"freeing"<<std::endl;
}
int main(){
    const char* default_path = "../resources/samples/face.jpg";
    auto cvMatrix = cv::imread(default_path);
//    std::shared_ptr<uint8_t> a{cvMatrix.data, myfree};
//    std::shared_ptr<uint8_t> a{cvMatrix.data, free};
    std::shared_ptr<uint8_t> a{cvMatrix.data, cv::fastFree};
//    std::shared_ptr<uint8_t> a{cvMatrix.data, cvMatrix.deallocate};
    cvMatrix.flags = cv::Mat::MAGIC_VAL;
    cvMatrix.dims = cvMatrix.rows = cvMatrix.cols = 0;
    cvMatrix.data = NULL;
    cvMatrix.datastart = NULL;
    cvMatrix.dataend = NULL;
    cvMatrix.datalimit = NULL;
    cvMatrix.allocator = NULL;
    cvMatrix.u = NULL;
//    cvMatrix.step.p = cvMatrix.step.buf;
}