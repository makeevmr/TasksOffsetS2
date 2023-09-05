#ifndef MATRIXLAB_BIGINTEGER_FRACTION
#define MATRIXLAB_BIGINTEGER_FRACTION

#include "BigInteger/biginteger.h"

struct StringFraction {
    std::string _str_numerator;
    std::string _std_denominator = "1";
};

struct IntFraction {
    const int numerator;
    const int denominator = 1;
};

struct BigIntegerPair {
    BigInteger _left;
    BigInteger _right;
};

class BigIntegerFraction {
    BigInteger _numerator;
    BigInteger _denominator;
    static BigInteger GCD(const BigIntegerPair &bint_pair);
    void reduceFraction();

public:
    BigIntegerFraction();

    BigIntegerFraction(const IntFraction &int_fraction);

    BigIntegerFraction(const StringFraction &str_fraction);

    BigIntegerFraction(const BigIntegerFraction &other);

    BigIntegerFraction(BigIntegerFraction &&other) noexcept;

    BigIntegerFraction &operator=(const BigIntegerFraction &other);

    BigIntegerFraction &operator=(BigIntegerFraction &&other) noexcept;

    BigIntegerFraction &operator+=(const BigIntegerFraction &right);

    BigIntegerFraction &operator-=(const BigIntegerFraction &right);

    BigIntegerFraction &operator*=(const BigIntegerFraction &right);

    BigIntegerFraction &operator/=(const BigIntegerFraction &right);

    explicit operator std::string() const;

    const BigInteger &getNumerator() const;

    bool is_negative() const;

    void changeSign();

    ~BigIntegerFraction();
};

BigIntegerFraction operator+(const BigIntegerFraction &left, const BigIntegerFraction &right);

BigIntegerFraction operator-(const BigIntegerFraction &left, const BigIntegerFraction &right);

BigIntegerFraction operator*(const BigIntegerFraction &left, const BigIntegerFraction &right);

BigIntegerFraction operator/(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator<(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator<=(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator>(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator>=(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator==(const BigIntegerFraction &left, const BigIntegerFraction &right);

bool operator!=(const BigIntegerFraction &left, const BigIntegerFraction &right);

#endif // MATRIXLAB_BIGINTEGER_FRACTION
