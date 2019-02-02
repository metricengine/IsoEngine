#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <iostream>
#include <vector>

namespace iso::math
{

// Row major matrix
template <typename T, int Rows, int Cols>
class Matrix
{
public:
    Matrix(T value = T{})
    {
        data.resize(Rows * Cols, value);
    }
    Matrix(std::array<T, Rows * Cols> values)
    {
        data.resize(values.size());
        for (int i = 0; i < values.size(); ++i) {
            data[i] = values[i];
        }
    }

    const std::vector<T> & getData()
    {
    }

    int rows() const
    {
        return Rows;
    }

    int cols() const
    {
        return Cols;
    }

    T & operator()(int row, int col)
    {
        return data[Rows * row + col];
    }

    const T & operator()(int row, int col) const
    {
        return data[Rows * row + col];
    }

    template <int Cols2>
    Matrix<T, Rows, Cols2> operator*(const Matrix<T, Cols, Cols2> & other)
    {
        Matrix<T, Rows, Cols2> res;
        for (int i = 0; i < Rows; ++i) {
            for (int j = 0; j < Cols2; ++j) {
                // Sum over other matrices
                T sum = T{};
                for (int k = 0; k < Cols; ++k) {
                    sum += data[Rows * i + k] * other(k, j);
                }
                res(i, j) = sum;
            }
        }
        return res;
    }

    Matrix<T, Rows, Cols> & operator*=(const Matrix<T, Cols, Cols> & other)
    {
        for (int i = 0; i < Rows; ++i) {
            for (int j = 0; j < Cols; ++j) {
                T sum = T{};
                for (int k = 0; k < Cols; ++k) {
                    sum += data[Rows * i + k] * other(k, j);
                }
                data[Rows * i + j] = sum;
            }
        }
        return *this;
    }

    static Matrix<T, Rows, Cols> Identity()
    {
        Matrix<T, Rows, Cols> I(0);
        for (int i = 0; i < std::min(Rows, Cols); ++i) {
            I(i, i) = 1;
        }
        return I;
    }

private:
    std::vector<T> data;
};

template <typename T, int Rows, int Cols>
std::ostream & operator<<(std::ostream & os, const iso::math::Matrix<float, Rows, Cols> & matrix)
{
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            os << matrix(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}

} // namespace iso::math

#endif