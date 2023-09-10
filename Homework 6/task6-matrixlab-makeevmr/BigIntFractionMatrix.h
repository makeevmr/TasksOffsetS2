#ifndef MATRIX_MATRIXLAB
#define MATRIX_MATRIXLAB

#include "BigIntegerFraction.h"
#include <vector>

struct MatrixSize {
    size_t rows;
    size_t columns;
};

struct RowsPair {
    size_t left_row;
    size_t right_row;
};

struct NumberLine;

class BigIntFractionMatrix {
    size_t _rows;
    size_t _columns;
    BigIntegerFraction **_frac_matrix;
    std::string _variable_name;
    void swapValue(BigIntFractionMatrix &other);
    void swapRows(const RowsPair &rows_pair);
    void divideRow(size_t row_to_divide, const BigIntegerFraction &factor);
    // rows_pair.left_row - factor * rows_pair.right_row
    void linearRowConversion(const RowsPair &rows_pair, const BigIntegerFraction &factor);
    void resize(const MatrixSize &matrix_size);

public:
    BigIntFractionMatrix();

    BigIntFractionMatrix(const MatrixSize &matrix_size);

    BigIntFractionMatrix(const BigIntegerFraction &fraction);

    BigIntFractionMatrix(const std::string &variable_name);

    BigIntFractionMatrix(const BigIntFractionMatrix &other);

    BigIntFractionMatrix(BigIntFractionMatrix &&other) noexcept;

    BigIntFractionMatrix &operator=(const BigIntFractionMatrix &other);

    BigIntFractionMatrix &operator=(BigIntFractionMatrix &&other) noexcept;

    BigIntFractionMatrix &operator+=(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &operator-=(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &elementWiseMultiplication(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &elementWiseDivision(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &operator*=(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &multiplyByScapar(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &operator/=(const BigIntFractionMatrix &right);

    BigIntFractionMatrix operator-() const;

    // change sing of all values in matrix
    BigIntFractionMatrix &reverseMatrix();

    void zeros(const MatrixSize &matrix_size);

    void ones(const MatrixSize &matrix_size);

    void eye(size_t size);

    void linspace(const NumberLine &number_line, const BigIntFractionMatrix &elements_amount);

    void vertcat(std::vector<BigIntFractionMatrix> &matrix_array);

    void horzcat(std::vector<BigIntFractionMatrix> &matrix_array);

    void transpose();

    BigIntFractionMatrix det();

    void inv();

    BigIntFractionMatrix sum() const;

    BigIntFractionMatrix prod() const;

    BigIntFractionMatrix &min(const BigIntFractionMatrix &right);

    BigIntFractionMatrix &max(const BigIntFractionMatrix &right);

    explicit operator std::string() const;

    std::string getVariableName() const;

    unsigned long int getScalarInt() const;

    bool isScalar() const;

    ~BigIntFractionMatrix();
};

BigIntFractionMatrix operator+(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right);

BigIntFractionMatrix operator-(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right);

BigIntFractionMatrix operator*(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right);

BigIntFractionMatrix operator/(const BigIntFractionMatrix &left, const BigIntFractionMatrix &right);

struct NumberLine {
    BigIntFractionMatrix left_border;
    BigIntFractionMatrix right_border;
};

#endif /* MATRIX_MATRIXLAB */
