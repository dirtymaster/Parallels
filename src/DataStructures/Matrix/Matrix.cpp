#include "Matrix.h"

namespace s21 {

void S21Matrix::destroy_matrix() {
    if (_matrix) {
        for (int i = 0; i < _rows; i++) delete[] _matrix[i];
        delete[] _matrix;
        _matrix = nullptr;
    }
    _rows = 0;
    _cols = 0;
}

void S21Matrix::FillWithDigit(const double digit) {
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            _matrix[i][j] = digit;
        }
    }
}

void S21Matrix::allocate_matrix(int rows, int cols) {
    if (rows < 0 || cols < 0) throw "Matrix creation error: Rows and columns must be greater than zero";
    _rows = rows;
    _cols = cols;
    _matrix = new double *[_rows];
    for (int i = 0; i < rows; i++) _matrix[i] = new double[cols]();
}

void S21Matrix::copy_matrix_elements(const S21Matrix &other) {
    for (int i = 0; i < other._rows && i < _rows; i++)
        for (int j = 0; j < other._cols && j < _cols; j++) _matrix[i][j] = other._matrix[i][j];
}

S21Matrix::S21Matrix() {
    _rows = 0;
    _cols = 0;
    _matrix = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) { allocate_matrix(rows, cols); }

S21Matrix::S21Matrix(const S21Matrix &other) {
    allocate_matrix(other._rows, other._cols);
    copy_matrix_elements(other);
}

S21Matrix::S21Matrix(S21Matrix &&other) {
    if (this == &other) throw "Error: trying to move from the same matrix";
    allocate_matrix(other._rows, other._cols);
    copy_matrix_elements(other);
    other.destroy_matrix();
}

S21Matrix::~S21Matrix() { destroy_matrix(); }

bool S21Matrix::eq_matrix(const S21Matrix &other) const {
    bool result = true;
    if (_rows != other._rows || _cols != other._cols) result = false;
    for (int i = 0; i < _rows && result; i++)
        for (int j = 0; j < _cols && result; j++)
            if (_matrix[i][j] != other._matrix[i][j]) result = false;
    return result;
}

void S21Matrix::sum_matrix(const S21Matrix &other) {
    if (_rows != other._rows || _cols != other._cols)
        throw "Sum error: dimensions of the matrices must be the same";
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++) (*this)(i, j) += other._matrix[i][j];
}

void S21Matrix::sub_matrix(const S21Matrix &other) {
    if (_rows != other._rows || _cols != other._cols)
        throw "Sub error: dimensions of the matrices must be the same";
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++) _matrix[i][j] -= other._matrix[i][j];
}

void S21Matrix::mul_number(const double num) {
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++) _matrix[i][j] *= num;
}

void S21Matrix::mul_matrix(const S21Matrix &other) {
    if (_rows != other._cols || _cols != other._rows) {
        throw "Mult error: Number of rows of the first matrix"
          "must be equal to number of columns of the second matrix";
    }
    S21Matrix result_matrix(_rows, other._cols);
    for (int i = 0; i < result_matrix._rows; i++)
        for (int j = 0; j < result_matrix._cols; j++)
            for (int k = 0; k < _cols; k++)
                result_matrix._matrix[i][j] += _matrix[i][k] * other._matrix[k][j];
    (*this) = result_matrix;
}

S21Matrix S21Matrix::transpose() {
    S21Matrix result(_cols, _rows);
    for (int i = 0; i < result._rows; i++)
        for (int j = 0; j < result._cols; j++) result(i, j) = (*this)(j, i);
    return result;
}

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
    S21Matrix result(*this);
    result.sum_matrix(other);
    return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
    S21Matrix result(*this);
    result.sub_matrix(other);
    return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
    S21Matrix result(*this);
    result.mul_matrix(other);
    return result;
}

bool S21Matrix::operator==(const S21Matrix &other) const { return eq_matrix(other); }

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
    if (this != &other) {
        destroy_matrix();
        allocate_matrix(other._rows, other._cols);
        copy_matrix_elements(other);
    }
    return *this;
}

void S21Matrix::operator+=(const S21Matrix &other) { sum_matrix(other); }

void S21Matrix::operator-=(const S21Matrix &other) { sub_matrix(other); }

void S21Matrix::operator*=(const S21Matrix &other) { mul_matrix(other); }

double &S21Matrix::operator()(const int i, const int j) {
    return _matrix[i][j];
}

int S21Matrix::get_rows() { return _rows; }

int S21Matrix::get_cols() { return _cols; }

void S21Matrix::set_rows(int new_rows) {
    if (new_rows <= 0) throw "Set rows error: rows must be greater than 0";
    S21Matrix new_matrix(new_rows, _cols);
    new_matrix.copy_matrix_elements(*this);
    (*this) = new_matrix;
}

void S21Matrix::set_columns(int new_cols) {
    if (new_cols <= 0) throw "Set rows error: rows must be greater than 0";
    S21Matrix new_matrix(_rows, new_cols);
    new_matrix.copy_matrix_elements(*this);
    (*this) = new_matrix;
}

bool S21Matrix::is_empty() {
    return !get_rows() && !get_cols();
}

void S21Matrix::FillMatrixWithRandValues(s21::S21Matrix &m) {
    double n = 1;
    for (int i = 0; i < m.get_rows(); i++) {
        for (int j = 0; j < m.get_cols(); j++) {
            m(i, j) = n++;
        }
    }
}

}  // namespace s21
