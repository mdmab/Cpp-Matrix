#include "matrix_t.hh"
#include <cmath>

matrix_t::matrix_t(integer r, integer c) {
    if (r < 0 || c < 0) {
        dimension_error;
        stop;
    }

    row = r;
    column = c;

    matrix = new double* [row];

    for (integer i = 0; i < row; ++i) {
        matrix[i] = new double[column];
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] = 0;
        }
    }
}

matrix_t::matrix_t(const std::initializer_list<std::initializer_list<double>>& mat) {
    integer prev = -1;

    for (std::initializer_list<double> mat_row: mat) {
        if (prev != -1 && prev != mat_row.size()) {
            row_size_error;
            stop;
        }

        prev = mat_row.size();
    }

    row = mat.size();
    column = mat.begin()->size();

    matrix = new double* [row];

    for (integer i = 0; i < row; ++i) {
        matrix[i] = new double[column];
    }

    integer i = 0;

    for (std::initializer_list<double> mat_row: mat) {
        integer j = 0;

        for (double element: mat_row) {
            matrix[i][j] = element;
            ++j;
        }

        ++i;
    }
}

matrix_t::matrix_t(const matrix_t& mat) {
    row = mat.row_count();
    column = mat.column_count();

    matrix = new double* [row];

    for (integer i = 0; i < row; ++i) {
        matrix[i] = new double[column];
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] = mat.at(i, j);
        }
    }
}

matrix_t::~matrix_t() {
    for (integer i = 0; i < row; ++i) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

matrix_t matrix_t::operator=(const matrix_t& other) {
    if (this == &other) {
        return *this;
    }

    for (integer i = 0; i < row; ++i) {
        delete[] matrix[i];
    }

    delete[] matrix;

    row = other.row_count();
    column = other.column_count();

    matrix = new double* [row];

    for (integer i = 0; i < row; ++i) {
        matrix[i] = new double[column];
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] = other.at(i, j);
        }
    }

    return *this;
}

matrix_t matrix_t::operator+(const matrix_t& other) {
    if (row != other.row_count() || column != other.column_count()) {
        equality_error;
        stop;
    }

    matrix_t new_matrix(row, column);

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            new_matrix[i][j] = this->at(i, j) + other.at(i, j);
        }
    }

    return new_matrix;
}

matrix_t matrix_t::operator-(const matrix_t& other) {
    if (row != other.row_count() || column != other.column_count()) {
        equality_error;
        stop;
    }

    matrix_t new_matrix(row, column);

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            new_matrix[i][j] = this->at(i, j) - other.at(i, j);
        }
    }

    return new_matrix;
}

matrix_t matrix_t::operator*(const matrix_t& other) {
    if (column != other.row_count()) {
        std::cerr << err << "For matrix multiplication, the number of columns in the first matrix must be equal to the number of rows in the second column." << std::endl;
        stop;
    }

    matrix_t new_matrix(row, other.column_count());
    
    for (integer i = 0; i < new_matrix.row_count(); ++i) {
        for (integer j = 0; j < new_matrix.column_count(); ++j) {
            for (integer k = 0; k < column; ++k) {
                new_matrix[i][j] += this->at(i, k) * other.at(k, j);
            }
        }
    }

    return new_matrix;
}

matrix_t matrix_t::operator+=(const matrix_t& other) {
    if (row != other.row_count() || column != other.column_count()) {
        equality_error;
        stop;
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] += other.at(i, j);
        }
    }

    return *this;
}

matrix_t matrix_t::operator-=(const matrix_t& other) {
    if (row != other.row_count() || column != other.column_count()) {
        equality_error;
        stop;
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] -= other.at(i, j);
        }
    }

    return *this;
}

matrix_t matrix_t::operator*=(const matrix_t& other) {
    if (column != other.row_count()) {
        std::cerr << err << "For matrix multiplication, the number of columns in the first matrix must be equal to the number of rows in the second column." << std::endl;
        stop;
    }

    matrix_t new_matrix(row, other.column_count());
    
    for (integer i = 0; i < new_matrix.row_count(); ++i) {
        for (integer j = 0; j < new_matrix.column_count(); ++j) {
            for (integer k = 0; k < column; ++k) {
                new_matrix[i][j] += this->at(i, k) * other.at(k, j);
            }
        }
    }

    *this = new_matrix;
    return *this;
}

bool matrix_t::operator==(const matrix_t& other) const {
    if (row != other.row_count() || column != other.column_count()) {
        return false;
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            if (std::fabs(this->at(i, j) - other.at(i, j)) > eps) {
                return false;
            }
        }
    }

    return true;
}

double* matrix_t::operator[](integer r) {
    if (r < 0 || r >= row) {
        indexing_error;
        stop;
    }

    return matrix[r];
}

integer matrix_t::row_count() const {
    return row;
}

integer matrix_t::column_count() const {
    return column;
}

double matrix_t::at(integer i, integer j) const {
    if (i < 0 || j < 0 || i >= row || j >= column) {
        indexing_error;
        stop;
    }

    return matrix[i][j];
}

matrix_t matrix_t::scalar_mult(double scalar) {
    matrix_t new_matrix(row, column);

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            new_matrix[i][j] = this->at(i, j) * scalar;
        }
    }

    return new_matrix;
}

matrix_t matrix_t::scalar_div(double scalar) {
    matrix_t new_matrix(row, column);

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            new_matrix[i][j] = this->at(i, j) / scalar;
        }
    }

    return new_matrix;
}

matrix_t matrix_t::transpose() {
    matrix_t transpose_matrix(column, row);

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            transpose_matrix[j][i] = this->at(i, j);
        }
    }

    return transpose_matrix;
}

void matrix_t::make_identity_matrix() {
    if (row != column || row == 0) {
        identity_error;
        stop;
    }

    for (integer i = 0; i < row; ++i) {
        for (integer j = 0; j < column; ++j) {
            matrix[i][j] = (i == j ? 1: 0);
        }
    }
}