#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <cstddef>

template <typename T>
class Matrix;

template <typename T>
std::ostream& operator<<(std::ostream&, const Matrix<T>&);
template <typename T>
std::istream& operator>>(std::istream&, const Matrix<T>&);

template <typename T>
class Matrix {       
    public:
        Matrix(std::size_t, std::size_t);
        Matrix(std::size_t, std::size_t, const std::vector<T>&);
        Matrix(const Matrix<T>&);
        ~Matrix();
        Matrix<T>& operator=(const Matrix<T>&);

        // access operators
        // checks range
        inline T& operator()(std::size_t, std::size_t);
        inline T& operator()(std::size_t, std::size_t) const;
        // doesn't check range
        inline T* operator[](std::size_t);
        inline T* operator[](std::size_t) const;

        //operations
        Matrix<T>& operator+=(const Matrix<T>&);
        Matrix<T>& operator-=(const Matrix<T>&);
        Matrix<T>& operator*=(const Matrix<T>&);
        Matrix<T>& operator*=(const double&);
        Matrix<T>& operator/=(const double&);
        Matrix<T>  operator^(const int&);

        friend std::ostream& operator<<<>(std::ostream&, const Matrix<T>&);
        friend std::istream& operator>><>(std::istream&, const Matrix<T>&);

        // static methods
        static Matrix<T> createIdentity(std::size_t);

        // methods
        Matrix<T> transpose();
    private:
        std::size_t _rows, _cols;
        T** _matrix;

        Matrix<T> expHelper(const Matrix<T>&, int);
};

template <typename T>
Matrix<T> operator+(Matrix<T>, const Matrix<T>&);
template <typename T>
Matrix<T> operator-(Matrix<T>, const Matrix<T>&);
template <typename T>
Matrix<T> operator*(Matrix<T>, const Matrix<T>&);
template <typename T>
Matrix<T> operator*(Matrix<T>, const double&);
template <typename T>
Matrix<T> operator*(const double&, Matrix<T>);
template <typename T>
Matrix<T> operator/(Matrix<T>, const double&);

#endif