//
// Created by 叶璨铭 on 2021/11/26.
//
Mat::Mat() : Mat(0, 0, 0) {}
Mat::Mat(size_t rows, size_t cols, int type) : Mat(rows, cols, type, Scalar{}) {}
Mat::Mat(size_t rows, size_t cols, int type, const Scalar& s)
    : mFlag(MatFlag{ MatFlag::NormalMatrixSignature, MatFlag::ContinuityFlag, static_cast<int16_t>(type) }), mRows(rows),
      mCols(cols), mData(nullptr), mOriginalDataRefCount(nullptr),
      mOriginalDataStepCol(depth_step[mFlag.depth()] * mFlag.channels()), mOriginalDataStepRow(mCols * mOriginalDataStepCol),
      mOriginalDataStart(nullptr), mOriginalDataEnd(nullptr) {
    if(!empty())
        create();
    *this = static_cast<std::array<double, 4>>(s);
}