#pragma once
#include <string>
#include <stdexcept>
#include <array>
//disable the old codes from project 4
#define SUSTech_New_Standard 1
//final and noexcept
#define SUSTech_FINAL final
#define SUSTech_NOEXCEPT noexcept
// Deprecated annotations
#if defined __GNUCXX__
#define __SUSTC_Deprecated
#define SUSTC_Deprecated__ __attribute__((deprecated))
#elif defined(_MSC_VER)
#define __SUSTC_Deprecated __declspec(deprecated)
#define SUSTC_Deprecated__
#else
#define __SUSTC_Deprecated
#define SUSTC_Deprecated__
#endif
//inline
#ifndef SUSTech_ALWAYS_INLINE
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define SUSTech_ALWAYS_INLINE inline /* __attribute__((always_inline)) */
#elif defined(_MSC_VER)
#define SUSTech_ALWAYS_INLINE  __forceinline  /*inline*/
#else
#define SUSTech_ALWAYS_INLINE inline
#endif
#endif
//用内联函数代替宏定义，这段代码废弃
# if (SUSTech_New_Standard)
#else
#ifdef _DEBUG
#define SUSTech_Assert(expectedCondition, exception, message) \
    do {                                             \
        if(!(expectedCondition))                     \
            throw exception(message);                         \
    } while(false)
#else
#define SUSTech_Assert
#endif
#endif

//#define SUSTech_REQUIRES(constexprCondition) typename=typename std::enable_if_t<constexprCondition>
namespace SUSTech {
    struct SUSTechError:public std::runtime_error {
        explicit SUSTechError(const std::string& message) : runtime_error("SUSTechError: "+message) {}
    };
    /**
     * Assert is widely used to detect errors.
     * Only in debug mode, assertion is activated.
     * In release mode, assertion here does not work for performance considerations, like what java does.
     * @tparam Ex exception type. Note that if the condition is satisfied, we will not construct a new exception object, which may slow down the speed.
     * @param expectedCondition
     * @param message
     */
    template<typename Ex = SUSTechError> inline
    void sustechDynamicAssert(bool expectedCondition, const std::string& message = ""){
#ifndef NDEBUG
        if(!(expectedCondition))
            throw Ex{message};
#endif
    }
    /**
     * If channel major matrix gets its data from external source,
     * (typically from the CNN weights and bias float*, or from OpenCV imread)
     * we want to avoid copy,
     * while as well the data would not be double-freed.
     * So we let the share_ptr of the matrix and its future sub matrices not to free the data.
     */
    void noFree(void*){}

    namespace std20 {
        /**
     * source： https://zh.cppreference.com/w/cpp/container/array/to_array
     *  a possible implementation of C++ 20 std::to_array.
     *  Note: This "to_array" copies but not share. But it is as fast or even faster than C memcpy thanks to great C++ standard library.
         */
        template <class T, std::size_t N, std::size_t... I>
        constexpr std::array<std::remove_cv_t<T>, N> to_array_impl(T (&a)[N], std::index_sequence<I...>) {
            return { { a[I]... } };
        }
        template <class T, std::size_t N>
        constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
            return to_array_impl(a, std::make_index_sequence<N>{});
        }
        /**
     * source： https://zh.cppreference.com/w/cpp/algorithm/accumulate
     *  a possible implementation of C++ 20 std::accumulate.
         */
        template <class InputIt, class T>
        constexpr  // C++20 起
            T
            accumulate(InputIt first, InputIt last, T init) {
            for(; first != last; ++first) {
                init = std::move(init) + *first;  // C++20 起有 std::move
            }
            return init;
        }
        /**
         * source: https://zh.cppreference.com/w/cpp/algorithm/inner_product
         */
        template<class InputIt1, class InputIt2, class T>
        constexpr // C++20 起
            T inner_product(InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, T init)
        {
            while (first1 != last1) {
                init = std::move(init) + *first1 * *first2; // C++20 起有 std::move
                ++first1;
                ++first2;
            }
            return init;
        }
        template<class ForwardIt>
        ForwardIt max_element(ForwardIt first, ForwardIt last)
        {
            if (first == last) {
                return last;
            }
            ForwardIt largest = first;
            ++first;
            for (; first != last; ++first) {
                if (*largest < *first) {
                    largest = first;
                }
            }
            return largest;
        }
    }
    namespace caffe{
    /**
     * source： caffe. Best way to know whether 0<=a<b;
     */
    inline bool is_a_ge_zero_and_a_lt_b(int64_t a, int64_t b) {
        return static_cast<unsigned>(a) < static_cast<unsigned>(b);
    }
    }
    namespace utils{
        inline size_t file_len(FILE* file) {
            fseek(file, 0L, SEEK_END);
            size_t result = ftell(file);
            fseek(file, 0L, SEEK_SET);
            return result;
        }
    }
}  // namespace SUSTech