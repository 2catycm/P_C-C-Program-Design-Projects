//
// Created by 叶璨铭 on 2021/11/23.
//
#pragma once
#include "sustech_def.hpp"
#include <array>
#include <ostream>
#include <typeinfo>
namespace SUSTech {
    // opencv写了很多其实编译器可以帮我们写。
    template <typename Tp>
    class Point_ SUSTech_FINAL {  //写成aggregate就可以不用写拷贝、移动了。
    public:
        //        using Size<Tp> = Point_<Tp>;//元编程不能完成的任务。
        Tp x, y;
        template <typename Tp_>
        friend std::ostream& operator<<(std::ostream& os, const Point_<Tp_>& point);
    };
    using Point2i = Point_<int>;
    using Point2st = Point_<size_t>;
    using Point = Point2st;

    template <typename Tp>
    class Rect_ SUSTech_FINAL {
    public:
        Tp x;       //!< x coordinate of the top-left corner
        Tp y;       //!< y coordinate of the top-left corner
        Tp width;   //!< width of the rectangle
        Tp height;  //!< height of the rectangle
        //! area (width*height) of the rectangle
        Tp area() const;
        //! true if empty
        bool empty() const;
        //! conversion to another data type
        template <typename Tp2>
        explicit operator Rect_<Tp2>() const;
        template <typename Tp2_>
        friend std::ostream& operator<<(std::ostream& os, const Rect_<Tp2_>& rect);
    };
    using Recti = Rect_<int>;
    using Rectst = Rect_<size_t>;
    using Rect = Rectst;

    template <typename Tp>
    class Scalar_ SUSTech_FINAL {
    public:
        std::array<Tp, 4> mScalar;
        Tp& s0() const;
        Tp& s1() const;
        Tp& s2() const;
        Tp& s3() const;
        explicit operator std::array<Tp, 4>() const;
        template <typename Tp_>
        friend std::ostream& operator<<(std::ostream& os, const Scalar_<Tp_>& scalar);
    };
    using Scalar = Scalar_<double>;

    template <typename Tp>
    class VarScalar_ {
        std::vector<Tp> mVector; //不能继承标准库的成员（因为不保证是virtual的），所以要给他加功能，只能自己适配。
    public:
        //复制构造和移动构造,以及赋值，都是默认我想要的功能，就是把mVector复制、移动。
        VarScalar_();
        explicit VarScalar_(size_t initialCapacity);
        void push_back(const Tp& val);
        void push_back(Tp&& val);
        Tp& operator[](size_t rank);
        bool operator==(const VarScalar_& rhs) const;
        bool operator!=(const VarScalar_& rhs) const;
        template <typename Tp_>
        friend std::ostream& operator<<(std::ostream& os, const VarScalar_<Tp_>& scalar);
    };
    using VarScalar = VarScalar_<double>;

    //////////////////////////////////////Point_ ///////////////////////////////////
    template <typename Tp>
    std::ostream& operator<<(std::ostream& os, const Point_<Tp>& point) {
        os << "Point_<" << typeid(Tp).name() << ">{" << point.x << ", " << point.y << "}";
        return os;
    }
    //////////////////////////////////////Rect_ ///////////////////////////////////
    //类型转换
    //要写两个template，是因为类本身只有一个模板参数，而不是两个。
    template <typename Tp>
    template <typename Tp2>
    SUSTech_ALWAYS_INLINE Rect_<Tp>::operator Rect_<Tp2>() const {
        return Rect_<Tp2>(static_cast<Tp2>(x), static_cast<Tp2>(y), static_cast<Tp2>(width), static_cast<Tp2>(height));
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Tp Rect_<Tp>::area() const {
        return width * height;  // int，uchar的话有可能溢出的。
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE bool Rect_<Tp>::empty() const {
        return width <= 0 || height <= 0;
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, const Rect_<Tp>& rect) {
        os << "Rect_<" << typeid(Tp).name() << ">{"
           << "x: " << rect.x << " y: " << rect.y << " width: " << rect.width << " height: " << rect.height << "}";
        return os;
    }
    //////////////////////////////////////Scalar_ ///////////////////////////////////
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Tp& Scalar_<Tp>::s0() const {
        return mScalar[0];
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Tp& Scalar_<Tp>::s1() const {
        return mScalar[1];
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Tp& Scalar_<Tp>::s2() const {
        return mScalar[2];
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Tp& Scalar_<Tp>::s3() const {
        return mScalar[3];
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, const Scalar_<Tp>& scalar) {
        os << "Scalar_<" << typeid(Tp).name() << ">{";
        for(int i = 0; i < 3; ++i) {
            if(scalar.mScalar[i] == 0)
                break;
            os << scalar.mScalar[i] << ", ";
        }
        if(scalar.mScalar[3] == 0)
            os << scalar.mScalar[3];
        return os;
    }
    template <typename Tp>
    SUSTech_ALWAYS_INLINE Scalar_<Tp>::operator std::array<Tp, 4>() const {
        return mScalar;
    }
    template <typename Cvt, typename Tp, size_t Channels>
    SUSTech_ALWAYS_INLINE std::array<Cvt, Channels> convert(const std::array<Tp, Channels>& src) {
        std::array<Cvt, Channels> newArray;
        for(int i = 0; i < Channels; ++i) {
            newArray[i] = static_cast<Cvt>(src[i]);
        }
        return newArray;  // move constructor.
    }
    //////////////////////////////////////VarScalar_ ///////////////////////////////////
    //不好，不应该修改标准库。
    //    using VarScalar = std::vector<double>;
    //    std::ostream& operator<<(std::ostream& os, const VarScalar& vec){
    //        for(const auto& e:vec)
    //            os << e;
    //        return os;
    //    }
    template <typename Tp>
    bool VarScalar_<Tp>::operator==(const VarScalar_& rhs) const {
        return mVector == rhs.mVector;
    }
    template <typename Tp>
    bool VarScalar_<Tp>::operator!=(const VarScalar_& rhs) const {
        return rhs != *this;
    }
    template <typename Tp>
    Tp& VarScalar_<Tp>::operator[](const size_t rank) {
        return mVector[rank];
    }
    template <typename Tp>
    std::ostream& operator<<(std::ostream& os, const VarScalar_<Tp>& scalar) {
        os<<"(";
        int i;
        for(i = 0; i < scalar.mVector.size()-1; ++i) {
            os<<scalar.mVector[i]<<", ";
        }
        return os<<scalar.mVector[i]<<")";
    }
    template <typename Tp>
    VarScalar_<Tp>::VarScalar_(size_t initialCapacity) :mVector(initialCapacity){}
    template <typename Tp>
    VarScalar_<Tp>::VarScalar_() :VarScalar_(0){}
    template <typename Tp>
    void VarScalar_<Tp>::push_back(const Tp& val) {
        mVector.push_back(val);
    }
    template <typename Tp>
    void VarScalar_<Tp>::push_back(Tp&& val) {
        mVector.push_back(val);
    }

    SUSTech_ALWAYS_INLINE size_t file_len(FILE* file) {
        fseek(file, 0L, SEEK_END);
        size_t result = ftell(file);
        fseek(file, 0L, SEEK_SET);
        return result;
    }
}  // namespace SUSTech
