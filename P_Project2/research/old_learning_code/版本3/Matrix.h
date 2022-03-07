//
// Created by 叶璨铭 on 2021/9/29.
//

#ifndef P_PROJECT2_MATRIX_H
#define P_PROJECT2_MATRIX_H
/*
* 一开始，没有想用类，就是写了个简单的结构体，能乘就行了。
* 后来感觉有必要写个类
 * 由于还没学到c++类的设计，这个类可能和真正c++的类思想不一样。
 * 这个matrix的意思是，一个数据不可变对象。
 * 只有乘法、加法才能产生新对象，或者进行构造才能产生新对象。
 * （所以没有像c++的写法，c++一般是先实现了+=，然后+才是次要的，+必须依赖于对象复制，作为学过java还没学c++的同学，我暂时不认可）
 * 所以，我没有写拷贝构造函数，operator=我觉得和java一样不就行了，两个变量指向同一块内存，语义不必是复制整个内容。（目前这样认为）
 * 
 * 这个matrix独特的地方在于submatrix
 * 
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <type_traits> //判断模板的类型
#include <omp.h> //openmp 并发计算
template<typename E>
class Matrix { //class 智能定义一次。
public:
    Matrix(E **data, size_t rowCount, size_t columnCount);//外部构造E**. 封成一个matrix。
    Matrix(size_t rowCount, size_t columnCount);//初始化普通构造，其实只是把手动的构造一个确定大小的二维数组的过程进行了合理的封装。
    ~Matrix();//析构。封好的E**可以被构造
    const E& get(size_t rowIndex, size_t columnIndex);//const 引用。防止对象传递时复制；防止外面修改矩阵的E**

    Matrix<E> Matrix<E>::*sub_matrix(size_t firstRow, size_t firstColumn, size_t rowCount, size_t columnCount);
    /*乘法的一系列函数
    * 原本只想暴露file_matrix_multiplication和operator*。但是google benchmark也要访问到。
    */

    static//一般c++写法是写在类的外面。我觉得应该写在类里面，而不是全局的。相当于命名空间保护了。
    Matrix<E>* _concurrent_continuous_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b); //似乎不能返回引用，日后再探究。先用指针返回不会错。
    static
    Matrix<E>* _continuous_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b); 
    static
    Matrix<E>* _trivial_multiplication(const Matrix<E>& matrix_a, const Matrix<E>& matrix_b);
    /**
     * 涉及文件的一些函数
     */
    friend
    Matrix<E> *operator*(const Matrix<E> &matrix_a, const Matrix<E> &matrix_b);
    static
    Matrix<E>* readMatrixFromFile(const char* filename);
    static
    void writeMatrixToFile(const Matrix<E>* matrix, const char* filename);
    static
    void file_matrix_multiplication(char* in_out_filenames[]);//TODO:不能传char*[3]到const char**
private:
    E **data = nullptr; //[行坐标][列坐标]
    size_t firstRow = 0;
    size_t firstColumn = 0;
    size_t rowCount = 0;
    size_t columnCount = 0;
    static const size_t matrix_max_size = 4096;

    Matrix *fatherMatrix = nullptr; //为了子数组的析构问题，如果我是子数组，我就不析构，如果我是父数组，我就析构
};



#endif //P_PROJECT2_MATRIX_H
