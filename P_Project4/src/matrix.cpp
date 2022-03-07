//
// Created by 叶璨铭 on 2021/11/23.
//
#include "mat.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
#include <omp.h>
namespace SUSTech {
#define GetElementPtr_l3_frd(rowIndex, colIndex, channelIndex, CASE, ths)  (reinterpret_cast<decltype(depth_to_type<CASE>::value)*>((ths).mData+(rowIndex)*(ths).mOriginalDataStepRow+(colIndex)*(ths).mOriginalDataStepCol+(channelIndex)*(ths).mFlag.depthStep()))
#define GetElementPtr_l1_frd(depthSteps,CASE, ths) (reinterpret_cast<decltype(depth_to_type<CASE>::value)*>((ths).mData+(depthSteps)*(ths).mFlag.depthStep()))
    BadArgumentSize::BadArgumentSize(const std::string& message) : runtime_error(message) {}
    // Mat
    Mat::Mat() : Mat(0, 0, 0) {}
    Mat::Mat(size_t rows, size_t cols, int type) : mFlag(MatFlag{ MatFlag::NormalMatrixSignature, MatFlag::ContinuityFlag, static_cast<int16_t>(type) }), mRows(rows),
          mCols(cols), mData(nullptr), mDataEnd(nullptr), mOriginalDataRefCount(nullptr),
          mOriginalDataStepCol(mFlag.depthStep() * mFlag.channels()), mOriginalDataStepRow(mCols * mOriginalDataStepCol),
          mOriginalDataStart(nullptr), mOriginalDataEnd(nullptr) {
        if(!empty())
            create();
        //不会赋值为0.
    }
    Mat::Mat(size_t rows, size_t cols, int type, const Scalar& s)
        : mFlag(MatFlag{ MatFlag::NormalMatrixSignature, MatFlag::ContinuityFlag, static_cast<int16_t>(type) }), mRows(rows),
          mCols(cols), mData(nullptr), mDataEnd(nullptr), mOriginalDataRefCount(nullptr),
          mOriginalDataStepCol(mFlag.depthStep() * mFlag.channels()), mOriginalDataStepRow(mCols * mOriginalDataStepCol),
          mOriginalDataStart(nullptr), mOriginalDataEnd(nullptr) {
        if(!empty())
            create();
        *this = static_cast<std::array<double, 4>>(s);
    }

    void Mat::create(size_t rows, size_t cols, int type, const Scalar& s) {
        if(!empty())
            release();  //将旧的矩阵释放了。 并且保证以往数据都是0.
        //构造新的矩阵头信息。
        mFlag = MatFlag{ MatFlag::NormalMatrixSignature, MatFlag::ContinuityFlag, static_cast<int16_t>(type) };
        mRows = rows;
        mCols = cols;
        mOriginalDataStepCol = mFlag.depthStep() * mFlag.channels();
        mOriginalDataStepRow = mCols * mOriginalDataStepCol;
        create();
    }

    void Mat::create() {
        mOriginalDataRefCount = new SUSTechAtomicInt{ 1 };
        mOriginalDataStart = static_cast<uint8_t*>(malloc(mRows * mOriginalDataStepRow));
        mOriginalDataEnd = mOriginalDataStart + mRows * mOriginalDataStepRow;
        mData = mOriginalDataStart;   //作为创始者。
        mDataEnd = mOriginalDataEnd;  //作为创始者。
    }
    Mat::Mat(size_t rows, size_t cols, int type, void* data)
        : mFlag(MatFlag{ MatFlag::NormalMatrixSignature, MatFlag::ContinuityFlag, static_cast<int16_t>(type) }), mRows(rows),
          mCols(cols), mData(static_cast<uint8_t*>(data)), mOriginalDataRefCount(new SUSTechAtomicInt{ 1 }),
          mOriginalDataStepCol(mFlag.depthStep() * mFlag.channels()), mOriginalDataStepRow(mCols * mOriginalDataStepCol),
          mOriginalDataStart(mData), mOriginalDataEnd(mOriginalDataStart + mRows * mOriginalDataStepRow) {
        mDataEnd = mOriginalDataEnd;
        //小问题，如果opencv不保护外源数据，那么imread是怎么实现的呢？
    }

    Mat::Mat(const Mat& m)
        : mFlag(m.mFlag), mRows(m.mRows), mCols(m.mCols), mData(m.mData), mDataEnd(m.mDataEnd),
          mOriginalDataRefCount(m.mOriginalDataRefCount), mOriginalDataStepCol(m.mOriginalDataStepCol),
          mOriginalDataStepRow(m.mOriginalDataStepRow), mOriginalDataStart(m.mOriginalDataStart),
          mOriginalDataEnd(m.mOriginalDataEnd) {
        (*mOriginalDataRefCount)++;  //矩阵头信息逐成员拷贝，除了引用计数递增。
    }
    Mat::Mat(Mat&& m) SUSTech_NOEXCEPT : mFlag(m.mFlag),
                                         mRows(m.mRows),
                                         mCols(m.mCols),
                                         mData(m.mData),
                                         mDataEnd(m.mDataEnd),
                                         mOriginalDataRefCount(m.mOriginalDataRefCount),
                                         mOriginalDataStepCol(m.mOriginalDataStepCol),
                                         mOriginalDataStepRow(m.mOriginalDataStepRow),
                                         mOriginalDataStart(m.mOriginalDataStart),
                                         mOriginalDataEnd(m.mOriginalDataEnd) {
        //清零操作。
        /*m.mFlag = {0,0,0};
        m.mData = nullptr;
        m.mOriginalDataStepCol = 0;
        m.mOriginalDataStepRow = 0;
        m.mOriginalDataStart = nullptr;
        m.mOriginalDataEnd = nullptr;*/ //如果对不关键的成员清零，可能导致析构器不知道mFlag，从而不知道怎么析构(虽然这不需要释放内存)。
        //关键的清零只有这三个，给析构函数提供信息告诉它不用释放数据就可以了。
        m.mRows = 0;
        m.mCols = 0;
        m.mOriginalDataRefCount = nullptr;
    }
    Mat::Mat(Mat m, const Rect& roiRect) : Mat(std::move(m)) {
        SUSTech_Assert(roiRect.x + roiRect.width <= mCols && roiRect.y + roiRect.height <= mRows,
                       BadArgumentSize("Bad roi region. "));
        //还有别的特殊情况，矩形有0或者负数。因为是size_t，所以保证没有负数。0的时候，矩阵状态可以保证置空。
        if(!mFlag.isSubMatrix())
            mFlag.flipSubMatrixFlag();
        mRows = roiRect.height;  //矩形的长度是行数
        mCols = roiRect.width;   //矩形的宽度是列数
        if(roiRect.height > 1 && mFlag.isContinuous())
            mFlag.flipContinuityFlag();  //这个连续性对于赋值、切片、取元素乃至加减乘除至关重要。 如果是一行roi，一定是连续的。
        mData += roiRect.x * mOriginalDataStepCol;  // x是列标，和矩阵是反的。
        mData += roiRect.y * mOriginalDataStepRow;
        mDataEnd = mData + (mRows - 1) * mOriginalDataStepRow +
            mCols *
                mOriginalDataStepCol;  //这个-1很关键。rows减一，以便于定位到最后一行。但是cols不减一，表示合法位置的下一个，以吻合左闭右开语义。
        (*mOriginalDataRefCount)++;  //引用计数递增。子矩阵也是原矩阵的一份子，需要负起释放资源的责任。
    }
    Mat::Mat(Mat m, const int& channel) : Mat(std::move(m)) {
        SUSTech_Assert(0 <= channel && channel < mFlag.channels(), BadArgumentSize("Bad channel index."));
        if(!mFlag.isSubMatrix())
            mFlag.flipSubMatrixFlag();
        if(mFlag.isContinuous())
            mFlag.flipContinuityFlag();
        mData += channel * mFlag.depthStep();
        mDataEnd += channel * mFlag.depthStep();
        mFlag.mType = (int16_t)SUSTech_MAKE_TYPE(mFlag.depthOrder(), 1);
        // OriginalData的step和指针都不发生改变。但是计数加一。
        (*mOriginalDataRefCount)++;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-unconventional-assign-operator"
    Mat& Mat::operator=(const Scalar& s) {
        return this->operator=(static_cast<std::array<double, 4>>(s));
    }
#pragma clang diagnostic pop
    //如果实际channel是3，但是传入100，就不赋值后面的。如果实际channel是100，传入了3，那么不改变后面的值(而不是设置为0)
    template <typename Tp, size_t MaxChannels>
    Mat& Mat::operator=(const std::array<Tp, MaxChannels>& _s) {
        if(this->empty() || _s.empty())
            return *this;
        const int assignmentChannels = std::min(static_cast<int>(MaxChannels), mFlag.channels());
        //把s转换为我想要的类型.
        //        depth_to_type a{depth_to_type<mFlag.depthOrder()>::value}::convert(_s[0]); //失败的，因为模板不能传入非常数。
        //                const std::array<uint8_t, MaxChannels> newS = _s; //array没有提供转换函数，我的rect才有.
        //自己写一个模板函数，避免对Scalar多次类型转换。
        const auto depth = mFlag.depthOrder();
#define MatOperatorAssignmentSwitchGenerate(CASE)                                                            \
    {                                                                                                        \
        const auto convertedScalar = convert<decltype(depth_to_type<CASE>::value), double, MaxChannels>(_s); \
        if(mFlag.isContinuous()) {                                                                           \
            for(uint8_t* elementIt = mData; (elementIt < mDataEnd); elementIt += mOriginalDataStepCol) {     \
                auto concretePtr = reinterpret_cast<decltype(depth_to_type<CASE>::value)*>(elementIt);       \
                for(int i = 0; i < assignmentChannels; ++i) {                                                \
                    concretePtr[i] = convertedScalar[i];                                                     \
                }                                                                                            \
            }                                                                                                \
        } else {                                                                                             \
            for(size_t i = 0; i < mRows; ++i) {                                                              \
                uint8_t* rowIt = mData + i * mOriginalDataStepRow;                                           \
                for(size_t j = 0; j < mCols; ++j) {                                                          \
                    uint8_t* elementIt = rowIt + j * mOriginalDataStepCol;                                   \
                    auto concretePtr = reinterpret_cast<decltype(depth_to_type<CASE>::value)*>(elementIt);   \
                    for(int k = 0; k < assignmentChannels; ++k) {                                            \
                        concretePtr[k] = convertedScalar[k];                                                 \
                    }                                                                                        \
                }                                                                                            \
            }                                                                                                \
        }                                                                                                    \
        break;                                                                                               \
    }
        switch((SUSTech::depth_order)depth) {  // switch是无法避免的，switch是高效的。
            case SUSTech_8U:
                MatOperatorAssignmentSwitchGenerate(SUSTech_8U) case SUSTech_32S
                    : MatOperatorAssignmentSwitchGenerate(SUSTech_32S) case SUSTech_32F
                    : MatOperatorAssignmentSwitchGenerate(SUSTech_32F) case SUSTech_64F
                    : MatOperatorAssignmentSwitchGenerate(SUSTech_64F) default
                    : throw std::runtime_error("new type cast not supported!");
        }
        //                elementIt[i] = s[i];
        //                memcpy(elementIt[i*mFlag.depthOrder()], reinterpret_cast<size_t>newS[i], );
        return *this;
    }
    //    template Mat& Mat::operator=(const std::array<double, 4>& _s);
    //    template Mat& Mat::operator=(const std::array<float, 3>& _s);

    SUSTech_ALWAYS_INLINE bool Mat::empty() const {
        return mRows == 0 || mCols == 0;
    }

    //析构函数
    Mat::~Mat() {
        //引用减数
        release();
        //销毁自己的存在（系统自动）
    }
    void Mat::release() {
        if(empty() || mOriginalDataRefCount == nullptr)  // opencv就是通过没有计数，使得释放的时候不释放外源数据。
            return;
        if(((*mOriginalDataRefCount)--) == 1)
            destroy();
        mRows = mCols = 0;
        mData = nullptr;
        mDataEnd = nullptr;
        mOriginalDataRefCount = nullptr;  //为了保险，把自己拥有的地址清空。
        mOriginalDataStart = nullptr;
        mOriginalDataEnd = nullptr;
    }
    void Mat::destroy() {
        free(mOriginalDataStart);
        delete mOriginalDataRefCount;
    }
    //复制赋值(opencv语义，不是c++语义)
    Mat& Mat::operator=(const Mat& m) {
        if(this == &m)
            return *this;
        release();
        mFlag = m.mFlag;
        mRows = m.mRows;
        mCols = m.mCols;
        mData = m.mData;
        mDataEnd = m.mDataEnd;
        mOriginalDataRefCount = m.mOriginalDataRefCount;
        mOriginalDataStepCol = m.mOriginalDataStepCol;
        mOriginalDataStepRow = m.mOriginalDataStepRow;
        mOriginalDataStart = m.mOriginalDataStart;
        mOriginalDataEnd = m.mOriginalDataEnd;

        (*mOriginalDataRefCount)++;
        return *this;
    }
    //移动赋值
    Mat& Mat::operator=(Mat&& m) SUSTech_NOEXCEPT {
        if(this == &m)
            return *this;
        release();
        mFlag = m.mFlag;
        mRows = m.mRows;
        mCols = m.mCols;
        mData = m.mData;
        mDataEnd = m.mDataEnd;
        mOriginalDataRefCount = m.mOriginalDataRefCount;
        mOriginalDataStepCol = m.mOriginalDataStepCol;
        mOriginalDataStepRow = m.mOriginalDataStepRow;
        mOriginalDataStart = m.mOriginalDataStart;
        mOriginalDataEnd = m.mOriginalDataEnd;

        m.mRows = 0;
        m.mCols = 0;
        m.mOriginalDataRefCount = nullptr;
        return *this;
    }
    SUSTech_ALWAYS_INLINE VarScalar Mat::operator()(const Point& location) const {
        return this->operator()(location.x, location.y);
    }
    SUSTech_ALWAYS_INLINE VarScalar Mat::operator()(size_t rowIndex, size_t colIndex) const {
        SUSTech_Assert(rowIndex < mRows && colIndex < mCols, BadArgumentSize{ "element access out of range. " });
        VarScalar scalar(mFlag.channels());
        const uint8_t* elementLocation = mData + (rowIndex * mOriginalDataStepRow) + (colIndex * mOriginalDataStepCol);
        for(int i = 0; i < mFlag.channels(); i++) {
            const uint8_t* channelLocation = elementLocation + (i * mFlag.depthStep());
            //            scalar.push_back(doubleValueOf(channelLocation, mFlag.depthOrder()));
            scalar[i] = doubleValueOf(channelLocation, mFlag.depthOrder());
        }
        return scalar;
    }
    SUSTech_ALWAYS_INLINE Mat Mat::operator[](const Rect& roiRect) const {
        return { *this, roiRect };
    }
    SUSTech_ALWAYS_INLINE Mat Mat::operator[](const Point& location) const {
        return this->operator[]({ location.y, location.x, 1, 1 });
    }
    SUSTech_ALWAYS_INLINE Mat Mat::row(size_t rowIndex) const {
        return this->operator[]({ 0, rowIndex, mCols, 1 });  // row(rowsThenColumnsIndex)
    }
    SUSTech_ALWAYS_INLINE Mat Mat::col(size_t colIndex) const {
        return this->operator[]({ colIndex, 0, 1, mRows });  // col(rowsThenColumnsIndex)
    }
    //    SUSTech_ALWAYS_INLINE
    Mat Mat::channel(int channelIndex) const {
        return { *this, channelIndex };
    }
    //    SUSTech_ALWAYS_INLINE
    Mat Mat::operator[](size_t rowsThenColumnsIndex) const {
        if(mRows > 1)
            return row(rowsThenColumnsIndex);
        else
            return col(rowsThenColumnsIndex);
    }

    //调试函数
    std::ostream& operator<<(std::ostream& os, const Mat& mat) {
        os<<"Mat<"<<"depth="<<mat.mFlag.depthName()<<", "<<"channel="<<mat.mFlag.channels()<<", "<<mat.mRows<<"x"<<mat.mCols<<">{"<<std::endl;
        for(int i = 0; i < mat.mRows; ++i) {
            //            for(int j = 0; ; ++j) {
            //                os << mat(i, j);
            //                if(j < mat.mCols) break;//优雅地消除最后一个空格, 但是这个对熔合运算不友好。
            //                os << " ";
            //            }
            int j = 0;
            for(;j < mat.mCols-1; ++j) {
                os << mat(i, j)<< "\t";
            }
            os << mat(i, j) <<std::endl;
        }
        return os<<"};";
    }
    //对于roi，也可以从文件读入数据。所以这个函数的作用是改变已有mat的数据，而不是生成新的矩阵。
    std::istream& operator>>(std::istream& in, Mat& mat) {
        if(mat.empty())
            return in;
//        if(mat.mData==nullptr || mat.mOriginalDataRefCount==nullptr) //不是某个矩阵的roi，是刚创建的矩阵，没有申请内存
//            mat.create();
#define MatOperatorInsertionSwitchGenerate(CASE)                                                                   \
    {                                                                                                              \
        if(mat.mFlag.isContinuous()) {                                                                             \
            for(uint8_t* elementIt = mat.mData; elementIt < mat.mDataEnd; elementIt += mat.mOriginalDataStepCol) { \
                auto concretePtr = reinterpret_cast<decltype(depth_to_type<CASE>::value)*>(elementIt);             \
                for(int j = 0; j < mat.mFlag.channels(); ++j) {                                                    \
                    in >> *(concretePtr+j);                                                                          \
                }                                                                                                  \
            }                                                                                                      \
        } else {                                                                                                   \
            for(size_t i = 0; i < mat.mRows; ++i) {                                                                \
                uint8_t* rowIt = mat.mData + i * mat.mOriginalDataStepRow;                                         \
                for(size_t j = 0; j < mat.mCols; ++j) {                                                            \
                    uint8_t* elementIt = rowIt + j * mat.mOriginalDataStepCol;                                     \
                    auto concretePtr = reinterpret_cast<decltype(depth_to_type<CASE>::value)*>(elementIt);         \
                    for(int k = 0; k < mat.mFlag.channels(); ++k) {                                                \
                        in >> concretePtr[k];                                                                      \
                    }                                                                                              \
                }                                                                                                  \
            }                                                                                                      \
        }                                                                                                          \
        break;                                                                                                     \
    }
        switch((SUSTech::depth_order)mat.mFlag.depthOrder()) {
            case SUSTech_8U:
                MatOperatorInsertionSwitchGenerate(SUSTech_8U) case SUSTech_32S
                    : MatOperatorInsertionSwitchGenerate(SUSTech_32S) case SUSTech_32F
                    : MatOperatorInsertionSwitchGenerate(SUSTech_32F) case SUSTech_64F
                    : MatOperatorInsertionSwitchGenerate(SUSTech_64F) default
                    : throw std::runtime_error("new type cast not supported!");
        }
        return in;
    }
     Mat Mat::matRead(const std::string& file_name) {
        FILE* file = fopen(file_name.c_str(), "r");
        SUSTech_Assert(file != nullptr, std::ios_base::failure{ "Fatal Error: File not found.\nmatrix reading terminated." });
        size_t file_length = file_len(file);
        size_t max_matrix_length = (file_length + 1) / 2;  //根据文件大小，解不等式得到
        float* data = (float*)calloc(max_matrix_length, sizeof(float));
        int k = 0;
        size_t rowCount = 0, columnCount = 0;
        size_t columnCountMax = 0;  //千万不能设置为INT32_MIN！！！
        size_t buffer_max_size = max_matrix_length * 32;
        //一行的信息，极端情况下一行有2048个数，每个数很长很长，假设字面量最长为32
        char* buffer = (char*)calloc(buffer_max_size, sizeof(char));
        for(rowCount = 0; fgets(buffer, buffer_max_size, file) != nullptr; ++rowCount) {
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
        fclose(file);
        data = static_cast<float*>(realloc(data, rowCount * columnCountMax * sizeof(float)));
        return { rowCount, columnCount, SUSTech_32FC1, data };
    }

     //前提是同型。
    Mat& fast_matrix_element_by_element_linear_combinations_assignment(Mat& ths, const Mat& matrixA, const Mat& matrixB, float _alpha,
                                                                  float _beta) {
#define alpha_beta(CASE) \
        const auto alpha = static_cast<decltype(depth_to_type<CASE>::value)>(_alpha);\
        const auto beta = static_cast<decltype(depth_to_type<CASE>::value)>(_beta)
#define slow_matrix_element_by_element_linear_combinations_assignment_CASE(CASE) \
        for(int64_t i = 0; i < ths.mRows; ++i) {\
            for(size_t j = 0; j < ths.mCols; ++j) {\
                for(int k = 0; k < ths.mFlag.channels(); ++k) {\
                    (*GetElementPtr_l3_frd(i, j, k, CASE, ths)) = (*GetElementPtr_l3_frd(i, j, k, CASE, matrixA))*alpha + (*GetElementPtr_l3_frd(i, j, k, CASE, matrixB))*beta; \
                }\
            }\
        }
#define fast_matrix_element_by_element_linear_combinations_assignment_CASE(CASE) \
    for(int64_t i = 0; i < ths.mRows*ths.mCols*ths.mFlag.channels(); ++i) {\
        (*GetElementPtr_l1_frd(i, CASE, ths)) = (*GetElementPtr_l1_frd(i, CASE, matrixA))*alpha + (*GetElementPtr_l1_frd(i, CASE, matrixB))*beta;\
    }
        switch ((SUSTech::depth_order)ths.mFlag.depthOrder()) {
            case SUSTech_8U: {
                alpha_beta(SUSTech_8U);
                if(ths.mFlag.isContinuous()) {
#pragma omp parallel for
                    fast_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_8U)
                } else {
#pragma omp parallel for
                    slow_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_8U)
                }
                break;
            }
            case SUSTech_32S: {
                alpha_beta(SUSTech_32S);
                if(ths.mFlag.isContinuous()) {
#pragma omp parallel for
                    fast_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_32S)
                } else {
#pragma omp parallel for
                    slow_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_32S)
                }
                break;
            }
            case SUSTech_32F: {
                alpha_beta(SUSTech_32F);
                if(ths.mFlag.isContinuous()) {
#pragma omp parallel for
                    fast_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_32F)
                } else {
#pragma omp parallel for
                    slow_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_32F)
                }
                break;
            }
            case SUSTech_64F: {
                alpha_beta(SUSTech_64F);
                if(ths.mFlag.isContinuous()) {
#pragma omp parallel for
                    fast_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_64F)
                } else {
#pragma omp parallel for
                    slow_matrix_element_by_element_linear_combinations_assignment_CASE(SUSTech_64F)
                }
                break;
            }
        }
        return ths;
    }
    Mat& Mat::operator+=(const Mat& matB) {
        SUSTech_Assert(matB.mFlag==this->mFlag && matB.mRows==this->mRows && matB.mCols==this->mCols, BadArgumentSize("Incompatible matrix addition."));
        return fast_matrix_element_by_element_linear_combinations_assignment(*this, *this, matB, 1.f, 1.f);
    }
    Mat& Mat::operator-=(const Mat& matB) {
        SUSTech_Assert(matB.mFlag==this->mFlag && matB.mRows==this->mRows && matB.mCols==this->mCols, BadArgumentSize("Incompatible matrix subtraction."));
        return fast_matrix_element_by_element_linear_combinations_assignment(*this, *this, matB, 1.f, -1.f);
    }
    Mat Mat::operator+(const Mat& matB) {
        SUSTech_Assert(matB.mFlag==this->mFlag && matB.mRows==this->mRows && matB.mCols==this->mCols, BadArgumentSize("Incompatible matrix addition."));
        Mat matC{mRows, mCols, mFlag.mType, Scalar{0} };
        return fast_matrix_element_by_element_linear_combinations_assignment(matC, *this, matB, 1.f, 1.f);
    }

    Mat Mat::operator-(const Mat& matB) {
        SUSTech_Assert(matB.mFlag==this->mFlag && matB.mRows==this->mRows && matB.mCols==this->mCols, BadArgumentSize("Incompatible matrix subtraction."));
        Mat matC{mRows, mCols, mFlag.mType, Scalar{0} };
        return fast_matrix_element_by_element_linear_combinations_assignment(matC, *this, matB, 1.f, -1.f);
    }
    Mat Mat::matMul(const Mat& matB) {
        SUSTech_Assert(matB.mFlag==this->mFlag && this->mCols == matB.mRows , BadArgumentSize("Incompatible matrix multiplication."));
        Mat result {mRows, matB.mCols, mFlag.mType, Scalar{0}};
        switch ((SUSTech::depth_order)mFlag.depthOrder()) {
            case SUSTech_8U: {
#pragma omp parallel for
                for(int64_t i = 0; i < this->mRows; ++i) {
                    std::vector<decltype(depth_to_type<SUSTech_8U>::value)> temps(mFlag.channels());
                    for(size_t k = 0; k < this->mCols; ++k) {
#pragma omp simd
                        for(int l = 0; l < mFlag.channels(); ++l) {
                            temps[l] = *GetElementPtr_l3_frd(i, k, l,SUSTech_8U, (*this));
                        }
                        for(size_t j = 0; j < matB.mCols; ++j) {
#pragma omp simd
                            for(int l = 0; l < mFlag.channels(); ++l) {
                                (*GetElementPtr_l3_frd( i, j, l, SUSTech_8U, result)) += temps[l] * (*GetElementPtr_l3_frd( k, j, l, SUSTech_8U, matB));
                            }
                        }
                    }
                }
                break;
            }
            case SUSTech_32S: {
#pragma omp parallel for
                for(int64_t i = 0; i < this->mRows; ++i) {
                    std::vector<decltype(depth_to_type<SUSTech_32S>::value)> temps(mFlag.channels());
                    for(size_t k = 0; k < this->mCols; ++k) {
#pragma omp simd
                        for(int l = 0; l < mFlag.channels(); ++l) {
                            temps[l] = *GetElementPtr_l3_frd(i, k, l,SUSTech_32S, (*this));
                        }
                        for(size_t j = 0; j < matB.mCols; ++j) {
#pragma omp simd
                            for(int l = 0; l < mFlag.channels(); ++l) {
                                (*GetElementPtr_l3_frd( i, j, l, SUSTech_32S, result)) += temps[l] * (*GetElementPtr_l3_frd( k, j, l, SUSTech_32S, matB));
                            }
                        }
                    }
                }
                break;
            }
            case SUSTech_32F: {
#pragma omp parallel for
                for(int64_t i = 0; i < this->mRows; ++i) {
                    std::vector<decltype(depth_to_type<SUSTech_32F>::value)> temps(mFlag.channels());
                    for(size_t k = 0; k < this->mCols; ++k) {
#pragma omp simd
                        for(int l = 0; l < mFlag.channels(); ++l) {
                            temps[l] = *GetElementPtr_l3_frd(i, k, l,SUSTech_32F, (*this));
                        }
                        for(size_t j = 0; j < matB.mCols; ++j) {
#pragma omp simd
                            for(int l = 0; l < mFlag.channels(); ++l) {
                                (*GetElementPtr_l3_frd( i, j, l, SUSTech_32F, result)) += temps[l] * (*GetElementPtr_l3_frd( k, j, l, SUSTech_32F, matB));
                            }
                        }
                    }
                }
                break;
            }
            case SUSTech_64F: {
#pragma omp parallel for
                for(int64_t i = 0; i < this->mRows; ++i) {
                    std::vector<decltype(depth_to_type<SUSTech_64F>::value)> temps(mFlag.channels());
                    for(size_t k = 0; k < this->mCols; ++k) {
#pragma omp simd
                        for(int l = 0; l < mFlag.channels(); ++l) {
                            temps[l] = *GetElementPtr_l3_frd(i, k, l,SUSTech_64F, (*this));
                        }
                        for(size_t j = 0; j < matB.mCols; ++j) {
#pragma omp simd
                            for(int l = 0; l < mFlag.channels(); ++l) {
                                (*GetElementPtr_l3_frd( i, j, l, SUSTech_64F, result)) += temps[l] * (*GetElementPtr_l3_frd( k, j, l, SUSTech_64F, matB));
                            }
                        }
                    }
                }
                break;
            }
        }
        return result;
    }

}  // namespace SUSTech