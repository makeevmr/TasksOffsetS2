#ifndef MATRIXLAB_MATRIX_OPERATORS
#define MATRIXLAB_MATRIX_OPERATORS

#include "BigIntFractionMatrix.h"

class MatrixOperator {
    const unsigned char _operator;
    const BigIntFractionMatrix *const _start_position;

public:
    MatrixOperator(const unsigned char &matrix_operator, const BigIntFractionMatrix *const start_position)
        : _operator(matrix_operator), _start_position(start_position) {}
    unsigned char getOperator() const { return _operator; }

    const BigIntFractionMatrix *getStartPosition() const { return _start_position; }
};

#endif /* MATRIXLAB_MATRIX_OPERATORS */
