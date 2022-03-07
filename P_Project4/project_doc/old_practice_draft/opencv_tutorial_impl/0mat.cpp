// To tackle this issue OpenCV uses a reference counting system. The idea is that each Mat object has its own header, however a
// matrix may be shared between two Mat objects by having their matrix pointers point to the same address. Moreover, the copy
// operators will only copy the headers and the pointer to the large matrix, not the data itself.
#include <opencv2/core.hpp>
int main(int argc, char const *argv[])
{
    cv::Mat a;
    return 0;
}