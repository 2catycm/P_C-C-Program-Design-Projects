#pragma once
#include <iostream>
#include <matrix/Matrix.h>
namespace SUSTech {
    template <typename Tp, size_t Channels, bool IsContinuous>
    const std::shared_ptr<Tp>& SUSTech::ChannelMajorMatrix<Tp, Channels, IsContinuous>::getMOriginalData() const {
        return mOriginalData;
    }
    // 1.创造型构造器
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::ChannelMajorMatrix(size_t rows, size_t cols) requires(IsContinuous)
        //        :mOriginalChannelStep(rows), mOriginalRowStep(cols){
        : mSlice{ { 0, rows, 1 }, { 0, cols, 1 } }, mOriginalChannelStep(rows * cols), mOriginalRowStep(cols) {
        if(!empty())
            this->mOriginalData = std::shared_ptr<Tp>(new Tp[Channels * rows * cols]);
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::ChannelMajorMatrix(
        size_t mRows, size_t mCols, const std::array<Tp, Channels>& scalar) requires(IsContinuous)
        : ChannelMajorMatrix(mRows, mCols) {
        (*this) = scalar;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    void ChannelMajorMatrix<Tp, Channels, IsContinuous>::create(size_t rows, size_t cols,
                                                                const std::array<Tp, Channels>& scalar) requires(IsContinuous) {
        this->mOriginalChannelStep = rows;
        this->mOriginalRowStep = cols;
        this->mSlice = { { 0, rows, 1 }, { 0, cols, 1 } };
        this->mOriginalData = std::shared_ptr<Tp>(new Tp[Channels * rows * cols]);
        (*this) = scalar;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::ChannelMajorMatrix(std::shared_ptr<Tp> eData, size_t rows,
                                                                       size_t cols) requires(IsContinuous)
        : mOriginalData{ eData }, mOriginalChannelStep(rows * cols),
          mOriginalRowStep(cols), mSlice{ { 0, rows, 1 }, { 0, cols, 1 } } {
        //        if(notOwner)
        //            mOriginalData = { eData, noFree};
        //        else
        //            mOriginalData = { eData };
    }
    // 2.3roi构造器

    template <typename Tp, size_t Channels, bool IsContinuous>
    template <bool IsContinuous_>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::ChannelMajorMatrix(
        const ChannelMajorMatrix<Tp, Channels, IsContinuous_>& other, std::slice rowSlice,
        std::slice colSlice) requires(!IsContinuous)
        : mOriginalData{ other.mOriginalData }, mOriginalChannelStep(other.mOriginalChannelStep),
          mOriginalRowStep(other.mOriginalRowStep), mSlice{ { other.mSlice.rowSlice.start() + rowSlice.start(), rowSlice.size(),
                                                              other.mSlice.rowSlice.stride() * rowSlice.stride() },
                                                            { other.mSlice.colSlice.start() + colSlice.start(), colSlice.size(),
                                                              other.mSlice.colSlice.stride() * colSlice.stride() } } {
        sustechDynamicAssert<BadArgumentSize>(rowSlice.start() + rowSlice.size() <= other.mRows() &&
                                                  colSlice.start() + colSlice.size() <= other.mCols(),
                                              "Roi size must be smaller than original size.");  //这个是小于等于，不是小于。
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, false>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator()(std::slice rowSlice, std::slice colSlice) const {
        ChannelMajorMatrix<Tp, Channels, false> result{ *this, rowSlice, colSlice };
        return result;
    }
    //特殊处理的channel
    // noncontinuous version
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, 1, IsContinuous> ChannelMajorMatrix<Tp, Channels, IsContinuous>::channel(size_t channel) const {
        sustechDynamicAssert<BadArgumentSize>(channel < Channels, "Index out of bound.");
        ChannelMajorMatrix<Tp, 1, IsContinuous> result{};  //不能调用复制构造函数，因为类型不一致。(通道数不同)
        result.mOriginalData = std::shared_ptr<Tp>(mOriginalData.get() + channel * mOriginalChannelStep, noFree);
        result.mOriginalChannelStep = mOriginalChannelStep;
        result.mOriginalRowStep = mOriginalRowStep;
        result.mSlice = mSlice;
        return result;
    }
    // continuous version
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, 1, IsContinuous> ChannelMajorMatrix<Tp, Channels, IsContinuous>::channel(size_t channel) const
        requires(IsContinuous) {
        sustechDynamicAssert<BadArgumentSize>(channel < Channels, "Index out of bound.");
        return { std::shared_ptr<Tp>(mOriginalData.get() + channel * mOriginalChannelStep, noFree), mRows(), mCols() };
    }
    //取值逻辑(核心逻辑)， 从内存存储方式、ROI实现方式推导得来。
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline Tp& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator()(size_t channel, size_t row, size_t col) {
        sustechDynamicAssert(!empty());
        sustechDynamicAssert<BadArgumentSize>(channel < Channels && row < mRows() && col < mCols(), "Index out of bound. ");
        return this->mOriginalData.get()[channel * mOriginalChannelStep +
                                         (row + mSlice.rowSlice.start()) * mOriginalRowStep * mSlice.rowSlice.stride() +
                                         (col + mSlice.colSlice.start()) * mSlice.colSlice.stride()];
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline const Tp& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator()(size_t channel, size_t row, size_t col) const {
        sustechDynamicAssert(!empty());
        sustechDynamicAssert<BadArgumentSize>(channel < Channels && row < mRows() && col < mCols(), "Index out of bound. ");
        return this->mOriginalData.get()[channel * mOriginalChannelStep +
                                         (row + mSlice.rowSlice.start()) * mOriginalRowStep * mSlice.rowSlice.stride() +
                                         (col + mSlice.colSlice.start()) * mSlice.colSlice.stride()];
    }
    // continuous partial specialization. The stride must be 1, and the start of slice must be 0.
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline Tp& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator()(size_t channel, size_t row,
                                                                          size_t col) requires(IsContinuous) {
        sustechDynamicAssert(!empty());
        sustechDynamicAssert<BadArgumentSize>(channel < Channels && row < mRows() && col < mCols(), "Index out of bound. ");
        return this->mOriginalData.get()[channel * mOriginalChannelStep + row * mOriginalRowStep + col];
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline const Tp& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator()(size_t channel, size_t row, size_t col) const
        requires(IsContinuous) {
        sustechDynamicAssert(!empty());
        sustechDynamicAssert<BadArgumentSize>(channel < Channels && row < mRows() && col < mCols(), "Index out of bound. ");
        return this->mOriginalData.get()[channel * mOriginalChannelStep + row * mOriginalRowStep + col];
    }

    //形状逻辑
    template <typename Tp, size_t Channels, bool IsContinuous>
    void ChannelMajorMatrix<Tp, Channels, IsContinuous>::reshape(size_t rows, size_t cols) {
        sustechDynamicAssert<BadArgumentSize>(rows * cols == this->mRows() * this->mCols(), "Bad reshape size.");
        this->mSlice.setRows(rows);
        this->mSlice.setCols(cols);
        //下面这一步很重要。
        this->mOriginalRowStep =
            cols;  //并没有改变原始数据的形状，这里无关roi。意思是对于坐标的理解发生变化之后，对内存的理解需要发生变化。
                   //        return *this;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    void ChannelMajorMatrix<Tp, Channels, IsContinuous>::reshape(size_t cols) {
        sustechDynamicAssert<BadArgumentSize>(cols % this->mCols() == 0 || this->mCols() % cols == 0, "Bad reshape size.");
        this->mSlice.setRows(mRows() * mCols() / cols);
        this->mSlice.setCols(cols);
        this->mOriginalRowStep = cols;
        //        return *this;
    }

    template <typename Tp, size_t Channels, bool IsContinuous>
    inline ChannelMajorMatrix<Tp, Channels, IsContinuous> ChannelMajorMatrix<Tp, Channels, IsContinuous>::reshaped(size_t rows,
                                                                                                                   size_t cols) {
        auto result{ *this };  //复制，不连续的也可以复制。
        result.reshape(rows, cols);
        return result;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline ChannelMajorMatrix<Tp, Channels, IsContinuous> ChannelMajorMatrix<Tp, Channels, IsContinuous>::reshaped(size_t cols) {
        auto result{ *this };
        result.reshape(cols);
        return result;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline std::array<size_t, 3> ChannelMajorMatrix<Tp, Channels, IsContinuous>::shape() const {
        return { Channels, mRows(), mCols() };
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline size_t ChannelMajorMatrix<Tp, Channels, IsContinuous>::mChannels() const {
        return Channels;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline size_t ChannelMajorMatrix<Tp, Channels, IsContinuous>::mRows() const {
        return mSlice.getRows();
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline size_t ChannelMajorMatrix<Tp, Channels, IsContinuous>::mCols() const {
        return mSlice.getCols();
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline bool ChannelMajorMatrix<Tp, Channels, IsContinuous>::empty() const {
        return Channels == 0 || mSlice.getCols() == 0 || mSlice.getRows() == 0;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    inline size_t ChannelMajorMatrix<Tp, Channels, IsContinuous>::mElements() const {
        return mRows() * mCols() * Channels;
    }

    //展开与堆叠的逻辑
    template <typename Tp, size_t Channels, bool IsContinuous>
    std::array<ChannelMajorMatrix<Tp, 1, true>, Channels> ChannelMajorMatrix<Tp, Channels, IsContinuous>::copySplit() {
        std::array<ChannelMajorMatrix<Tp, 1, true>, Channels> result{};
        for(int k = 0; k < Channels; ++k) {
            //            result[k]  = {this->mRows(), this->mCols()};
            result[k].create(this->mRows(), this->mCols());  // avoid copy.
            for(size_t i = 0; i < this->mRows(); i++) {
                for(size_t j = 0; j < this->mCols(); j++) {
                    result[k](0, i, j) = this->operator()(k, i, j);
                }
            }
        }
        return result;
    }

    template <typename Tp, size_t Channels, bool IsContinuous>
    __SUSTC_Deprecated std::array<ChannelMajorMatrix<Tp, 1, IsContinuous>, Channels>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::sharedSplit() {
        std::array<ChannelMajorMatrix<Tp, 1>, Channels> result{};
        result[0] = { std::shared_ptr<Tp>(this->mOriginalData), this->mRows(), this->mCols() };
        for(int k = 1; k < Channels; ++k) {
            //            result[0] = { std::shared_ptr<Tp>(this->mOriginalData + k * mOriginalChannelStep, noFree),
            //            this->mRows(),
            //                          this->mCols() };
            result[0] = this->channel(k);
        }  // FIXME, other channel cannot exist if the first channel is freed.
        return result;
    }
    SUSTC_Deprecated__

        template <typename Tp, size_t Channels, bool IsContinuous>
        inline ChannelMajorMatrix<Tp, 1> ChannelMajorMatrix<Tp, Channels, IsContinuous>::sharedChannelFlatten() const
        requires(IsContinuous) {
        return { this->mOriginalData, this->mRows() * Channels, this->mCols() };
    }
    template <typename Tp, size_t Channels, bool IsContinuous_>
    ChannelMajorMatrix<Tp, Channels, true>
    stackMatrices(const std::array<ChannelMajorMatrix<Tp, 1, IsContinuous_>, Channels>& matrices) {
        sustechDynamicAssert<BadCast>(Channels != 0, "Zero channel cannot be stacked. ");
        size_t M = matrices[0].mRows(), N = matrices[0].mCols();
        ChannelMajorMatrix<Tp, Channels> result{ M, N };
        for(int k = 0; k < Channels; ++k) {
            const auto& mat = matrices[k];
            sustechDynamicAssert<BadCast>(mat.mRows() == M && mat.mCols() == N,
                                          "The matrices to be stacked have different shapes. ");
            for(size_t i = 0; i < M; i++) {
                for(size_t j = 0; j < N; j++) {
                    result(k, i, j) = mat(0, i, j);
                }
            }
        }
        return result;
    }
#if(SUPPORT_OPENCV)
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::ChannelMajorMatrix(cv::Mat&& cvMatrix) SUSTech_NOEXCEPT
        requires(Channels == 1, IsContinuous)
        : ChannelMajorMatrix{ std::shared_ptr<Tp>(reinterpret_cast<Tp*>(cvMatrix.data), cv::fastFree),
                              static_cast<size_t>(cvMatrix.rows), static_cast<size_t>(cvMatrix.cols) } {
        constexpr size_t result_cn = 1;
        //        static_assert(Channels==result_cn, "bad cast");
        sustechDynamicAssert<BadCast>(cvMatrix.channels() == result_cn);
        //抢夺指针
        //把对方置零
        cvMatrix.flags = cv::Mat::MAGIC_VAL;
        cvMatrix.dims = cvMatrix.rows = cvMatrix.cols = 0;
        cvMatrix.data = NULL;
        cvMatrix.datastart = NULL;
        cvMatrix.dataend = NULL;
        cvMatrix.datalimit = NULL;
        cvMatrix.allocator = NULL;
        cvMatrix.u = NULL;
    }
#endif
    //核心的lambda元素执行逻辑
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename F>
    __SUSTC_Deprecated ChannelMajorMatrix<Tp, Channels, IsContinuous>&
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::forEachScalarApply(std::array<F, Channels> lambdas) requires(
        IsContinuous /*, std::is_convertible_v<F, std::function<void(Tp&)>>*/) {
        static_assert(std::is_convertible_v<F, std::function<void(Tp&)>>, "Unsupported Lambda function.");
        const size_t elements = mRows() * mCols();
        for(size_t k = 0; k < Channels; k++) {
            Tp* ptr = this->getMOriginalData().get() + mOriginalChannelStep * k;
            const auto& lambda = lambdas[k];
#pragma omp simd
            for(int64_t l = 0; l < elements; ++l) {
                lambda(ptr[l]);
            }
        }
        return *this;
    }
    SUSTC_Deprecated__ template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename F>
    __SUSTC_Deprecated ChannelMajorMatrix<Tp, Channels, IsContinuous>&
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::forEachScalarApply(std::array<F, Channels> lambdas) /*requires(
        std::is_convertible_v<F, std::function<void(Tp&)>>)*/
    {
        static_assert(std::is_convertible_v<F, std::function<void(Tp&)>>, "Unsupported Lambda function.");
        for(size_t k = 0; k < Channels; k++) {
            const auto& lambda = lambdas[k];
            for(size_t i = 0; i < mRows(); ++i) {
#pragma omp simd
                for(int64_t j = 0; j < mCols(); ++j) {
                    lambda(this->operator()(k, i, j));
                }
            }
        }
        return *this;
    }
    SUSTC_Deprecated__
        //    赋值逻辑
        template <typename Tp, size_t Channels, bool IsContinuous>
        ChannelMajorMatrix<Tp, Channels, IsContinuous>&
        ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator=(const std::array<Tp, Channels>& scalar) {
        //                auto lambda = [](Tp& a, const std::array<Tp, Channels>& scalar, size_t num){
        //
        //                };
        //
        //                std::array<decltype([&](Tp& a){}), Channels> lambdas;
        //                int index = 0;
        //                std::fill(lambdas.begin(), lambdas.end(), [&](Tp& a){a = scalar[index++];});
        //                return forEachScalarApply(lambdas); //bad idea.
        for(size_t k = 0; k < Channels; k++) {
            const auto& value = scalar[k];
            for(size_t i = 0; i < this->mRows(); i++) {
                //#pragma omp simd //不连续也就无法要求使用simd
                for(size_t j = 0; j < this->mCols(); j++) {
                    this->operator()(k, i, j) = value;
                }
            }
        }
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>&
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator=(const std::array<Tp, Channels>& scalar) requires(IsContinuous) {
        const size_t elements = mRows() * mCols();
        for(size_t k = 0; k < Channels; k++) {
            const auto& value = scalar[k];
            Tp* ptr = this->getMOriginalData().get() + mOriginalChannelStep * k;
#pragma omp simd
            for(int64_t l = 0; l < elements; ++l) {
                ptr[l] = value;
            }
        }
        return *this;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename F>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::forEachValueApply(
        F lambda) /*requires(std::is_convertible_v<F, std::function<void(Tp&)>>)*/ {
        static_assert(std::is_convertible_v<F, std::function<void(Tp&)>>, "Unsupported Lambda function.");
        for(size_t k = 0; k < Channels; k++) {
            for(size_t i = 0; i < mRows(); ++i) {
                //#pragma omp simd
                for(int64_t j = 0; j < mCols(); ++j) {
                    lambda(this->operator()(k, i, j));  //不连续就得用operator()去计算roi后的位置。
                }
            }
        }
        return *this;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename F>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::forEachValueApply(
        F lambda) requires(IsContinuous /*&&std::is_convertible_v<F, std::function<void(Tp&)>>*/) {
        static_assert(std::is_convertible_v<F, std::function<void(Tp&)>>, "Unsupported Lambda function.");
        const size_t elements = mElements();
        Tp* ptr = this->getMOriginalData().get();
#pragma omp simd
        for(int64_t l = 0; l < elements; ++l) {
            lambda(ptr[l]);
        }
        return *this;
    }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"

    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator=(const Tp& value) {
        return this->forEachValueApply([&](Tp& a) { a = value; });
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator+=(const Tp& value) {
        return forEachValueApply([&](Tp& a) { a += value; });
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator-=(const Tp& value) {
        return forEachValueApply([&](Tp& a) { a -= value; });
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator*=(const Tp& value) {
        return forEachValueApply([&](Tp& a) { a *= value; });
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>& ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator/=(const Tp& value) {
        return forEachValueApply([&](Tp& a) { a /= value; });
    }
#pragma clang diagnostic pop

    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename Tp2, bool IsContinuous2>
    auto
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator+(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
        -> ChannelMajorMatrix<decltype(Tp() + Tp2()), Channels, true> {
        sustechDynamicAssert<BadArgumentSize>(this->mRows() == other.mRows() && this->mCols() == other.mCols(),
                                              "ChannelMajorMatrix size not match");
        ChannelMajorMatrix<Tp, Channels> result(this->mRows(), this->mCols());
        for(size_t k = 0; k < Channels; k++) {
            for(size_t i = 0; i < this->mRows(); i++) {
                for(size_t j = 0; j < this->mCols(); j++) {
                    result(k, i, j) = this->operator()(k, i, j) + other.operator()(k, i, j);
                }
            }
        }
        return result;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename Tp2, bool IsContinuous2>
    auto
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::operator*(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
        -> ChannelMajorMatrix<decltype(Tp() * Tp2()), Channels, true> {
        sustechDynamicAssert<BadArgumentSize>(this->mRows() == other.mRows() && this->mCols() == other.mCols(),
                                              "ChannelMajorMatrix size not match");
        ChannelMajorMatrix<Tp, Channels> result(this->mRows(), this->mCols());
        for(size_t k = 0; k < Channels; k++) {
#pragma omp parallel for
            for(size_t i = 0; i < this->mRows; i++) {
                for(size_t j = 0; j < this->mCols; j++) {
                    result(k, i, j) = this->operator()(k, i, j) * other.operator()(k, i, j);
                }
            }
        }
        return result;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename Tp2, bool IsContinuous2>
    auto
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::matMul(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
        -> ChannelMajorMatrix<decltype(Tp() * Tp2()), Channels, true> {
        sustechDynamicAssert<BadArgumentSize>(this->mCols() == other.mRows(), "ChannelMajorMatrix size not match");
        ChannelMajorMatrix<Tp, Channels> result(this->mRows(), other.mCols(), { 0 });  //所有通道自动都是0. 数组可以推导

        for(size_t cn = 0; cn < Channels; cn++) {
#pragma omp parallel for
            for(int64_t i = 0; i < this->mRows(); ++i) {
                for(size_t k = 0; k < this->mCols(); ++k) {
                    const Tp& temp = this->operator()(cn, i, k);
#pragma omp simd
                    for(int64_t j = 0; j < other.mCols(); ++j) {
                        result(cn, i, j) += temp * other(cn, k, j);
                    }
                }
            }
        }
        return result;
    }

#ifndef DeclResult
#define DeclResult decltype(Tp() * Tp2() + Tp() * Tp2())
#endif
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename Tp2, bool IsContinuous2>
    auto
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::inner_product(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other,
                                                                  DeclResult initialValue) const -> DeclResult {
        sustechDynamicAssert<BadArgumentSize>(mRows() * mCols() == other.mRows() * other.mCols());
        for(size_t k = 0; k < Channels; k++) {
#pragma omp parallel for
            for(size_t i = 0; i < this->mRows; i++) {
                for(size_t j = 0; j < this->mCols; j++) {
                    initialValue += this->operator()(k, i, j) * other.operator()(k, i, j);
                }
            }
        }
        return initialValue;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    template <typename Tp2, bool IsContinuous2>
    auto
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::inner_product(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other,
                                                                  DeclResult initialValue) const -> DeclResult
        requires(IsContinuous) {
        const size_t elements = mElements();
        return std20::inner_product(mOriginalData.get(), mOriginalData.get() + elements, other.mOriginalData.get(), initialValue);
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    ChannelMajorMatrix<Tp, Channels, true>
    ChannelMajorMatrix<Tp, Channels, IsContinuous>::matRead(const std::string& file_name) requires(std::is_same_v<Tp, float>,
                                                                                                   Channels == 1, IsContinuous) {
        std::unique_ptr<std::FILE, void (*)(std::FILE*)> file{ std::fopen(file_name.c_str(), "r"),
                                                               [](std::FILE* fp) { std::fclose(fp); } };
        sustechDynamicAssert<std::ios_base::failure>(file != nullptr, "Fatal Error: File not found.\nmatrix reading terminated.");
        const size_t file_length = utils::file_len(file.get());
        const size_t max_matrix_length = (file_length + 1) / 2;  //根据文件大小，解不等式得到
        auto data = (float*)calloc(max_matrix_length, sizeof(float));
        int k = 0;
        size_t rowCount = 0, columnCount = 0;
        size_t columnCountMax = 0;  //千万不能设置为INT32_MIN！！！
        size_t buffer_max_size = max_matrix_length * 32;
        //一行的信息，极端情况下一行有2048个数，每个数很长很长，假设字面量最长为32
        char* buffer = (char*)calloc(buffer_max_size, sizeof(char));
        for(rowCount = 0; fgets(buffer, static_cast<int32_t>(buffer_max_size), file.get()) != nullptr; ++rowCount) {
            columnCount = 0;
            char* token = strtok(buffer, " ");  //不能正则的。而且还是个指针，只能一步步走。
            while(token != NULL) {
                columnCount++;
                sscanf(token, "%f", &data[k++]);
                token = strtok(NULL, " ");
            }
            if(columnCount > columnCountMax)
                columnCountMax = columnCount;
        }
        data = static_cast<float*>(realloc(data, rowCount * columnCountMax * sizeof(float)));
        return { std::shared_ptr<float>{ data }, rowCount, columnCount };
    }
    template <typename Tp_, size_t Channels_, bool IsContinuous_>
    std::istream& operator>>(std::istream& in, ChannelMajorMatrix<Tp_, Channels_, IsContinuous_>& matrix) {
        matrix.template forEachValueApply([&](Tp_& e) { in >> e; });
        return in;
    }
    template <typename Tp, size_t Channels, bool IsContinuous>
    std::string ChannelMajorMatrix<Tp, Channels, IsContinuous>::toPrettyString() const {
        std::stringstream ss;
        ss << "ChannelMajorMatrix: {" << std::endl;
        for(size_t k = 0; k < Channels; k++) {
            ss << "Channel " << k << "[" << std::endl;
            for(size_t i = 0; i < this->mRows(); i++) {
                for(size_t j = 0; j < this->mCols(); j++) {
                    ss << this->operator()(k, i, j) << " ";
                }
                ss << std::endl;
            }
            ss << "]" << std::endl;
        }
        ss << "}";
        return ss.str();
    }
    template <typename Tp_, size_t Channels_, bool IsContinuous_>
    std::ostream& operator<<(std::ostream& os, const ChannelMajorMatrix<Tp_, Channels_, IsContinuous_>& matrix) {
        for(size_t k = 0; k < Channels_; k++) {
            for(size_t i = 0; i < matrix.mRows(); i++) {
                for(size_t j = 0; j < matrix.mCols(); j++) {
                    os << matrix(k, i, j) << " ";
                }
                os << std::endl;
            }
        }
        return os;
    }

}  // namespace SUSTech
