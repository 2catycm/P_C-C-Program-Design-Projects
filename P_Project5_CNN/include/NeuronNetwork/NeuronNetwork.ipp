//
// Created by 叶璨铭 on 2021/12/12.
//
#pragma once
#include "NeuronNetwork.h"
#include <cmath>
namespace SUSTech {
    namespace ActivationFunction {
        template <typename dType>
        requires(std::is_fundamental_v<dType>) constexpr void ReLU(dType& f) {
            if(f < 0)
                f = 0;
        }
        template <>
        constexpr void ReLU(float& f) {
            // 1.
            //            f = std::max(f, 0.f);//184.050ms
            // 2. avoid if.
            //            f *= (f > 0.f); //174.922ms
            // 3. trivially if.
            if(f < 0)
                f = 0;  // 158.544ms
            // 4. fast return
            //            if (f>0) return;
            //            f = 0; //160.887ms
            // 5.
        }
        // SoftMaxAndSumFunction
        template <typename dType>
        inline SoftMaxAndSumFunction<dType>::SoftMaxAndSumFunction() requires(std::is_fundamental_v<dType>) : sum(0) {}
        template <typename dType>
        inline void SoftMaxAndSumFunction<dType>::clear() requires(std::is_fundamental_v<dType>) {
            sum = 0;
        }
        template <typename dType>
        inline SoftMaxAndSumFunction<dType>::SoftMaxAndSumFunction(const dType& initialValue) : sum{ initialValue } {}
        template <typename dType>
        inline void SoftMaxAndSumFunction<dType>::accept(dType& newElement) {
            newElement = exp(newElement);
            sum += newElement;
        }
        template <typename dType>
        inline void SoftMaxAndSumFunction<dType>::operator()(dType& newElement) {
            newElement = exp(newElement);
            sum += newElement;
        }
        template <typename dType>
        inline const dType& SoftMaxAndSumFunction<dType>::getSum() const {
            return sum;
        }

    }  // namespace ActivationFunction
    namespace ActivationFunction {
        template <class InputIt, class T>
        constexpr T average(InputIt first, InputIt last, T init) {
            for(; first != last; ++first) {
                init = std::move(init) + *first;
            }
            return init / std::distance(first, last);
        }
    }  // namespace ActivationFunction
    namespace AggregateFunction {
        // MaxFunction
        template <typename T>
        inline MaxFunction<T>::MaxFunction(const T& initialValue) : max_element{ static_cast<const float*>(&initialValue) } {}
        template <typename T>
        inline void MaxFunction<T>::accept(const T& newElement) {
            max_element = static_cast<const float*>(static_cast<const float*>(&std::max((*max_element), newElement)));
        }
        template <typename T>
        inline const T& MaxFunction<T>::get() const {
            return *max_element;
        }
        template <typename T>
        inline MaxFunction<T>::MaxFunction() requires(std::is_fundamental_v<T>) : MaxFunction{ std::numeric_limits<T>::min() } {}
        template <typename T>
        inline void MaxFunction<T>::clear() requires(std::is_fundamental_v<T>) {
            static constexpr T min = std::numeric_limits<T>::min();
            max_element = static_cast<const float*>(&min);
        }
    }  // namespace AggregateFunction
    namespace Layer {
        template <size_t in_channels, size_t out_channels, size_t kernel_size, size_t stride, size_t padding, typename dType,
                  void (*activator)(dType&)>
        requires(std::is_fundamental_v<dType>)
            Convolution<in_channels, out_channels, kernel_size, stride, padding, dType, activator>::Convolution(
                dType (&kernels)[kernel_step * out_channels], dType (&bias)[out_channels])
            : mBias{ std::shared_ptr<dType>(bias, noFree), out_channels, 1 } {
            for(int k = 0; k < out_channels; ++k) {
                mKernels[k] = { std::shared_ptr<dType>{ kernels + k * kernel_step, noFree }, kernel_size, kernel_size };
            }
        }
        template <size_t in_channels, size_t out_channels, size_t kernel_size, size_t stride, size_t padding, typename dType,
                  void (*activator)(dType&)>
        requires(std::is_fundamental_v<dType>) template <size_t input_rows, size_t input_cols>
        auto Convolution<in_channels, out_channels, kernel_size, stride, padding, dType, activator>::forward(
            const ChannelMajorMatrix<dType, in_channels>& inputX) const -> ChannelMajorMatrix<dType, out_channels> {
            //输出形状的计算
            //动手学深度学习一书的padding定义不一样。
            static constexpr size_t output_rows = (input_rows + padding * 2 - kernel_size + stride) / stride,
                                    output_cols = (input_cols + padding * 2 - kernel_size + stride) / stride;

            ChannelMajorMatrix<dType, out_channels> result{ output_rows, output_cols, { 0 } };
            for(size_t k = 0; k < out_channels; ++k) {
                // input是inputX
                const auto& kernel = mKernels[k];  //比如 3x3x3
                sustechDynamicAssert<BadArgumentSize>(kernel.mChannels() == in_channels && kernel.mRows() == kernel_size &&
                                                          kernel.mCols() == kernel_size,
                                                      "Bad kernel to do convolution. ");
                auto out_matrix = result.channel(k);  //比如 1x64x64
                for(size_t l = 0; l < in_channels; ++l) {
                    size_t out_matrix_index = 0;
                    //下面，我们需要把每一个channel的结果累加到out_matrix里面
                    const auto& single_kernel = kernel.channel(l);
                    const auto& single_input = inputX.channel(l);
                    constexpr int64_t rowLimit = input_rows + padding - kernel_size + 1;
                    constexpr int64_t colLimit = input_rows + padding - kernel_size + 1;
                    //#pragma omp parallel for
                    for(int64_t m = -int64_t(padding); m < rowLimit; m += stride) {
                        for(int64_t n = -int64_t(padding); n < colLimit; n += stride) {
                            //可以用sum叠加，也可以直接加到结果矩阵。
                            auto& sum = out_matrix.getMOriginalData().get()[out_matrix_index++];
                            //九宫格
#pragma omp simd
                            for(int i1 = 0; i1 < kernel_size; ++i1) {
                                for(int j1 = 0; j1 < kernel_size; ++j1) {
                                    const int64_t real_row = m + i1;
                                    const int64_t real_col = n + j1;
                                    if(caffe::is_a_ge_zero_and_a_lt_b(real_row, input_rows) &&
                                       caffe::is_a_ge_zero_and_a_lt_b(real_col, input_cols)) {
                                        sum += single_kernel(0, i1, j1) * single_input(0, real_row, real_col);
                                    }
                                }
                            }
                        }
                    }
                }  //现在所有通道都被一个卷积核卷积过了，加到了这一层的out_matrix上面
                //加上一个bias 再来个relu
                //            out_matrix += mBias[k];
                out_matrix.forEachValueApply([&](dType& e) {
                    e += mBias(0, k, 0);
                    activator(e);
                });
            }  //现在16个卷积和算出了16个矩阵。
            return result;
        }

        template <size_t in_channels, size_t kernel_size, size_t stride, typename dType, typename Aggregate>
        //    requires(std::is_base_of_v<AggFun::AggregateFunction<float>, Aggregate>)
        requires(std::is_fundamental_v<dType>) template <size_t input_rows, size_t input_cols>
        auto Pooling<in_channels, kernel_size, stride, dType, Aggregate>::forward(
            const ChannelMajorMatrix<dType, in_channels>& inputX) const -> ChannelMajorMatrix<dType, out_channels> {
            //解不等式： let x = index of last window up left, y = index of last window up right+1. k = kernel_size. r =
            // input_rows
            // y = x+k, y <= r.   x = 0, stride, 2*stride, 3*stride, 4*stride ...
            // exists p, such that x = stride* p; Notice that output_rows = p+1 .
            // stride*p+k<=r; therefore:
            static constexpr size_t output_rows = (input_rows - kernel_size + stride) / stride,
                                    output_cols = (input_cols - kernel_size + stride) / stride;
            sustechDynamicAssert<BadArgumentSize>(inputX.mRows() == input_rows && inputX.mCols() == input_cols,
                                                  "Input X does not match the type of layer.");
            ChannelMajorMatrix<dType, out_channels> result(1, output_rows * output_cols, { 0 });
            //#pragma omp parallel for
            for(size_t k = 0; k < in_channels; ++k) {
                size_t index = 0;
                const auto& single_input = inputX.channel(k);
                auto out_matrix = result.channel(k);
                //#pragma omp simd
                for(size_t i = 0; i + kernel_size <= input_rows; i += stride) {
                    for(size_t j = 0; j + kernel_size <= input_cols; j += stride) {
                        aggregate.clear();
                        auto roi = single_input({ i, kernel_size, 1 }, { j, kernel_size, 1 });
                        roi.forEachValueApply([&](dType& x) { aggregate.accept(x); });
                        out_matrix(0, 0, index++) = aggregate.get();
                    }
                }
            }
            result.reshape(output_rows, output_cols);
            return result;
        }
        // FullConnection
        template <size_t in_features, size_t out_features, typename dType, typename Activator>
        requires(std::is_fundamental_v<dType>) FullConnection<in_features, out_features, dType, Activator>::FullConnection(
            dType (&weights)[out_features * in_features], dType (&bias)[out_features * 1])
            : mWeights{ std::shared_ptr<dType>(weights, noFree), out_features, in_features }, mBias{
                  std::shared_ptr<dType>(bias, noFree), out_features, 1
              } {}

        template <size_t in_features, size_t out_features, typename dType, typename Activator>
        requires(std::is_fundamental_v<dType>) template <size_t in_channels>
        auto FullConnection<in_features, out_features, dType, Activator>::forward(
            const ChannelMajorMatrix<dType, in_channels>& inputX) const -> ChannelMajorMatrix2D<dType> {
            sustechDynamicAssert<BadArgumentSize>(in_channels * inputX.mRows() * inputX.mCols() == in_features,
                                                  "Input X does not match the type of layer.");
            activator.clear();
            auto flattenedX = inputX.sharedChannelFlatten().reshaped(1);
            return ((mWeights.matMul(flattenedX) + mBias).forEachValueApply([&](dType& d) { activator(d); })) /=
                activator.getSum();
        }

    }  // namespace Layer
}  // namespace SUSTech