//
// Created by 叶璨铭 on 2021/12/11.
//
#pragma once
#include "Matrix.h"

namespace SUSTech {
    // Exception handling
    inline MatrixError::MatrixError(const std::string& message) : SUSTechError("MatrixError: " + message) {}
    inline BadArgumentSize::BadArgumentSize(const std::string& message) : MatrixError("BadArgumentSize: " + message) {}
    inline BadCast::BadCast(const std::string& message) : MatrixError("BadCast: " + message) {}
    // Matrix Slice
    inline bool MatSlice::sliceEquals(const std::slice& lhs, const std::slice& rhs) {
        return lhs.size() == rhs.size() && lhs.start() == rhs.start() && lhs.stride() == rhs.stride();
    }
    inline bool MatSlice::sliceUnequals(const std::slice& lhs, const std::slice& rhs) {
        return lhs.size() != rhs.size() || lhs.start() != rhs.start() || lhs.stride() != rhs.stride();
    }
    inline bool MatSlice::operator==(const MatSlice& rhs) const {
        return sliceEquals(rowSlice, rhs.rowSlice) && sliceEquals(colSlice, rhs.colSlice);
    }
    inline bool MatSlice::operator!=(const MatSlice& rhs) const {
        return sliceUnequals(rowSlice, rhs.rowSlice) || sliceUnequals(colSlice, rhs.colSlice);
    }
    inline bool MatSlice::isZero() {
        return (*this) == zero();
    }
    inline MatSlice MatSlice::zero() {
        return MatSlice{ std::slice{ 0, 0, 0 }, std::slice{ 0, 0, 0 } };
    }
    inline size_t MatSlice::getRows() const {
        return rowSlice.size();
    }
    inline size_t MatSlice::getCols() const {
        return colSlice.size();
    }
    inline void MatSlice::setRows(size_t rows) {
        rowSlice = { rowSlice.start(), rows, rowSlice.stride() };
    }
    inline void MatSlice::setCols(size_t cols) {
        colSlice = { colSlice.start(), cols, colSlice.stride() };
    }

}  // namespace SUSTech
