#include "matrix.hpp"

#include <stdexcept>

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols) : _rows(rows), _cols(cols) {
    _matrix = new T*[_rows];
    for (int i = 0; i < _rows; i++) {
        _matrix[i] = new T[_cols];
    }
};

template <typename T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const std::vector<T>& values) : _rows(rows), _cols(cols) {
    if (values.size() != rows * cols) {
        throw std::invalid_argument("Invalid vector size");
    }
    _matrix = new T*[rows];
    for (int i = 0; i < rows; i++) {
        _matrix[i] = new T[cols];
        for (int j = 0; j < cols; j++) {
            _matrix[i][j] = values.at((i * cols) + j);
        }
    }
};

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& m) : _rows(m._rows), _cols(m._cols) {
    _matrix = new T*[_rows];
    for (int i = 0; i < _rows; ++i) {
        _matrix[i] = new T[_cols];
        for (int j = 0; j < _cols; ++j) {
            _matrix[i][j] = m[i][j];
        }
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < this->_rows; i++) {
        delete[] _matrix[i];
    }
    delete[] _matrix;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m) {
    if (this == &m) {
        return *this;
    }
    if (_rows != m._rows || _cols != m._cols) {
        for (int i = 0; i < _rows; ++i) {
            delete[] _matrix[i];
        }
        delete[] _matrix;
        _rows = m._rows;
        _cols = m._cols;
        _matrix = new T*[_rows];
        for (int i = 0; i < _rows; ++i) {
            _matrix[i] = new T[_cols];
            for (int j = 0; j < _cols; ++j) {
                _matrix[i][j] = m[i][j];
            }
        }
        return *this;
    }
    for(int i = 0; i < _rows; ++i) {
        for(int j = 0; j < _cols; ++j) {
            _matrix[i][j] = m[i][j];
        }
    }
    return *this;
}

// ACCESS OPERATORS
template <typename T>
T& Matrix<T>::operator()(std::size_t i, std::size_t j) {
    if (i >= _rows || i < 0 || j >= _cols || j < 0) {
        throw std::out_of_range("Invalid index");
    }
    return _matrix[i][j];
}
template <typename T>
T& Matrix<T>::operator()(std::size_t i, std::size_t j) const {
    if (i >= _rows || i < 0 || j >= _cols || j < 0) {
        throw std::out_of_range("Invalid index");
    }
    return _matrix[i][j];
}
template <typename T>
T* Matrix<T>::operator[](std::size_t i) {
    return _matrix[i];
}
template <typename T>
T* Matrix<T>::operator[](std::size_t i) const {
    return _matrix[i];
}

// OPERATORS
template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs) {
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::out_of_range("Matrix must be same size");
    }

    for(int i = 0; i < _rows; ++i) {
        for(int j = 0; j < _cols; ++j) {
            _matrix[i][j] += rhs[i][j];
        }
    }
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs) {
    if (_rows != rhs._rows || _cols != rhs._cols) {
        throw std::out_of_range("Matrix must be same size");
    }

    for(int i = 0; i < _rows; ++i) {
        for(int j = 0; j < _cols; ++j) {
            _matrix[i][j] -= rhs[i][j];
        }
    }
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs) {
    if (_cols != rhs._rows) {
        throw std::out_of_range("Dimensional error");
    }

    Matrix<T> temp(_rows, rhs._cols);
    for (int i = 0; i < temp._rows; ++i) {
        for (int j = 0; j < temp._cols; ++j) {
            int sum = 0;
            for (int k = 0; k < _cols; ++k) {
                sum += (_matrix[i][k] * rhs[k][j]);
            }
            temp[i][j] = sum;
        }
    }
    return (*this = temp);
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const double& rhs) {
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            _matrix[i][j] *= rhs;
        }
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(const double& rhs) {
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            _matrix[i][j] /= rhs;
        }
    }
    return *this;
}

template <typename T>
Matrix<T>  Matrix<T>::operator^(const int& i) {
    Matrix<T> temp(*this);
    return expHelper(temp, i);
}

// PRIVATE METHOD
template <typename T>
Matrix<T> Matrix<T>::expHelper(const Matrix<T>& m, int i) {
    if (i == 0) {
        return createIdentity(m._rows);
    } else if (i == 1) {
        return m;
    } else if (i % 2 == 0) {
        return expHelper(m * m, i/2);
    } else {
        return m * expHelper(m * m, (i-1)/2);
    }
}

// NON-MEMBER FUNCTIONS
template <typename T>
Matrix<T> operator+(Matrix<T> lhs, const Matrix<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Matrix<T> operator-(Matrix<T> lhs, const Matrix<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <typename T>
Matrix<T> operator*(Matrix<T> lhs, const Matrix<T>& rhs) {
    lhs *= rhs;
    return lhs;
}

template <typename T>
Matrix<T> operator*(Matrix<T> lhs, const double& rhs) {
    lhs *= rhs;
    return lhs;
}

template <typename T>
Matrix<T> operator*(const double& lhs, Matrix<T> rhs) {
    return rhs * lhs;
}

template <typename T>
Matrix<T> operator/(Matrix<T> lhs, const double& rhs) {
    lhs /= rhs;
    return lhs;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
    for (int i = 0; i < m._rows; ++i) {
        for (int j = 0; j < m._cols; ++j)
            os << m[i][j] << " ";
        os << std::endl;
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, const Matrix<T>& m) {
    for (int i = 0; i < m._rows; ++i) {
        for (int j = 0; j < m._cols; ++j)
            is >> m[i][j];
    }
    return is;
}

// STATIC METHODS
template <typename T>
Matrix<T> Matrix<T>::createIdentity(std::size_t size) {
    Matrix<T> temp(size, size);
    for(int i = 0; i < temp._rows; ++i) {
        for(int j = 0; j < temp._cols; ++j) {
            if (i == j) {
                temp[i][j] = 1;
            } else {
                temp[i][j] = 0;
            }
        }
    }
    return temp;
}

// METHODS
template <typename T>
Matrix<T> Matrix<T>::transpose() {
    Matrix<T> temp(_cols, _rows);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            temp[j][i] = _matrix[i][j];
        }
    }
    return temp;
}
