#pragma once
#include "SUSTechDefinitions.h"
#include <array>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <valarray>
#ifndef SUPPORT_OPENCV
#define SUPPORT_OPENCV 1
#endif
#if(SUPPORT_OPENCV)
#include <opencv2/opencv.hpp>
#endif
namespace SUSTech {
    //矩阵相关异常
    struct MatrixError : public SUSTechError {
        explicit MatrixError(const std::string& message);
    };
    struct BadArgumentSize : public MatrixError {
        explicit BadArgumentSize(const std::string& message);
    };
    struct BadCast : public MatrixError {
        explicit BadCast(const std::string& message);
    };
    /**
     * 矩阵切片类。
     * 为了高效地实现roi。
     */
    struct MatSlice {
        std::slice rowSlice;
        std::slice colSlice;

        size_t getRows() const;
        void setRows(size_t rows);
        size_t getCols() const;
        void setCols(size_t cols);
        static bool sliceEquals(const std::slice& lhs, const std::slice& rhs);
        static bool sliceUnequals(const std::slice& lhs, const std::slice& rhs);
        bool operator==(const MatSlice& rhs) const;
        bool operator!=(const MatSlice& rhs) const;
        bool isZero();
        static MatSlice zero();  //不是单例，会有拷贝，只是便捷方法, 简单类型移动也没用。
    };
    /**
     * 通道主序的矩阵。
     * 通道主序：与行列主序相对。
     * 通道主序的矩阵对自己拥有的内存建立的不变式（理解）是，首先，一个通道的所有元素是存储在一起的。
     * 其次，在每一个通道当中，同一行的元素是存储在一起。
     * 再次，在每一个行中，同一列的元素(只有一个)存储在一起。
     * 即 rr gg bb。
     * note: 与 np.ndarray 类似、与 cv::Mat 不同。
     * @tparam Tp 数据类型
     * @tparam Channels 通道数（重要）@seealso 成员函数mChannels, channel，sharedChannelFlatten，copySplit，sharedSplit，stackMatrices，构造函数cv::Mat。
     * @tparam IsContinuous 矩阵的连续性。（重要）@seealso 成员函数operator(), roi构造函数, forEachValueApply.
     * note: IsContinuous 作为模板参数是一个非常好的设计。首先，我们完全可以在编译时知晓一个矩阵是否连续，而不是运行时，
     * 因为设计矩阵的人有责任知晓每一个函数的功能，是否改变矩阵的连续性。
     * 其次，针对不同连续性的矩阵，不需要在运行时用if来判断选择哪一个算法，而是编译时用模板特化确定，不仅让代码编写更为优雅（比起project4）、
     * 给予了编译器优化的空间（编译时就可以因为是连续矩阵而大胆使用连续版本的方法，然后进行优化），还避免了分支预测失败。
     * 再次，对于使用矩阵的人，由于默认参数为连续，大多数情况下也是连续的矩阵，使用没有带来不便；如果使用矩阵的人用到了roi
     * 不连续版本的成员函数将被调用，这些函数的正确性可以保证。
     * note：这得益于c++20的概念，使得函数可以简单、优雅、正确地进行偏特化。
     */
    template <typename Tp, size_t Channels = 1ull, bool IsContinuous = true>
    class ChannelMajorMatrix {
        /**
         * 同一个模板、不同参数的矩阵为友元。
         * note: 本是同根生，相煎何太急。
         * note: 这一性质在roi、IsContinuous中得到应用。
         */
        template <typename Tp_, size_t Channels_, bool IsContinuous_>
        friend class ChannelMajorMatrix;

        //矩阵的原始内存区域
        std::shared_ptr<Tp> mOriginalData{ nullptr };
        size_t mOriginalChannelStep{ 0 };
        size_t mOriginalRowStep{ 0 };
        //矩阵的切片区域。
        MatSlice mSlice{ MatSlice::zero() };
    public:
        //1.构造
        // 1.1创造型构造器(实用)
        ChannelMajorMatrix() requires(IsContinuous) = default; //得益于shared_ptr, 我们不用操心。
    private:
        ChannelMajorMatrix() = default;  //不连续矩阵不允许外部构造. 但是允许内部由矩阵的设计者构造.
    public:
        ~ChannelMajorMatrix() = default;
        ChannelMajorMatrix(size_t rows, size_t cols) requires(IsContinuous);
        ChannelMajorMatrix(size_t mRows, size_t mCols, const std::array<Tp, Channels>& scalar) requires(IsContinuous);

    public:
        /**
         * 1.2 从外界数据创造 (重要, 许多算法的基础)
         * @param eData
         * @param rows
         * @param cols
         * note: 把内存管理的责任交给shared_ptr，是绝妙的设计。不仅代码简洁不易错，
         * 而且可以灵活地指定析构器为noFree或cv::fastFree，以此可以大幅提高神经网络的效率。
         * @seealso matRead, ChannelMajorMatrix(cv::Mat&& cvMatrix), 卷积层/池化层/全连接层构造函数
         * note: requires(IsContinuous)
         * 不允许ROI矩阵创造新的矩阵，这样会放弃ROI的身份，是不可以实现的。
         */
        ChannelMajorMatrix(std::shared_ptr<Tp> eData, size_t rows, size_t cols) requires(
            IsContinuous);

    public:
        //1.3 重新创造矩阵。
        //空矩阵也可以调用，所以在public当中. 行为是去除以往申请的矩阵，重新开始
        void create(size_t rows, size_t cols, const std::array<Tp, Channels>& scalar = {}) requires(
            IsContinuous);
        // 1.4 复制构造器
        ChannelMajorMatrix(const ChannelMajorMatrix<Tp, Channels, IsContinuous>& other) = default;
        // 2.2 移动构造器
        ChannelMajorMatrix(ChannelMajorMatrix<Tp, Channels, IsContinuous>&& other) SUSTech_NOEXCEPT = default;
        // 2. Project5 核心修改：切片逻辑、取值逻辑、形状逻辑、堆叠、展开 的逻辑。
        // 2.1 roi构造器
        /**
         * 2.1 roi构造器
         * 官方的不连续矩阵构造器，用且只能用于不连续矩阵的构造。
         * @tparam isContinuous_  requires(!IsContinuous) 取了roi的矩阵必然是不连续的。
         * @param other 原矩阵。可以是连续的也可是不连续的。
         * @param rowSlice
         * @param colSlice
         * note: 取了roi的矩阵必然是不连续的 是接口的约定。
         * 若要使用取了roi之后仍然是连续的特例（比如通道roi），使用的并不是这个成员函数
         * @seealso 成员函数channel(通道roi)
         */
        template <bool isContinuous_>
        ChannelMajorMatrix(const ChannelMajorMatrix<Tp, Channels, isContinuous_>& other, std::slice rowSlice,
                           std::slice colSlice) requires(!IsContinuous);
        ChannelMajorMatrix<Tp, Channels, false> operator()(std::slice rowSlice, std::slice colSlice) const;
        //2.2 添加：通道的roi特殊处理。
        /**
         * 通道的roi。
         * @param channel
         * @return 子矩阵
         * 实现困难：slice中没有channel。因此，share_ptr设置为noFree。
         * note：由于实现方式，父矩阵必须先行析构。这是合理的，通常情况都是如此。
         * note: 连续性：不要求连续， 原本是连续的，取了channel还是连续的，原本不是连续的，就不是连续的。
         * note：连续特化：连续矩阵调用这个函数速度更快。
         */
        ChannelMajorMatrix<Tp, 1, IsContinuous> channel(size_t channel) const;  // warning, base matrix must deallocate first.
        ChannelMajorMatrix<Tp, 1, IsContinuous> channel(size_t channel) const requires(IsContinuous);
        //2.3 取值逻辑
        //note：对于连续的矩阵，可以特化提高速度。
        Tp& operator()(size_t channel, size_t row, size_t col);  //可能会修改对象，不能用const。
        const Tp& operator()(size_t channel, size_t row, size_t col) const;
        Tp& operator()(size_t channel, size_t row, size_t col) requires(IsContinuous);
        const Tp& operator()(size_t channel, size_t row, size_t col) const requires(IsContinuous);

        //2.4 形状逻辑
        //note：形状的修改不会修改连续性。
        //reshape 修改的只是当前对象对数据的解释。
        void reshape(size_t rows, size_t cols);
        void reshape(size_t cols);
        //note: reshape和reshaped： reshaped 返回的是新的共享矩阵。
        ChannelMajorMatrix<Tp, Channels, IsContinuous> reshaped(size_t rows, size_t cols);
        ChannelMajorMatrix<Tp, Channels, IsContinuous> reshaped(size_t cols);
        //note: clang建议我标记nodiscard， 意思是调用这个函数之后，函数的返回值要用到，不要只是调用这个函数. 给予编译器检查空间。
        [[nodiscard]] size_t mChannels() const;
        [[nodiscard]] size_t mRows() const;
        [[nodiscard]] size_t mCols() const;
        [[nodiscard]] std::array<size_t, 3> shape() const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] size_t mElements() const;
        //2.5 堆叠、展开逻辑
        //展开支持不连续矩阵。
        const std::shared_ptr<Tp>& getMOriginalData() const;
        /**
         * note：数据会进行复制。
         * @return 分离通道后的几个矩阵
         */
        std::array<ChannelMajorMatrix<Tp, 1, true>, Channels> copySplit();
        /**
         * @return 分离通道后的几个矩阵
         * 数据共享, 警告，数据所有权在第一个通道手里。与channel方法的困难是一样的。
         * @seealso channel
         */
        std::array<ChannelMajorMatrix<Tp, 1, IsContinuous>, Channels>
        sharedSplit();  //
        //        std::array<ChannelMajorMatrix<Tp, 1, true>, Channels> sharedSplit() requires(IsContinuous);  //数据共享,

        /**
         * 重要，用于全连接层的flatten。
         * @return 返回多通道矩阵的单通道版本roi。
         * note: requires(IsContinuous)
         * 不连续矩阵不可能进行共享展开。
         * 不连续的矩阵要展开必须进行复制。
         */
        ChannelMajorMatrix<Tp, 1> sharedChannelFlatten() const requires(IsContinuous);

        //不同内存的矩阵允许内存不连续（是roi）。
        template <bool IsContinuous_>
        friend ChannelMajorMatrix<Tp, Channels, true>
        stackMatrices(const std::array<ChannelMajorMatrix<Tp, 1, IsContinuous_>, Channels>&
                          matrices);  //这里进行数据复制，以堆叠来自不同内存的矩阵。
                                      // OpenCV单通道矩阵可以转换过来
#if(SUPPORT_OPENCV)
        explicit ChannelMajorMatrix(cv::Mat&& cvMatrix) SUSTech_NOEXCEPT
            requires(Channels == 1, IsContinuous);  // stealing the matrix to be mine.
#endif
        //2.6 赋值逻辑和调试打印
        //在上面取值逻辑确定之后，就是顺水推舟的事情。
        //不管连续还是不连续，以下操作都支持。
        //对于连续矩阵，以下操作有特化版本, 以提高性能, 这也是我们引入模板参数IsContinuous的根本目的。
        //复制赋值
        ChannelMajorMatrix& operator=(const ChannelMajorMatrix<Tp, Channels, IsContinuous>& m) = default;
        //移动赋值
        ChannelMajorMatrix& operator=(ChannelMajorMatrix<Tp, Channels, IsContinuous>&& m) SUSTech_NOEXCEPT = default;
        /**
         * 重要。
         * @tparam F
         * @param lambda 对每个元素的操作。
         * @return 操作完成后的this。
         */
        template <typename F>
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& forEachValueApply(
            F lambda) /*requires(std::is_convertible_v<F, std::function<void(Tp&)>>)*/;  //note: 其他类型的lambda我不会调用。
        /**
         * 重要。
         * @tparam F
         * @param lambda 对每个元素的操作。
         * @return 操作完成后的this。
         * note：requires(IsContinuous)
         * 优化的核心，高速版本。
         */
        template <typename F>
        ChannelMajorMatrix<Tp, Channels, IsContinuous>&
        forEachValueApply(F lambda) requires(IsContinuous /*&&std::is_convertible_v<F, std::function<void(Tp&)>>*/);

        template <typename F>
        ChannelMajorMatrix<Tp, Channels, IsContinuous>&
        forEachScalarApply(std::array<F, Channels> lambdas) /*requires(std::is_convertible_v<F, std::function<void(Tp&)>>)*/
            ;  //只能分别对三个通道的三个元素进行不同的lambda处理。
        template <typename F>
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& forEachScalarApply(std::array<F, Channels> lambdas) requires(
            IsContinuous /*, std::is_convertible_v<F, std::function<void(Tp&)>>*/);

        //通道元素赋值
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator=(const std::array<Tp, Channels>& scalar);
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator=(const std::array<Tp, Channels>& scalar) requires(IsContinuous);
        //所有元素赋值
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator=(const Tp& value);
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator+=(const Tp& value);
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator-=(const Tp& value);
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator*=(const Tp& value);
        ChannelMajorMatrix<Tp, Channels, IsContinuous>& operator/=(const Tp& value);

        //2.7 数学计算
        //不管连续还是不连续，以下操作都支持。对于连续矩阵，以下操作有特化版本。
        template <typename Tp2, bool IsContinuous2>
        auto operator+(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
            -> ChannelMajorMatrix<decltype(Tp() + Tp2()), Channels, true>;

        template <typename Tp2, bool IsContinuous2>
        auto operator*(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
            -> ChannelMajorMatrix<decltype(Tp() * Tp2()), Channels, true>;

        template <typename Tp2, bool IsContinuous2>
        auto matMul(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other) const
            -> ChannelMajorMatrix<decltype(Tp() * Tp2()), Channels, true>;

#define DeclResult decltype(Tp() * Tp2() + Tp() * Tp2())
        template <typename Tp2, bool IsContinuous2>
        auto inner_product(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other, DeclResult initialValue = {}) const
            -> DeclResult;

        template <typename Tp2, bool IsContinuous2>
        auto inner_product(const ChannelMajorMatrix<Tp2, Channels, IsContinuous2>& other, DeclResult initialValue = {}) const
            -> DeclResult requires(IsContinuous);
        //调试打印
        std::string toPrettyString() const ;
        //序列化打印
        template <typename Tp_, size_t Channels_, bool IsContinuous_>
        friend std::ostream& operator<<(std::ostream& os, const ChannelMajorMatrix<Tp_, Channels_, IsContinuous_>& matrix);
        //矩阵输入
    public:
        //无需知道size，从文件自动快速推断。但是需要通过模板来指定通道数。
        static ChannelMajorMatrix<Tp, Channels, true> matRead(const std::string& file_name) requires(std::is_same_v<Tp, float>,
                                                                                                     Channels == 1, IsContinuous);
        //根据当前Mat的信息，从输入流读入恰当数量的元素填入当前矩阵。
        //支持不连续矩阵。对连续矩阵可以特化。
        template <typename Tp_, size_t Channels_, bool IsContinuous_>
        friend std::istream& operator>>(std::istream& in, ChannelMajorMatrix<Tp_, Channels_, IsContinuous_>& mat);
    };
    /**
     * 2D矩阵的定义。
     * 行列主序矩阵的定义。
     */
    template <typename Tp, bool IsContinuous = true>
    using ChannelMajorMatrix2D = ChannelMajorMatrix<Tp, 1, IsContinuous>;  // C++11
    // and I can specialize the implementation of methods of channel 1 if I want. Here it is more convenient for us to specialize
    // functions.
    template <typename Tp, size_t Channels, bool IsContinuous = true>
    using RowColMajorMatrix = ChannelMajorMatrix2D<std::array<Tp, Channels>,
                                                   IsContinuous>;  //更准确的定义需要 非公有继承 来实现，本次project暂时不用到。
}  // namespace SUSTech
#include "ChannelMajorMatrix.ipp"
#include "MatrixConcepts.ipp"
