//
// Created by 叶璨铭 on 2021/11/23.
//
#pragma once
#include "sustech_simd_intrinsics.hpp"
#include "sustech_def.hpp"
#include "types.hpp"
#include <atomic>
#include <cstdint>
#include <ostream>
#include <vector>
namespace SUSTech {
    class MatExpr;
    class Mat;
    class BadArgumentSize:public std::runtime_error{
    public:
        BadArgumentSize(const std::string& message);
    };
    class Mat {
    public:
        //创造构造器
        Mat();  //默认构造器的意思是就创建了一个header，需要用create函数或者被别的处理函数（如matRead）来具体分配。
        Mat(size_t rows, size_t cols, int type);
        Mat(size_t rows, size_t cols, int type, const Scalar& s);//前三个构造函数同属一个重载序列。
    private:
        Mat(size_t rows, size_t cols, int type, void* data);    //第四个让矩阵的来源于已经分配好的内存，提供给matRead函数使用。
        //注意，这里的设计让Mat就像一个shared_ptr一样，会保护data。如果最后一个Mat析构，data会被矩阵释放。(opencv不会, 机制较为复杂)
        //opencv是因为友元函数多才不封装，看不下去了。这个函数很容易用错，很多东西会转换成void*.
    public:
        //无需知道size，从文件推断。
        static Mat matRead(const std::string& file_name);
        //根据当前Mat的信息，从输入流读入恰当数量的元素填入当前矩阵。
        friend std::istream&  operator>>(std::istream &in, Mat& mat);
    private: void create();
    public:  void create(size_t rows, size_t cols, int type, const Scalar& s = {});//空矩阵也可以调用，所以在public当中. 行为是去除以往申请的矩阵，重新开始。
        //复制构造器(opencv语义，不是c++语义)
        Mat(const Mat& m);
        //移动构造器
        Mat(Mat&& m) SUSTech_NOEXCEPT; //虽然我这个矩阵复制开销不大，移动构造器仍是必须的， 否则函数栈上返回会出问题、复杂表达式求值无法执行拷贝消除。
        //ROI构造器
        Mat(Mat  m, const Rect& roiRect);
        Mat(Mat  m, const int& channel);
        //析构函数
         ~Mat();//除了析构函数，还有很多地方需要释放。因此指针释放委托给release, Mat本身的释放（内存空间的清空）委托给系统的析构。
    private:void release(); //释放的语义是对与当前矩阵所用的信息而言，放弃当前矩阵对originalData的共享权。
    private:void destroy(); //如果释放发现this是最后拥有originalData的矩阵，销毁originalData。
    public:
        //复制赋值(opencv语义，不是c++语义)
        Mat& operator=(const Mat& m);
        //移动赋值
        Mat& operator=(Mat&& m) SUSTech_NOEXCEPT;
        //真正内部使用的指针求位
//#define GetElementPtr(rowIndex, colIndex, channelIndex, CASE)
        //单元素取值, 由于channel的性质，暂时不能修改。//No: (或许要实现一个channel类才好, 或者视作三维矩阵才行)
        //operator(), 这里把operator()与operator[]区分开，operator()的目的只是简单读取。
        VarScalar operator()(size_t rowIndex, size_t colIndex) const;
        VarScalar operator()(const Point& location) const;
        // operator[]是为了切片（roi）, 甚至为了修改roi（切片），虽然用=Mat不能修改，但是允许并且鼓励用=Scalar
        //注意，下面的const只是表面的。
        Mat operator[](const Rect& roiRect) const; //虽然不是返回mat的引用，但是实际上就是通过roi引用了原矩阵的一部分。
        Mat operator[](const Point& location) const;
        Mat row(size_t rowIndex) const;
        Mat col(size_t colIndex) const;
        Mat channel(int channelIndex) const;
        Mat operator[](size_t rowsThenColumnsIndex) const;
        //调试函数
        friend std::ostream& operator<<(std::ostream& os, const Mat& mat);
        //全元素操作
        template<typename Tp, size_t channels>
        Mat& operator = (const std::array<Tp, channels>& s); //全员赋值

        Mat& operator = (const Scalar& s);

        bool empty() const;
        // 类型转换

        //数学运算(非MatExpr版本), 与opencv语义不同
        Mat& operator+=(const Mat& matB);
        Mat operator+(const Mat& matB);
        Mat& operator-=(const Mat& matB);
        Mat operator-(const Mat& matB);
    private:
        friend Mat& fast_matrix_element_by_element_linear_combinations_assignment(Mat& ths, const Mat& matrixA,
                                                                     const Mat& matrixB, float alpha,
                                                                     float beta);
    public:
        Mat matMul(const Mat& matB);
    private:  // opencv用的是public。
        static constexpr int dims = 2;                                      //本次project只实现二维矩阵，但是对扩展开放。
        MatFlag mFlag;
        size_t mRows, mCols;
        uint8_t* mData {nullptr};
        uint8_t* mDataEnd {nullptr};
        size_t mOriginalDataStepCol, mOriginalDataStepRow;  // step0是从一行到另一行（第一维），所需要的指针字节偏移数。step1是从一列到另一列（第二维）的偏移数
                                                            // （本次project是二维矩阵，所以step1也就是一个元素的字节大小。）
                                                            // （注意，这里说的元素包括了channel）
        //这里的左右顺序是有讲究的。
        SUSTechAtomicInt* mOriginalDataRefCount{nullptr};  // allocated and deallocated together with mOriginalDataStart.
        uint8_t* mOriginalDataStart{nullptr};//uint8_t* 是指step按照字节来计算。const是指Mat操作data应该根据mData指针来进行，而不是mOriginalDataStart 。
        uint8_t* mOriginalDataEnd{nullptr};//这里start和end语义与opencv不同。
    };
//    class SingleMatExpr{
//        Mat
//    };
    class MatExpr{
    public:
        Mat& A,B,C;
        Scalar a, b, c;
    };
}  // namespace SUSTech
