#ifndef MATRIXLAB_BIGINTEGER
#define MATRIXLAB_BIGINTEGER

#include <limits>
#include <stdexcept>
#include <string>

struct ULPair {
    unsigned long int &result;
    unsigned long int &carry;
};

class BigInteger {
    bool negative_;
    int size_;
    unsigned long int *ul_digit_number_;
    void swap(BigInteger &other);

public:
    BigInteger(int number = 0);

    BigInteger(const std::string &number);

    BigInteger(const BigInteger &other);

    BigInteger(BigInteger &&other) noexcept;

    ~BigInteger();

    BigInteger &operator=(const BigInteger &other);

    BigInteger &operator=(BigInteger &&other) noexcept;

    BigInteger &operator+=(const BigInteger &other);

    BigInteger &operator-=(const BigInteger &other);

    BigInteger &operator*=(const BigInteger &other);

    BigInteger &operator/=(const BigInteger &other);

    BigInteger &operator%=(const BigInteger &other);

    BigInteger operator++(int);

    BigInteger &operator++();

    BigInteger operator--(int);

    BigInteger &operator--();

    BigInteger operator-() const;

    BigInteger &divide_by_two();

    const unsigned long int *get_data() const;

    int get_size() const;

    bool is_negative() const;

    void changeSign();
};

bool operator<(const BigInteger &left_big_int, const BigInteger &right_big_int);

bool operator<=(const BigInteger &left_big_int, const BigInteger &right_big_int);

bool operator>(const BigInteger &left_big_int, const BigInteger &right_big_int);

bool operator>=(const BigInteger &left_big_int, const BigInteger &right_big_int);

bool operator==(const BigInteger &left_big_int, const BigInteger &right_big_int);

bool operator!=(const BigInteger &left_big_int, const BigInteger &right_big_int);

BigInteger operator+(const BigInteger &right_big_int);

BigInteger operator+(const BigInteger &left_big_int, const BigInteger &right_big_int);

BigInteger operator-(const BigInteger &left_big_int, const BigInteger &right_big_int);

BigInteger operator*(const BigInteger &left_big_int, const BigInteger &right_big_int);

BigInteger operator/(const BigInteger &left_big_int, const BigInteger &right_big_int);

BigInteger operator%(const BigInteger &left_big_int, const BigInteger &right_big_int);

std::string to_string(const BigInteger &big_int);

std::string str_sum(const std::string &left_string, std::string &right_string);

std::string multiply_by_2(const std::string &input_string);

void divide_by_2(const std::string &input_string, std::string &output_string, int &remainder);

std::string string_reverse(const std::string &input_string);

unsigned long int to_decimal(const std::string &binary_string);

unsigned long int lo_bits(unsigned long int x, int separator);

unsigned long int hi_bits(unsigned long int x, int separator);

void uli_multiply(unsigned long int a, unsigned long int b, ULPair &ul_pair, int separator);

BigInteger division_by_bin_search(const BigInteger &dividend, const BigInteger &divisor, BigInteger &left_limit,
                                  BigInteger &right_limit);

int get_separator();

#endif // MATRIXLAB_BIGINTEGER