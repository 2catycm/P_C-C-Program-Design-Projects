//
// Created by 叶璨铭 on 2021/11/22.
//
namespace A{
    template<typename T>
    class Mat{};
    using FloatMat = Mat<float>;
    typedef FloatMat fMat;
}
int main(){
    A::FloatMat a;
    A::Mat<double> b;
    A::fMat c;
}