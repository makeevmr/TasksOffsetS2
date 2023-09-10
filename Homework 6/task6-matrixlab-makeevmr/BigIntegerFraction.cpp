#include "BigIntegerFraction.h"

BigInteger BigIntegerFraction::GCD(const BigIntegerPair &bint_pair) {
    BigInteger remainder = bint_pair._left % bint_pair._right;
    if (remainder != 0) {
        return GCD(BigIntegerPair{bint_pair._right, remainder});
    }
    return bint_pair._right;
}

void BigIntegerFraction::reduceFraction() {
    bool is_negative = _numerator.is_negative();
    if (is_negative) {
        _numerator.changeSign();
    }
    BigInteger common_divisor = GCD(BigIntegerPair{_numerator, _denominator});
    if (common_divisor != 1) {
        _numerator /= common_divisor;
        _denominator /= common_divisor;
    }
    if (is_negative) {
        _numerator.changeSign();
    }
}

BigIntegerFraction::BigIntegerFraction() : _numerator(0), _denominator(1) {}

BigIntegerFraction::BigIntegerFraction(const IntFraction &int_fraction)
    : _numerator(int_fraction.numerator), _denominator(int_fraction.denominator) {
    this->reduceFraction();
}

BigIntegerFraction::BigIntegerFraction(const StringFraction &str_fraction)
    : _numerator(str_fraction._str_numerator), _denominator(str_fraction._std_denominator) {
    this->reduceFraction();
}

BigIntegerFraction::BigIntegerFraction(const BigIntegerFraction &other)
    : _numerator(other._numerator), _denominator(other._denominator) {}

BigIntegerFraction::BigIntegerFraction(BigIntegerFraction &&other) noexcept
    : _numerator(std::move(other._numerator)), _denominator(std::move(other._denominator)) {}

BigIntegerFraction &BigIntegerFraction::operator=(const BigIntegerFraction &other) {
    if (this != &other) {
        _numerator = other._numerator;
        _denominator = other._denominator;
    }
    return *this;
}

BigIntegerFraction &BigIntegerFraction::operator=(BigIntegerFraction &&other) noexcept {
    if (this != &other) {
        _numerator = std::move(other._numerator);
        _denominator = std::move(other._denominator);
    }
    return *this;
}

BigIntegerFraction &BigIntegerFraction::operator+=(const BigIntegerFraction &right) {
    BigInteger den_gcd = GCD(BigIntegerPair{_denominator, right._denominator});
    BigInteger left_multiplier = right._denominator / den_gcd;
    _numerator = _numerator * (left_multiplier) + right._numerator * (_denominator / den_gcd);
    _denominator *= left_multiplier;
    this->reduceFraction();
    return *this;
}

BigIntegerFraction &BigIntegerFraction::operator-=(const BigIntegerFraction &right) {
    BigIntegerFraction right_copy(right);
    right_copy._numerator.changeSign();
    return *this += right_copy;
}

BigIntegerFraction &BigIntegerFraction::operator*=(const BigIntegerFraction &right) {
    _numerator *= right._numerator;
    _denominator *= right._denominator;
    this->reduceFraction();
    return *this;
}

BigIntegerFraction &BigIntegerFraction::operator/=(const BigIntegerFraction &right) {
    return *this *= BigIntegerFraction(StringFraction{to_string(right._denominator), to_string(right._numerator)});
}

BigIntegerFraction::operator std::string() const {
    if (_denominator == 1) {
        return to_string(_numerator);
    }
    return to_string(_numerator) + "/" + to_string(_denominator);
}

const BigInteger &BigIntegerFraction::getNumerator() const { return _numerator; }

bool BigIntegerFraction::is_negative() const { return _numerator.is_negative(); }

void BigIntegerFraction::changeSign() { _numerator.changeSign(); }

BigIntegerFraction::~BigIntegerFraction() {
    _numerator.BigInteger::~BigInteger();
    _denominator.BigInteger::~BigInteger();
}

BigIntegerFraction operator+(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    BigIntegerFraction result = left;
    result += right;
    return result;
}

BigIntegerFraction operator-(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    BigIntegerFraction result = left;
    result -= right;
    return result;
}

BigIntegerFraction operator*(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    BigIntegerFraction result = left;
    result *= right;
    return result;
}

BigIntegerFraction operator/(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    BigIntegerFraction result = left;
    result /= right;
    return result;
}

bool operator<(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    if (left.is_negative() && !right.is_negative()) {
        return true;
    }
    if (!left.is_negative() && right.is_negative()) {
        return false;
    }
    BigIntegerFraction result = std::move(left - right);
    return result.is_negative();
}

bool operator<=(const BigIntegerFraction &left, const BigIntegerFraction &right) { return !(right < left); }

bool operator>(const BigIntegerFraction &left, const BigIntegerFraction &right) { return right < left; }

bool operator>=(const BigIntegerFraction &left, const BigIntegerFraction &right) { return !(left < right); }

bool operator==(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    return !(left < right) && !(right < left);
}

bool operator!=(const BigIntegerFraction &left, const BigIntegerFraction &right) {
    return (left < right) || (right < left);
}
