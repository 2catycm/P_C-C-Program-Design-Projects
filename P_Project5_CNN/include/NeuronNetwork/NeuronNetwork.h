//
// Created by 叶璨铭 on 2021/12/12.
//
#pragma once
#include "Matrix/Matrix.h"
#include <array>
#include <numeric>
namespace SUSTech {
    namespace ActivationFunction {
        // Rectified Linear Unit
        template <typename dType>
        requires(std::is_fundamental_v<dType>) constexpr void ReLU(dType& f);
        /**
         * 可能用于全连接层的激活函数Softmax.
         * @tparam dType  神经网络的数据类型， 类似于numpy、pytorch的dType。
         * 若dType为基础类型，支持自动构造，否则请在构造时提供 dType的“0”元素
         */
        template <typename dType>
        struct SoftMaxAndSumFunction{
            SoftMaxAndSumFunction() requires(std::is_fundamental_v<dType>);
            void clear() requires(std::is_fundamental_v<dType>);
            explicit SoftMaxAndSumFunction(const dType& initialValue);
            void accept(dType& newElement);
            void operator()(dType& newElement);
            [[nodiscard]] const dType& getSum() const;
        private:
            dType sum;
        };
    }  // namespace ActivationFunction
    namespace ActFun = ActivationFunction;
    namespace AggregateFunction {
        using std20::accumulate;     //求和是一个聚合函数
        using std20::inner_product;  //这个可以很好地优化。
        using std20::max_element;    //求最大值也是聚合函数
                                     //        using std::min_element;
        template <class InputIt, class T>
        constexpr
            T average(InputIt first, InputIt last, T init);//用于AveragePooling
        /**
         * 用于MaxPooling的“函数对象”。
         * @tparam dType 神经网络的数据类型.
         * 若dType为基础类型，支持自动构造，否则请在构造时提供dType的“最小值”
         */
        template <typename dType>
        struct MaxFunction {
            MaxFunction() requires(std::is_fundamental_v<dType>);
            void clear() requires(std::is_fundamental_v<dType>);
            explicit MaxFunction(const dType& initialValue);
            void accept(const dType& newElement);
            [[nodiscard]] const dType& get() const;
        private:
            const dType* max_element;
        };
    }       // namespace AggregateFunction
    namespace AggFun = AggregateFunction;

    namespace Layer {

        /**
         * 神经网络卷积层。
         * 卷积算法1：1.dispatch the channel logic to be trivial, and then use single channel single kernel method to dispatch the
         *padding and stride. 2. 及时求点积，而非搬运出来统一求。
         * 以下参数由在编译时确定的模型确定。
         * @tparam in_channels 输入的通道数。
         * @tparam out_channels 输出的通道数 = 卷积核的数量
         * @tparam kernel_size 卷积核对于一个输入通道的大小，为正方形的边长。
         * @tparam stride 卷积窗口移动的步长
         * @tparam padding 卷积对输入的逻辑填充长度
         * @tparam dType 神经网络数据类型。
         * @tparam activator 这一层的激活函数。Typically，这次project都是ReLU。
         */
        template <size_t in_channels = 3, size_t out_channels = 16, size_t kernel_size = 3, size_t stride = 2, size_t padding = 1,
                  typename dType = float, void (*activator)(dType&) = ActFun::ReLU<dType>>
        requires(std::is_fundamental_v<dType>) class Convolution {
            //note: mKernels不能是const，不然难以构造。
            std::array<ChannelMajorMatrix<dType, in_channels>, out_channels> mKernels{}; //需要std::array, 因为不支持四维数组。
            ChannelMajorMatrix2D<dType> mBias;
            static constexpr size_t kernel_step = in_channels * kernel_size * kernel_size;
        public:
            /**
             * 使用参数构造卷积层。
             * @param kernels
             * @param bias
             * note：1.数据以共享方式读入，析构责任在于外部。Typically，本次project中，由FaceClassifier析构时释放。
             * TODO：2.如果只是forward的话，不会修改卷积层的参数。但是训练的时候，是不是要不断更新权值呢？
             * note: 2.因此，目前传入的不是const数组，神经网络不保证不修改外部数据，尽管这次project没有。
             */
            Convolution(dType (&kernels)[kernel_step*out_channels], dType (&bias)[out_channels]);
            ~Convolution() = default;  //没有指针变量，可以放心由RAII进行析构。Concisely speaking, ChannelMajorMatrix的析构是久经考验的、安全的。
        public:
            /**
             * 下列参数在forward时确定，是forward函数的模板参数。
             * 为什么不是模板类的参数呢？因为是模板函数的参数的话，对于不同的输入大小，可以共享一个卷积核、偏倚。
             * @tparam input_rows 输入矩阵的行数
             * @tparam input_cols 输入矩阵的列数
             * @param inputX 输入的矩阵
             * @return 本卷积层的计算结果。
             * note: forward 不改变卷积层成员变量，因此标记为const。
             * note: input_rows和input_cols应该在调用时显式指定。
             * 一方面是因为这个forward推断不出来inputX的行列数，这是动态的。
             * 另一方面，要求设计神经网络模型时，仔细检查每一步矩阵形状的变化。如此可以避免发生错误
             * （比如这次project第二步卷积，可以检查出来(在maxPool的forward处检查)是32x32->30x30，而不是keep）。
             */
            template <size_t input_rows = 128, size_t input_cols = 128>
            auto forward(const ChannelMajorMatrix<dType, in_channels>& inputX)const -> ChannelMajorMatrix<dType, out_channels>;
        private:
            //        auto single_conv_single(const ChannelMajorMatrix2D<float>& single_input, const ChannelMajorMatrix2D<float>&
            //        single_kernel)
            //            -> ; //权衡：可能会导致复制，不划算
            //        auto im2col(const ChannelMajorMatrix2D<float>& inputX) -> ChannelMajorMatrix2D<float>;
        };
        //TODO：卷积算法2。 使用im2col及其变种方法完成卷积。比较两种卷积方法的性能。
        class Convolution2 {
            // only one im2col to dispatch everything
        };

        /**
         * 神经网络池化层。
         * 以下参数由在编译时确定的模型确定。
         * @tparam in_channels 输入的通道数。
         * @tparam kernel_size 池化正方形窗口的边长。
         * @tparam stride 池化窗口移动步长
         * @tparam dType 神经网络数据类型
         * @tparam Aggregate 聚合函数：将窗口的所有值聚合为一个值放到结果中。比如AveragePool的聚合函数是求平均值，MaxPool是求最大值。
         * note: Aggregate 这个参数没有设计为针对迭代器的、std::max_element的float*(*aggregate)(float*, float*) 。 这是为了避免拷贝。
         */
        template <size_t in_channels, size_t kernel_size, size_t stride, typename dType = float,
                  typename Aggregate = AggFun::MaxFunction<dType>>
        //    requires(std::is_base_of_v<AggFun::AggregateFunction<float>, Aggregate>) //是否要继承？不。我们的类型静态可以判断，编译时可以多态。
        requires(std::is_fundamental_v<dType>) class Pooling {
            static Aggregate aggregate;  //不能写成我喜欢的constexpr，然后这个不是int， 也不能类内初始化。
            static constexpr size_t out_channels = in_channels;
        public:
            /**
             * 下列参数在forward时确定，是forward函数的模板参数。
             * @tparam input_rows 输入矩阵的行数
             * @tparam input_cols 输入矩阵的列数
             * @param inputX 输入的矩阵
             * @return 本池化层的计算结果。
             * note: @seealso class Convolution
             */
            template <size_t input_rows, size_t input_cols>
            auto forward(const ChannelMajorMatrix<dType, in_channels>& inputX) const-> ChannelMajorMatrix<dType, out_channels>;
        };
        template <size_t in_channels, size_t kernel_size, size_t stride, typename dType, typename Aggregate>
        requires(std::is_fundamental_v<dType>) Aggregate Pooling<in_channels, kernel_size, stride, dType, Aggregate>::aggregate={};
        /**
         * 神经网络全连接层。
         * 以下参数由在编译时确定的模型确定。
         * @tparam in_features 输入矩阵铺平后的元素数量
         * @tparam out_features 下一层有几个节点。
         * @tparam dType 神经网络数据类型。
         * @tparam Activator 激活函数。比如人脸识别project是softmax。
         * FIXME: softmax的调用方法与ReLU不同，接口不统一。本次project无所谓。
         */
        template<size_t in_features, size_t out_features,typename dType = float, typename Activator = ActFun::SoftMaxAndSumFunction<dType>>
        requires(std::is_fundamental_v<dType>) class FullConnection{
            static Activator activator;
            const ChannelMajorMatrix2D<dType> mWeights; //out_features * in_features
            const ChannelMajorMatrix2D<dType> mBias; //out_features * 1
        public:
            /**
             * 使用参数构造全连接层。
             * @param weights
             * @param bias
             * note：@seealso class Convolution
             */
            FullConnection(dType (&weights)[out_features*in_features], dType (&bias)[out_features*1]);
            /**
             * 下列参数在forward时确定，是forward函数的模板参数。
             * @tparam in_channels 输入矩阵通道数。
             * @param inputX 输入矩阵
             * @return 本全连接层的计算结果。
             * note：输入的矩阵不需要在forward前铺平，铺平责任在本函数。
             * note: @seealso class Convolution
             */
            template <size_t in_channels>
            auto forward(const ChannelMajorMatrix<dType, in_channels>& inputX)const -> ChannelMajorMatrix2D<dType>;
        };
        template<size_t in_features, size_t out_features,typename dType, typename Activator>
        requires(std::is_fundamental_v<dType>)
            Activator FullConnection<in_features, out_features, dType, Activator>::activator={};
    }  // namespace Layer
}  // namespace SUSTech
#include "NeuronNetwork.ipp"