#include "BigIntFractionMatrix.h"

void BigIntFractionMatrix::swapValue(BigIntFractionMatrix &other) {
    std::swap(_rows, other._rows);
    std::swap(_columns, other._columns);
    std::swap(_frac_matrix, other._frac_matrix);
    std::swap(_variable_name, other._variable_name);
}

void BigIntFractionMatrix::swapRows(const RowsPair &rows_pair) {
    for (size_t j = 0; j < _columns; ++j) {
        std::swap(_frac_matrix[rows_pair.left_row][j], _frac_matrix[rows_pair.right_row][j]);
    }
}

void BigIntFractionMatrix::divideRow(const size_t row_to_divide, const BigIntegerFraction &factor) {
    for (size_t j = 0; j < _columns; ++j) {
        _frac_matrix[row_to_divide][j] /= factor;
    }
}

// rows_pair.left_row - factor * rows_pair.right_row
void BigIntFractionMatrix::linearRowConversion(const RowsPair &rows_pair, const BigIntegerFraction &factor) {
    for (size_t j = 0; j < _columns; ++j) {
        _frac_matrix[rows_pair.left_row][j] -= _frac_matrix[rows_pair.right_row][j] * factor;
    }
}

void BigIntFractionMatrix::resize(const MatrixSize &matrix_size) {
    for (size_t i = 0; i < _rows; ++i) {
        delete[] _frac_matrix[i];
    }
    delete[] _frac_matrix;
    _rows = matrix_size.rows;
    _columns = matrix_size.columns;
    _frac_matrix = new BigIntegerFraction *[_rows];
    for (size_t i = 0; i < _rows; ++i) {
        _frac_matrix[i] = new BigIntegerFraction[_columns];
    }
}

BigIntFractionMatrix::BigIntFractionMatrix() : _rows(0), _columns(0), _frac_matrix(nullptr) {}

BigIntFractionMatrix::BigIntFractionMatrix(const MatrixSize &matrix_size)
    : _rows(matrix_size.rows), _columns(matrix_size.columns), _frac_matrix(new BigIntegerFraction *[_rows]) {
    for (size_t i = 0; i < _rows; ++i) {
        _frac_matrix[i] = new BigIntegerFraction[_columns];
    }
}

BigIntFractionMatrix::BigIntFractionMatrix(const std::string &variable_name)
    : _rows(0), _columns(0), _frac_matrix(nullptr) {
    _variable_name = variable_name;
}

BigIntFractionMatrix::BigIntFractionMatrix(const BigIntFractionMatrix &other)
    : _rows(other._rows), _columns(other._columns), _frac_matrix(new BigIntegerFraction *[_rows]),
      _variable_name(other._variable_name) {
    for (size_t i = 0; i < _rows; ++i) {
        _frac_matrix[i] = new BigIntegerFraction[_columns];
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j] = other._frac_matrix[i][j];
        }
    }
}

BigIntFractionMatrix::BigIntFractionMatrix(BigIntFractionMatrix &&other) noexcept
    : _rows(0), _columns(0), _frac_matrix(nullptr) {
    this->swapValue(other);
}

BigIntFractionMatrix &BigIntFractionMatrix::operator=(const BigIntFractionMatrix &other) {
    if (this != &other) {
        for (size_t i = 0; i < _rows; ++i) {
            delete[] _frac_matrix[i];
        }
        delete[] _frac_matrix;
        _rows = other._rows;
        _columns = other._columns;
        _frac_matrix = new BigIntegerFraction *[_rows];
        for (size_t i = 0; i < _rows; ++i) {
            _frac_matrix[i] = new BigIntegerFraction[_columns];
            for (size_t j = 0; j < _columns; ++j) {
                _frac_matrix[i][j] = other._frac_matrix[i][j];
            }
        }
        _variable_name = other._variable_name;
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::operator=(BigIntFractionMatrix &&other) noexcept {
    if (this != &other) {
        this->swapValue(other);
    }
    return *this;
}

void BigIntFractionMatrix::fillScalar(BigIntegerFraction &&big_int_frac) {
    _frac_matrix[0][0] = std::move(big_int_frac);
}

BigIntFractionMatrix &BigIntFractionMatrix::operator+=(const BigIntFractionMatrix &right) {
    if ((_rows == right._rows) && (_columns == right._columns)) {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j) {
                _frac_matrix[i][j] += right._frac_matrix[i][j];
            }
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::operator-=(const BigIntFractionMatrix &right) {
    if ((_rows == right._rows) && (_columns == right._columns)) {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j) {
                _frac_matrix[i][j] -= right._frac_matrix[i][j];
            }
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::elementWiseMultiplication(const BigIntFractionMatrix &right) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j] *= right._frac_matrix[i][j];
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::elementWiseDivision(const BigIntFractionMatrix &right) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j] /= right._frac_matrix[i][j];
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::operator*=(const BigIntFractionMatrix &right) {
    if (_columns == right._rows) {
        BigIntegerFraction **tmp_matrix = new BigIntegerFraction *[_rows];
        for (size_t i = 0; i < _rows; ++i) {
            tmp_matrix[i] = new BigIntegerFraction[right._columns];
        }
        for (size_t j = 0; j < right._columns; ++j) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t k = 0; k < _columns; ++k) {
                    tmp_matrix[i][j] += _frac_matrix[i][k] * right._frac_matrix[k][j];
                }
            }
        }
        for (size_t i = 0; i < _rows; ++i) {
            delete[] _frac_matrix[i];
        }
        delete[] _frac_matrix;
        _columns = right._columns;
        _frac_matrix = tmp_matrix;
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::multiplyByScapar(const BigIntFractionMatrix &right) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j] *= right._frac_matrix[0][0];
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::operator/=(const BigIntFractionMatrix &right) {
    BigIntFractionMatrix right_inv = right;
    right_inv.inv();
    *this *= right_inv;
    return *this;
}

BigIntFractionMatrix BigIntFractionMatrix::operator-() const {
    BigIntFractionMatrix opposite_matrix(*this);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            opposite_matrix._frac_matrix[i][j].changeSign();
        }
    }
    return opposite_matrix;
}

// change sing of all values in matrix_array
BigIntFractionMatrix &BigIntFractionMatrix::reverseMatrix() {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j].changeSign();
        }
    }
    return *this;
}

void BigIntFractionMatrix::zeros(const MatrixSize &matrix_size) { this->resize(matrix_size); }

void BigIntFractionMatrix::ones(const MatrixSize &matrix_size) {
    this->resize(matrix_size);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            _frac_matrix[i][j] = BigIntegerFraction(IntFraction{1, 1});
        }
    }
}

void BigIntFractionMatrix::eye(size_t size) {
    this->resize(MatrixSize{size, size});
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            if (i == j) {
                _frac_matrix[i][j] = BigIntegerFraction(IntFraction{1, 1});
            }
        }
    }
}

void BigIntFractionMatrix::linspace(const NumberLine &number_line, const BigIntFractionMatrix &elements_amount) {
    this->resize(MatrixSize{1, elements_amount._frac_matrix[0][0].getNumerator().get_data()[0]});
    BigIntFractionMatrix current_element = number_line.left_border;
    BigIntFractionMatrix scalar_one_matrix;
    scalar_one_matrix.eye(1);
    BigIntFractionMatrix step =
        (number_line.right_border - number_line.left_border) / (elements_amount - scalar_one_matrix);
    for (size_t j = 0; j < _columns; ++j) {
        _frac_matrix[0][j] = current_element._frac_matrix[0][0];
        current_element += step;
    }
}

void BigIntFractionMatrix::vertcat(std::vector<BigIntFractionMatrix> &matrix_array) {
    size_t total_rows = 0;
    for (size_t i = 0; i < matrix_array.size(); ++i) {
        total_rows += matrix_array[i]._rows;
    }
    this->resize(MatrixSize{total_rows, matrix_array[0]._columns});
    size_t this_row_index = 0;
    size_t this_column_index = 0;
    for (int i = static_cast<int>(matrix_array.size() - 1); i >= 0; --i) {
        for (size_t other_row_index = 0; other_row_index < matrix_array[i]._rows; ++other_row_index) {
            for (size_t other_column_index = 0; other_column_index < matrix_array[i]._columns; ++other_column_index) {
                if (this_column_index == _columns) {
                    this_column_index = 0;
                    ++this_row_index;
                }
                _frac_matrix[this_row_index][this_column_index] =
                    std::move(matrix_array[i]._frac_matrix[other_row_index][other_column_index]);
                ++this_column_index;
            }
        }
    }
}

void BigIntFractionMatrix::horzcat(std::vector<BigIntFractionMatrix> &matrix_array) {
    size_t total_columns = 0;
    for (size_t i = 0; i < matrix_array.size(); ++i) {
        total_columns += matrix_array[i]._columns;
    }
    this->resize(MatrixSize{matrix_array[0]._rows, total_columns});
    size_t this_row_index = 0;
    size_t this_column_index = 0;
    for (int i = static_cast<int>(matrix_array.size() - 1); i >= 0; --i) {
        for (size_t other_column_index = 0; other_column_index < matrix_array[i]._columns; ++other_column_index) {
            for (size_t other_row_index = 0; other_row_index < matrix_array[i]._rows; ++other_row_index) {
                if (this_row_index == _rows) {
                    this_row_index = 0;
                    ++this_column_index;
                }
                _frac_matrix[this_row_index][this_column_index] =
                    std::move(matrix_array[i]._frac_matrix[other_row_index][other_column_index]);
                ++this_row_index;
            }
        }
    }
}

void BigIntFractionMatrix::transpose() {
    BigIntFractionMatrix transposed_matrix(MatrixSize{_columns, _rows});
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            transposed_matrix._frac_matrix[j][i] = _frac_matrix[i][j];
        }
    }
    this->swapValue(transposed_matrix);
}

BigIntFractionMatrix BigIntFractionMatrix::det() {
    BigIntFractionMatrix result_matrix(MatrixSize{1, 1});
    BigIntegerFraction frac_det(IntFraction{1, 1});
    size_t current_row = 0;
    while (current_row < _rows) {
        size_t current_column = current_row;
        size_t row_to_swap = current_row;
        while ((row_to_swap < _rows) && (_frac_matrix[row_to_swap][current_column].getNumerator() == BigInteger(0))) {
            ++row_to_swap;
        }
        if (row_to_swap != current_row) {
            if (row_to_swap == _rows) {
                frac_det = BigIntegerFraction(IntFraction{0, 1});
                break;
            }
            this->swapRows(RowsPair{current_row, row_to_swap});
            if ((row_to_swap - current_row) % 2 > 0) {
                frac_det.changeSign();
            }
        }
        if (_frac_matrix[current_row][current_column] != BigIntegerFraction{IntFraction{1, 1}}) {
            BigIntegerFraction factor = _frac_matrix[current_row][current_column];
            this->divideRow(current_row, factor);
            frac_det *= factor;
        }
        for (size_t linear_conversion_row = current_row + 1; linear_conversion_row < _rows; ++linear_conversion_row) {
            BigIntegerFraction factor = _frac_matrix[linear_conversion_row][current_column];
            if (factor.getNumerator() != BigInteger(0)) {
                this->linearRowConversion(RowsPair{linear_conversion_row, current_row}, factor);
            }
        }
        ++current_row;
    }
    result_matrix.fillScalar(std::move(frac_det));
    return result_matrix;
}

void BigIntFractionMatrix::inv() {
    BigIntFractionMatrix inv_matrix;
    inv_matrix.eye(_rows);
    size_t current_row = 0;
    while (current_row < _rows) {
        size_t current_column = current_row;
        size_t row_to_swap = current_row;
        while ((row_to_swap < _rows) && (_frac_matrix[row_to_swap][current_column].getNumerator() == BigInteger(0))) {
            ++row_to_swap;
        }
        if (row_to_swap != current_row) {
            this->swapRows(RowsPair{current_row, row_to_swap});
            inv_matrix.swapRows(RowsPair{current_row, row_to_swap});
        }
        if (_frac_matrix[current_row][current_column] != BigIntegerFraction{IntFraction{1, 1}}) {
            BigIntegerFraction factor = _frac_matrix[current_row][current_column];
            this->divideRow(current_row, factor);
            inv_matrix.divideRow(current_row, factor);
        }
        for (size_t linear_conversion_row = current_row + 1; linear_conversion_row < _rows; ++linear_conversion_row) {
            BigIntegerFraction factor = _frac_matrix[linear_conversion_row][current_column];
            if (factor.getNumerator() != BigInteger(0)) {
                this->linearRowConversion(RowsPair{linear_conversion_row, current_row}, factor);
                inv_matrix.linearRowConversion(RowsPair{linear_conversion_row, current_row}, factor);
            }
        }
        ++current_row;
    }
    --current_row;
    while (current_row != 0) {
        size_t current_column = current_row;
        for (size_t linear_conversion_row = 0; linear_conversion_row < current_row; ++linear_conversion_row) {
            BigIntegerFraction factor = _frac_matrix[linear_conversion_row][current_column];
            if (factor.getNumerator() != BigInteger(0)) {
                this->linearRowConversion(RowsPair{linear_conversion_row, current_row}, factor);
                inv_matrix.linearRowConversion(RowsPair{linear_conversion_row, current_row}, factor);
            }
        }
        --current_row;
    }
    std::swap(_frac_matrix, inv_matrix._frac_matrix);
}

BigIntFractionMatrix BigIntFractionMatrix::sum() const {
    BigIntFractionMatrix result_matrix(MatrixSize{1, 1});
    BigIntegerFraction frac_sum;
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            frac_sum += _frac_matrix[i][j];
        }
    }
    result_matrix.fillScalar(std::move(frac_sum));
    return result_matrix;
}

BigIntFractionMatrix BigIntFractionMatrix::prod() const {
    BigIntFractionMatrix result_matrix(MatrixSize{1, 1});
    BigIntegerFraction frac_prod(IntFraction{1, 1});
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            frac_prod *= _frac_matrix[i][j];
        }
    }
    result_matrix.fillScalar(std::move(frac_prod));
    return result_matrix;
}

BigIntFractionMatrix &BigIntFractionMatrix::min(const BigIntFractionMatrix &right) {
    if ((_rows == right._rows) && (_columns == right._columns)) {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j) {
                if (_frac_matrix[i][j] > right._frac_matrix[i][j]) {
                    _frac_matrix[i][j] = right._frac_matrix[i][j];
                }
            }
        }
    }
    return *this;
}

BigIntFractionMatrix &BigIntFractionMatrix::max(const BigIntFractionMatrix &right) {
    if ((_rows == right._rows) && (_columns == right._columns)) {
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _columns; ++j) {
                if (_frac_matrix[i][j] < right._frac_matrix[i][j]) {
                    _frac_matrix[i][j] = right._frac_matrix[i][j];
                }
            }
        }
    }
    return *this;
}

BigIntFractionMatrix::operator std::string() const {
    std::string result;
    bool is_scalar = true;
    if (_rows > 1 || _columns > 1) {
        is_scalar = false;
        result += "[\n";
    }
    std::string **str_fractios_matrix = new std::string *[_rows];
    for (size_t i = 0; i < _rows; ++i) {
        str_fractios_matrix[i] = new std::string[_columns];
    }
    size_t *max_column_length_array = new size_t[_columns];
    for (size_t j = 0; j < _columns; ++j) {
        max_column_length_array[j] = 0;
    }
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            str_fractios_matrix[i][j] = std::string(_frac_matrix[i][j]);
            if (max_column_length_array[j] < str_fractios_matrix[i][j].length()) {
                max_column_length_array[j] = str_fractios_matrix[i][j].length();
            }
        }
    }
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _columns; ++j) {
            if (j > 0) {
                result += " ";
            } else if (i > 0) {
                result += "\n";
            }
            result += std::string(max_column_length_array[j] - str_fractios_matrix[i][j].length(), ' ');
            result += std::string(_frac_matrix[i][j]);
        }
    }
    if (!is_scalar) {
        result += "\n]";
    }
    for (size_t i = 0; i < _rows; ++i) {
        delete[] str_fractios_matrix[i];
    }
    delete[] str_fractios_matrix;
    delete[] max_column_length_array;
    return result;
}

BigIntFractionMatrix::~BigIntFractionMatrix() {
    for (size_t i = 0; i < _rows; ++i) {
        delete[] _frac_matrix[i];
    }
    delete[] _frac_matrix;
}

std::string BigIntFractionMatrix::getVariableName() const { return _variable_name; }

BigIntFractionMatrix operator+(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right) {
    BigIntFractionMatrix result = left;
    result += right;
    return result;
}

unsigned long int BigIntFractionMatrix::getScalarInt() const { return _frac_matrix[0][0].getNumerator().get_data()[0]; }

BigIntFractionMatrix operator-(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right) {
    BigIntFractionMatrix result = left;
    result -= right;
    return result;
}

bool BigIntFractionMatrix::isScalar() const { return (_rows == 1) && (_columns == 1); }

BigIntFractionMatrix operator*(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right) {
    BigIntFractionMatrix result = left;
    result *= right;
    return result;
}

BigIntFractionMatrix operator/(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right) {
    BigIntFractionMatrix result = left;
    result /= right;
    return result;
}
