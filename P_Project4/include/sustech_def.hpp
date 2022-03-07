//
// Created by 叶璨铭 on 2021/11/23.
//
#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
// opencv的interface.h的功能也放到这个里面了。
//注意宏定义不能加分号
#if defined __GNUC__
#define __SUSTC_Deprecated
#define SUSTC_Deprecated__ __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define __SUSTC_Deprecated __declspec(deprecated)
#define SUSTC_Deprecated__
#else
#define __SUSTC_Deprecated
#define SUSTC_Deprecated__
#endif

#define SUSTech_FINAL final
#define SUSTech_NOEXCEPT noexcept
#ifndef SUSTech_ALWAYS_INLINE
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define SUSTech_ALWAYS_INLINE inline /* __attribute__((always_inline)) */
#elif defined(_MSC_VER)
#define SUSTech_ALWAYS_INLINE  __forceinline  /*inline*/
#else
#define SUSTech_ALWAYS_INLINE inline
#endif
#endif
#define SUSTech_Assert(expectedCondition, exception) \
    do {                                             \
        if(!(expectedCondition))                     \
            throw exception;                         \
    } while(false)

#include <atomic>
using SUSTechAtomicInt = std::atomic<std::int32_t>;
namespace SUSTech {

    //类型的定义：比opencv类型要少。
    // 1.最常用的四个都有了。
    // 2.需要重新计算二进制量，检验我是否真的看懂了 opencv的类型计算
    constexpr int SUSTech_CHANNEL_MAX = 1 << 12;
    constexpr int SUSTech_CHANNEL_SHIFT = 4;
    constexpr int SUSTech_CHANNEL_MASK = ((SUSTech_CHANNEL_MAX - 1) << SUSTech_CHANNEL_SHIFT);
    constexpr int SUSTech_DEPTH_MAX = (1 << SUSTech_CHANNEL_SHIFT);
    constexpr int SUSTech_MAT_DEPTH_MASK = SUSTech_DEPTH_MAX - 1;
    enum depth_order { SUSTech_8U, SUSTech_32S, SUSTech_32F, SUSTech_64F };
    constexpr std::array<int, 4> depth_step{ 1, 4, 4, 8 };  //避免opencv的魔法数字移位方法。
    constexpr std::array<const char*, 4> depth_name{ "SUSTech_8U", "SUSTech_32S", "SUSTech_32F", "SUSTech_64F"};
    constexpr double convertTo64F(double d) {
        return d;
    }
    constexpr float convertTo32F(double d) {
        return static_cast<float>(d);
    }
    constexpr int convertTo32S(double d) {
        return static_cast<int>(d);
    }
    constexpr uint8_t convertTo8U(double d) {
        return static_cast<uint8_t>(d);
    }
    template <int depth>
    struct depth_to_type;
    template <>
    struct depth_to_type<SUSTech_8U> {
        static double convert(double d) {
            return static_cast<uint8_t>(d);
        }
        static uint8_t value;
    };
    template <>
    struct depth_to_type<SUSTech_32S> {
        static double convert(double d) {
            return static_cast<int>(d);
        }
        static int32_t value;
    };
    template <>
    struct depth_to_type<SUSTech_32F> {
        static double convert(double d) {
            return static_cast<float>(d);
        }
        static float value;
    };
    template <>
    struct depth_to_type<SUSTech_64F> {
        static double convert(double d) {
            return d;
        }
        static double value;
    };  //学习了一波标准库, 大概是这样写的吧
        //    constexpr auto convert(double d, int depth)->decltype(depth_to_type<depthOrder>::value){
        //    //decltype推断的前提是类型确定。这里编译器认为depth是变量，不能被发现是什么
        //        switch((SUSTech::depth_order)depthOrder){
        //            case SUSTech_8U:
        //                return convertTo8U(d);
        //            case SUSTech_32S:
        //                return convertTo32S(d);
        //            case SUSTech_32F:
        //                return convertTo32F(d);
        //            case SUSTech_64F:
        //                return convertTo64F(d);
        //            default:
        //                throw std::runtime_error("new type cast not supported!");
        //        }
        //    }
    SUSTech_ALWAYS_INLINE double doubleValueOf(const void* block, int depth) {  //不能是constexpr，因为throw exception
        switch((SUSTech::depth_order)depth) {
            case SUSTech_8U: {
                auto concretePtr = reinterpret_cast<const decltype(depth_to_type<SUSTech_8U>::value)*>(block);
                return static_cast<double>(*concretePtr);
            }
            case SUSTech_32S: {
                auto concretePtr = reinterpret_cast<const decltype(depth_to_type<SUSTech_32S>::value)*>(block);
                return static_cast<double>(*concretePtr);
            }
            case SUSTech_32F: {
                auto concretePtr = reinterpret_cast<const decltype(depth_to_type<SUSTech_32F>::value)*>(block);
                return static_cast<double>(*concretePtr);
            }
            case SUSTech_64F: {
                auto concretePtr = reinterpret_cast<const decltype(depth_to_type<SUSTech_64F>::value)*>(block);
                return static_cast<double>(*concretePtr);
            }
            default:
                throw std::runtime_error("new type cast not supported!");
        }
        return 0;
    }

    constexpr int SUSTech_MAT_DEPTH(int type) {
        return (type & SUSTech_MAT_DEPTH_MASK);
    }
    constexpr int SUSTech_MAKE_TYPE(int depth, int channel) {
        return SUSTech_MAT_DEPTH(depth) + (((channel)-1) << SUSTech_CHANNEL_SHIFT);  //减1是因为0通道不存在。
    }
    constexpr int SUSTech_8UC(int channel) {
        return SUSTech_MAKE_TYPE(SUSTech_8U, channel);
    }
    constexpr int SUSTech_32SC(int channel) {
        return SUSTech_MAKE_TYPE(SUSTech_32S, channel);
    }
    constexpr int SUSTech_32FC(int channel) {
        return SUSTech_MAKE_TYPE(SUSTech_32F, channel);
    }
    constexpr int SUSTech_64FC(int channel) {
        return SUSTech_MAKE_TYPE(SUSTech_64F, channel);
    }
    enum type {
        SUSTech_8UC1 = SUSTech_8UC(1),
        SUSTech_8UC2 = SUSTech_8UC(2),
        SUSTech_8UC3 = SUSTech_8UC(3),
        SUSTech_8UC4 = SUSTech_8UC(4),

        SUSTech_32SC1 = SUSTech_32SC(1),
        SUSTech_32SC2 = SUSTech_32SC(2),
        SUSTech_32SC3 = SUSTech_32SC(3),
        SUSTech_32SC4 = SUSTech_32SC(4),

        SUSTech_32FC1 = SUSTech_32FC(1),
        SUSTech_32FC2 = SUSTech_32FC(2),
        SUSTech_32FC3 = SUSTech_32FC(3),
        SUSTech_32FC4 = SUSTech_32FC(4),

        SUSTech_64FC1 = SUSTech_64FC(1),
        SUSTech_64FC2 = SUSTech_64FC(2),
        SUSTech_64FC3 = SUSTech_64FC(3),
        SUSTech_64FC4 = SUSTech_64FC(4)
    };
    /**
     * 有了个结构体，不仅容易分离Mat的逻辑、更好的修改flag的定义，还可以避免用masks不断位操作浪费时间。（前提是取结构体的成员快于移位）
     * 坏处：使用mask的时候，如果用int8，int16，需要进行类型转换为int32，再移位。比如这里的mType不是一个好设计。
     * 但是我认为不比flags直接位操作差太多。
     */
    struct MatFlag {
        enum MatrixClassSignature { NormalMatrixSignature = 0x42 };
        enum BooleanFlags { ContinuityFlag = 1 << 4, SubMatrixFlag = 1 << 0 };
        int8_t mSignature;  //表明是Mat还是SparseMat还是std::vector。这样InputArray就可以区分了
                            //(为什么不是这些类型的矩阵继承InputArray呢, 是有原因的)。
        int8_t mFlags;      // flags包括continuity(左4位)和isSubMatrix(右4位)
        int16_t mType;      /*- depthOrder (float、double、int等等),
     这里设计占4位（opencv是三位），至多支持16种数据类型(深刻的问题：为用户实现高效的bool和float16矩阵)
- number of channels（depth和channel在一起合称type）, 这里占12位，支持2**12的通道。
因为矩阵构造的时候就传了一个type进来，所以我没有把depth和channel拆开。*/
        bool isNormalMatrix() const;
        void flipContinuityFlag();
        bool isContinuous() const;
        void flipSubMatrixFlag();
        bool isSubMatrix() const;
        int depthOrder() const;
        int depthStep() const;
        std::string depthName() const;
        int channels() const;
        bool operator==(const MatFlag& rhs) const;
        bool operator!=(const MatFlag& rhs) const;
    };
    SUSTech_ALWAYS_INLINE bool MatFlag::isNormalMatrix() const {
        return mSignature == NormalMatrixSignature;
    }
    SUSTech_ALWAYS_INLINE bool MatFlag::isContinuous() const {
        return (mFlags & ContinuityFlag) != 0;
    }
    SUSTech_ALWAYS_INLINE void MatFlag::flipContinuityFlag() {
        mFlags ^= ContinuityFlag;
    }

    SUSTech_ALWAYS_INLINE bool MatFlag::isSubMatrix() const {
        return (mFlags & SubMatrixFlag) != 0;
    }
    SUSTech_ALWAYS_INLINE void MatFlag::flipSubMatrixFlag() {
        mFlags ^= SubMatrixFlag;
    }
    SUSTech_ALWAYS_INLINE int MatFlag::depthOrder() const {
        return mType & SUSTech_MAT_DEPTH_MASK;
    }
    SUSTech_ALWAYS_INLINE int MatFlag::channels() const {
        return ((mType & SUSTech_CHANNEL_MASK) >> SUSTech_CHANNEL_SHIFT) + 1;
    }
    SUSTech_ALWAYS_INLINE int MatFlag::depthStep() const {
        return depth_step[depthOrder()];
    }
    SUSTech_ALWAYS_INLINE
    std::string MatFlag::depthName() const {
        return {depth_name[depthOrder()]};
    }
    SUSTech_ALWAYS_INLINE
    bool MatFlag::operator==(const MatFlag& rhs) const {
        return mSignature == rhs.mSignature && mFlags == rhs.mFlags && mType == rhs.mType;
    }
    SUSTech_ALWAYS_INLINE
    bool MatFlag::operator!=(const MatFlag& rhs) const {
        return !(rhs == *this);
    }

    //    using GenericPtr = union {
    //        uint8_t* uc8;  // unsigned char
    //        int32_t* i32;  // int
    //        float* f32;    // float
    //        double* f64;   // double
    //    };
}  // namespace SUSTech
