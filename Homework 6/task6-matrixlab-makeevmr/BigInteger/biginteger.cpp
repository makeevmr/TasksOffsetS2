#include "biginteger.h"

void BigInteger::swap(BigInteger &other) {
    std::swap(this->size_, other.size_);
    std::swap(this->ul_digit_number_, other.ul_digit_number_);
    std::swap(this->negative_, other.negative_);
}

BigInteger::BigInteger(int number) : negative_(false), size_(1) {
    if (number < 0) {
        negative_ = true;
        number *= -1;
    }
    ul_digit_number_ = new unsigned long int[size_];
    ul_digit_number_[0] = static_cast<unsigned long int>(number);
}

BigInteger::BigInteger(const std::string &number) : negative_(false) {
    size_t number_size = number.size();
    if (number_size == 0 || (number_size == 1 && number[0] == '-')) {
        throw std::invalid_argument("empty number");
    }
    for (size_t i = 0; i < number_size; ++i) {
        if ((number[i] == '-' && i > 0) ||
            ((number[i] != '-') && (static_cast<int>(number[i]) > 57 || static_cast<int>(number[i]) < 48))) {
            char error[17] = "invalid symbol ";
            error[15] = number[i];
            throw std::invalid_argument(error);
        }
    }
    std::string binary_number;
    std::string initial_number;
    if (number[0] == '-') {
        if (number != "-0") {
            negative_ = true;
        }
        initial_number = number.substr(1, number.size() - 1);
    } else {
        initial_number = number;
    }
    // conversion to binary
    std::string integer_part_number;
    int remainder = 0;
    bool first_step = true;
    while (integer_part_number != "0" && integer_part_number != "1") {
        if (first_step) {
            first_step = false;
        } else {
            initial_number = integer_part_number;
            integer_part_number = "";
        }
        divide_by_2(initial_number, integer_part_number, remainder);
        binary_number += static_cast<char>(remainder + 48);
    }
    if (integer_part_number == "1") {
        binary_number += integer_part_number;
    }
    // conversion to unsigned long int
    int separator = get_separator();
    size_ = static_cast<int>(binary_number.size()) / separator;
    if (static_cast<int>(binary_number.size()) % separator != 0) {
        ++size_;
    }
    ul_digit_number_ = new unsigned long int[size_];
    int start_index = 0;
    for (int i = 0; i < size_; ++i) {
        ul_digit_number_[i] = to_decimal(binary_number.substr(start_index, separator));
        start_index += separator;
    }
}

BigInteger::BigInteger(const BigInteger &other) {
    this->negative_ = other.negative_;
    this->size_ = other.size_;
    this->ul_digit_number_ = new unsigned long int[this->size_];
    for (int i = 0; i < this->size_; ++i) {
        this->ul_digit_number_[i] = other.ul_digit_number_[i];
    }
}

BigInteger::BigInteger(BigInteger &&other) noexcept : negative_(false), size_(0), ul_digit_number_(nullptr) {
    this->swap(other);
}

BigInteger::~BigInteger() {
    delete[] ul_digit_number_;
    ul_digit_number_ = nullptr;
    size_ = 0;
    negative_ = false;
}

BigInteger &BigInteger::operator=(const BigInteger &other) {
    if (this != &other) {
        delete[] this->ul_digit_number_;
        this->negative_ = other.negative_;
        this->size_ = other.size_;
        this->ul_digit_number_ = new unsigned long int[this->size_];
        for (int i = 0; i < this->size_; ++i) {
            this->ul_digit_number_[i] = other.ul_digit_number_[i];
        }
    }
    return *this;
}

BigInteger &BigInteger::operator=(BigInteger &&other) noexcept {
    if (this != &other) {
        this->swap(other);
    }
    return *this;
}

BigInteger &BigInteger::operator+=(const BigInteger &other) {
    unsigned long int max_number = std::numeric_limits<unsigned long int>::max();
    if (this->negative_ == other.negative_) {
        int index = 0;
        bool max_size_left = this->size_ > other.size_;
        int max_size = max_size_left ? this->size_ : other.size_;
        unsigned long int *result_ul_number = new unsigned long int[max_size + 1];
        for (int i = 0; i < max_size + 1; ++i) {
            result_ul_number[i] = 0;
        }
        bool digit_overflow = false;
        while (index < this->size_ && index < other.size_) {
            result_ul_number[index] = this->ul_digit_number_[index] + other.ul_digit_number_[index];
            if (digit_overflow) {
                ++result_ul_number[index];
                digit_overflow = false;
            }
            if ((result_ul_number[index] < this->ul_digit_number_[index] ||
                 result_ul_number[index] < other.ul_digit_number_[index]) ||
                (result_ul_number[index] == max_number && this->ul_digit_number_[index] == max_number &&
                 other.ul_digit_number_[index] == max_number)) {
                digit_overflow = true;
            }
            ++index;
        }
        while (index < max_size) {
            if (max_size_left) {
                result_ul_number[index] = this->ul_digit_number_[index];
            } else {
                result_ul_number[index] = other.ul_digit_number_[index];
            }
            if (digit_overflow) {
                ++result_ul_number[index];
                digit_overflow = false;
                if (result_ul_number[index] == 0) {
                    digit_overflow = true;
                }
            }
            ++index;
        }
        if (digit_overflow) {
            result_ul_number[max_size] = 1;
        }
        delete[] this->ul_digit_number_;
        if (result_ul_number[max_size] != 1) {
            this->size_ = max_size;
            unsigned long int *tmp_number = new unsigned long int[max_size];
            for (int i = 0; i < max_size; ++i) {
                tmp_number[i] = result_ul_number[i];
            }
            delete[] result_ul_number;
            this->ul_digit_number_ = tmp_number;
        } else {
            this->size_ = max_size + 1;
            this->ul_digit_number_ = result_ul_number;
        }
    } else {
        bool left_negative = this->negative_;
        bool max_left;
        if (left_negative) { // compare numbers by abs value
            this->negative_ = false;
            max_left = other < *this;
        } else {
            this->negative_ = true;
            max_left = !(other < *this);
        }
        int max_size = max_left ? this->size_ : other.size_;
        unsigned long int *result_ul_number = new unsigned long int[max_size];
        for (int i = 0; i < max_size; ++i) {
            result_ul_number[i] = 0;
        }
        const BigInteger *max_big_int = max_left ? this : &other;
        const BigInteger *min_big_int = max_left ? &other : this;
        int index = 0;
        bool digit_overflow = false;
        while (index < max_big_int->size_ && index < min_big_int->size_) {
            result_ul_number[index] = max_big_int->ul_digit_number_[index] - min_big_int->ul_digit_number_[index];
            if (digit_overflow) {
                --result_ul_number[index];
            }
            digit_overflow = ((result_ul_number[index] > max_big_int->ul_digit_number_[index]) ||
                              (min_big_int->ul_digit_number_[index] == max_number && digit_overflow));
            ++index;
        }
        while (index < max_big_int->size_) {
            result_ul_number[index] = max_big_int->ul_digit_number_[index];
            if (digit_overflow) {
                if (result_ul_number[index] != 0) {
                    digit_overflow = false;
                }
                --result_ul_number[index];
            }
            ++index;
        }
        delete[] this->ul_digit_number_;
        int zero_count = 0;
        for (int i = max_size - 1; i > 0; --i) {
            if (result_ul_number[i] == 0) {
                ++zero_count;
            } else {
                break;
            }
        }
        if (zero_count > 0) {
            unsigned long int *tmp = new unsigned long int[max_size - zero_count];
            for (int i = 0; i < max_size - zero_count; ++i) {
                tmp[i] = result_ul_number[i];
            }
            delete[] result_ul_number;
            this->ul_digit_number_ = tmp;
        } else {
            this->ul_digit_number_ = result_ul_number;
        }
        this->size_ = max_size - zero_count;
        this->negative_ = max_left ? left_negative : other.negative_;
    }
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &other) {
    BigInteger other_copy(other);
    other_copy.negative_ = !other_copy.negative_;
    return *this += other_copy;
}

BigInteger &BigInteger::operator*=(const BigInteger &other) {
    bool this_sign = this->negative_;
    unsigned long int max_number = std::numeric_limits<unsigned long int>::max();
    int separator = get_separator();
    int big_int_size = this->size_ + other.size_;
    BigInteger *big_int_array = new BigInteger[other.size_];
    for (int i = 0; i < other.size_; ++i) {
        delete[] big_int_array[i].ul_digit_number_;
        big_int_array[i].ul_digit_number_ = new unsigned long int[big_int_size];
        big_int_array[i].size_ = big_int_size;
        for (int j = 0; j < big_int_size; ++j) {
            big_int_array[i].ul_digit_number_[j] = 0;
        }
        big_int_array[i].negative_ = false;
    }
    int start_index = -1;
    unsigned long int result = 0;
    unsigned long int carry = 0;
    for (int i = 0; i < other.size_; ++i) {
        bool digit_overflow = false;
        unsigned long int prev_carry = 0;
        ++start_index;
        for (int j = 0; j < this->size_; ++j) {
            ULPair ul_pair = {result, carry};
            uli_multiply(this->ul_digit_number_[j], other.ul_digit_number_[i], ul_pair, separator);
            big_int_array[i].ul_digit_number_[j + start_index] = result + prev_carry;
            if (digit_overflow) {
                ++big_int_array[i].ul_digit_number_[j + start_index];
            }
            digit_overflow = (big_int_array[i].ul_digit_number_[j + start_index] < result) ||
                             (big_int_array[i].ul_digit_number_[j + start_index] < prev_carry) ||
                             (result == max_number && prev_carry == max_number && digit_overflow);
            prev_carry = carry;
        }
        big_int_array[i].ul_digit_number_[this->size_ + start_index] = prev_carry;
        if (digit_overflow) {
            ++big_int_array[i].ul_digit_number_[this->size_ + start_index];
            if (big_int_array[i].ul_digit_number_[this->size_ + start_index] == 0) {
                if (i < other.size_ - 1) {
                    big_int_array[i].ul_digit_number_[this->size_ + start_index + 1] = 1;
                } else {
                    unsigned long int *tmp = new unsigned long int[this->size_ + other.size_ + 1];
                    for (int k = 0; k < this->size_ + other.size_; ++k) {
                        tmp[k] = big_int_array[i].ul_digit_number_[k];
                    }
                    tmp[this->size_ + other.size_] = 1;
                    delete[] big_int_array[i].ul_digit_number_;
                    big_int_array[i].ul_digit_number_ = tmp;
                    big_int_array[i].size_ = this->size_ + other.size_ + 1;
                }
            }
        }
    }
    for (int i = 0; i < other.size_; ++i) {
        int zero_count = 0;
        if (big_int_array[i].ul_digit_number_[big_int_size - 1] == 0) {
            for (int j = big_int_size - 1; j > 0; --j) {
                if (big_int_array[i].ul_digit_number_[j] == 0) {
                    ++zero_count;
                } else {
                    break;
                }
            }
            unsigned long int *tmp = new unsigned long int[big_int_size - zero_count];
            for (int j = 0; j < big_int_size - zero_count; ++j) {
                tmp[j] = big_int_array[i].ul_digit_number_[j];
            }
            delete[] big_int_array[i].ul_digit_number_;
            big_int_array[i].ul_digit_number_ = tmp;
            big_int_array[i].size_ = big_int_size - zero_count;
        }
    }
    *this = big_int_array[0];
    for (int i = 1; i < other.size_; ++i) {
        *this += big_int_array[i];
    }
    delete[] big_int_array;
    this->negative_ = (!this_sign || !other.negative_) && (this_sign || other.negative_);
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &other) {
    bool this_sign = this->negative_;
    BigInteger other_copy = other;
    BigInteger this_copy = *this;
    other_copy.negative_ = false;
    this_copy.negative_ = false;
    if (this_copy < other_copy) {
        *this = BigInteger(0);
    } else {
        BigInteger left_limit = BigInteger(0);
        BigInteger right_limit = this_copy;
        *this = division_by_bin_search(this_copy, other_copy, left_limit, right_limit);
        this->negative_ = (!this_sign || !other.negative_) && (this_sign || other.negative_);
    }
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &other) {
    bool this_sign = this->negative_;
    BigInteger this_copy = *this;
    BigInteger other_copy = other;
    this_copy.negative_ = false;
    other_copy.negative_ = false;
    BigInteger quotient = this_copy / other_copy;
    *this = this_copy - quotient * other_copy;
    this->negative_ = (this_sign && !other.negative_);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger answer = *this;
    *this += BigInteger(1);
    return answer;
}

BigInteger &BigInteger::operator++() {
    *this += BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger answer = *this;
    *this -= BigInteger(1);
    return answer;
}

BigInteger &BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator-() const {
    BigInteger opposite_big_int(*this);
    opposite_big_int.negative_ = !this->negative_;
    return opposite_big_int;
}

BigInteger &BigInteger::divide_by_two() {
    if (this->size_ > 0) {
        unsigned long int last_pow_2;
        unsigned long int last_binary_digit = this->ul_digit_number_[this->size_ - 1] & 1ULL;
        last_pow_2 = std::numeric_limits<unsigned long int>::max() / 2 + 1;
        this->ul_digit_number_[this->size_ - 1] >>= 1;
        for (int i = this->size_ - 2; i >= 0; --i) {
            unsigned long int new_last_binary_digit = this->ul_digit_number_[i] & 1ULL;
            this->ul_digit_number_[i] = last_pow_2 * last_binary_digit + (this->ul_digit_number_[i] >> 1);
            last_binary_digit = new_last_binary_digit;
        }
        if (this->ul_digit_number_[this->size_ - 1] == 0 && this->size_ > 1) {
            unsigned long int *tmp = new unsigned long int[this->size_ - 1];
            for (int i = 0; i < this->size_ - 1; ++i) {
                tmp[i] = this->ul_digit_number_[i];
            }
            delete[] this->ul_digit_number_;
            this->ul_digit_number_ = tmp;
            --this->size_;
        }
    }
    return *this;
}

const unsigned long int *BigInteger::get_data() const { return this->ul_digit_number_; }

int BigInteger::get_size() const { return this->size_; }

bool BigInteger::is_negative() const { return this->negative_; }

void BigInteger::changeSign() { negative_ = !negative_; }

bool operator<(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    if (left_big_int.get_size() == 1 && right_big_int.get_size() == 1 && *left_big_int.get_data() == 0 &&
        *right_big_int.get_data() == 0) {
        return false;
    }
    bool left_sign_negative = left_big_int.is_negative();
    bool right_sign_negative = right_big_int.is_negative();
    if (left_sign_negative == right_sign_negative) {
        int left_size = left_big_int.get_size();
        int right_size = right_big_int.get_size();
        if (left_size == right_size) {
            const unsigned long int *left_data = left_big_int.get_data();
            const unsigned long int *right_data = right_big_int.get_data();
            for (int i = left_size - 1; i >= 0; --i) {
                if (left_data[i] == right_data[i]) {
                    continue;
                }
                return !left_sign_negative ? left_data[i] < right_data[i] : !(left_data[i] < right_data[i]);
            }
            return false;
        }
        return !left_sign_negative ? left_size < right_size : !(left_size < right_size);
    }
    return left_sign_negative;
}

bool operator<=(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    return !(right_big_int < left_big_int);
}

bool operator>(const BigInteger &left_big_int, const BigInteger &right_big_int) { return right_big_int < left_big_int; }

bool operator>=(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    return !(left_big_int < right_big_int);
}

bool operator==(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    return !(left_big_int < right_big_int) && !(right_big_int < left_big_int);
}

bool operator!=(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    return (left_big_int < right_big_int) || (right_big_int < left_big_int);
}

BigInteger operator+(const BigInteger &right_big_int) { return BigInteger(right_big_int); }

BigInteger operator+(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    BigInteger result = left_big_int;
    result += right_big_int;
    return result;
}

BigInteger operator-(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    BigInteger result = left_big_int;
    result -= right_big_int;
    return result;
}

BigInteger operator*(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    BigInteger result = left_big_int;
    result *= right_big_int;
    return result;
}

BigInteger operator/(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    BigInteger result = left_big_int;
    if (right_big_int != 0) {
        result /= right_big_int;
    } else {
        throw std::runtime_error("Math error: Attempted to divide by Zero\n");
    }
    return result;
}

BigInteger operator%(const BigInteger &left_big_int, const BigInteger &right_big_int) {
    BigInteger result = left_big_int;
    if (right_big_int != 0) {
        result %= right_big_int;
    } else {
        throw std::runtime_error("Math error: Attempted to divide by Zero\n");
    }
    return result;
}

std::string to_string(const BigInteger &big_int) {
    std::string binary_str;
    for (int i = 0; i < big_int.get_size(); ++i) {
        std::string binary_number;
        std::string input_string = std::to_string(big_int.get_data()[i]);
        std::string output_string;
        int remainder = 0;
        while (input_string != "0" && input_string != "1") {
            divide_by_2(input_string, output_string, remainder);
            binary_number += std::to_string(remainder);
            input_string = output_string;
            output_string = "";
        }
        binary_number += input_string;
        if (binary_number.size() < 64) {
            binary_number += std::string(64 - binary_number.size(), '0');
        }
        binary_str += binary_number;
    }
    std::string answer = "0";
    std::string pow_2 = "1";
    size_t binary_size = binary_str.size();
    for (size_t i = 0; i < binary_size; ++i) {
        if (binary_str[i] == '1') {
            answer = str_sum(answer, pow_2);
        }
        pow_2 = multiply_by_2(pow_2);
    }
    if (big_int.is_negative()) {
        answer = "-" + answer;
    }
    return answer;
}

std::string str_sum(const std::string &left_string, std::string &right_string) {
    std::string result;
    int left_index = static_cast<int>(left_string.size()) - 1;
    int right_index = static_cast<int>(right_string.size()) - 1;
    int digit = 0;
    int carry = 0;
    while (left_index >= 0 && right_index >= 0) {
        digit = static_cast<int>(left_string[left_index]) + static_cast<int>(right_string[right_index]) - 96 + carry;
        carry = digit > 9 ? 1 : 0;
        if (digit > 9) {
            digit -= 10;
        }
        result += std::to_string(digit);
        --left_index;
        --right_index;
    }
    while (left_index >= 0) {
        digit = static_cast<int>(left_string[left_index]) - 48 + carry;
        carry = digit == 10 ? 1 : 0;
        if (digit == 10) {
            digit = 0;
        }
        result += std::to_string(digit);
        --left_index;
    }
    while (right_index >= 0) {
        digit = static_cast<int>(right_string[right_index]) - 48 + carry;
        carry = digit == 10 ? 1 : 0;
        if (digit == 10) {
            digit = 0;
        }
        result += std::to_string(digit);
        --right_index;
    }
    if (carry > 0) {
        result += "1";
    }
    result = string_reverse(result);
    return result;
}

std::string multiply_by_2(const std::string &input_string) {
    std::string result;
    int carry = 0;
    int digit = 0;
    for (size_t i = input_string.size(); i > 0; --i) {
        digit = (static_cast<int>(input_string[i - 1]) - 48) * 2;
        digit += carry;
        carry = digit > 9 ? 1 : 0;
        if (digit > 9) {
            digit -= 10;
        }
        result += std::to_string(digit);
    }
    if (carry != 0) {
        result += std::to_string(carry);
    }
    result = string_reverse(result);
    return result;
}

void divide_by_2(const std::string &input_string, std::string &output_string, int &remainder) {
    size_t input_string_size = input_string.size();
    size_t index = 0;
    remainder = static_cast<int>(input_string[index]) - 48;
    while (remainder < 2 && ++index < input_string_size) {
        remainder = remainder * 10 + static_cast<int>(input_string[index]) - 48;
    }
    while (input_string_size > ++index) {
        output_string += static_cast<char>((remainder / 2) + 48);
        remainder = (remainder % 2) * 10 + static_cast<int>(input_string[index]) - 48;
    }
    output_string += static_cast<char>((remainder / 2) + 48);
    remainder = (remainder % 2);
    if (output_string.length() == 0) {
        output_string = "0";
        remainder = 0;
    }
}

std::string string_reverse(const std::string &input_string) {
    std::string result;
    int input_string_size = static_cast<int>(input_string.size());
    for (int i = input_string_size - 1; i >= 0; --i) {
        result += input_string[i];
    }
    return result;
}

unsigned long int to_decimal(const std::string &binary_string) {
    unsigned long int power_two = 1;
    unsigned long int decimal_number = 0;
    for (size_t i = 0; i < binary_string.size(); ++i) {
        decimal_number += (static_cast<int>(binary_string[i]) - 48) * power_two;
        power_two *= 2;
    }
    return decimal_number;
}

unsigned long int lo_bits(unsigned long int x, int separator) { return ((1ULL << separator / 2) - 1) & x; }

unsigned long int hi_bits(unsigned long int x, int separator) { return x >> separator / 2; }

// calculate single ulls multiply with overflow handling
void uli_multiply(unsigned long int a, unsigned long int b, ULPair &ul_pair, int separator) {
    unsigned long int s0;
    unsigned long int s1;
    unsigned long int s2;
    unsigned long int s3;
    unsigned long int x = lo_bits(a, separator) * lo_bits(b, separator);
    s0 = lo_bits(x, separator);

    x = hi_bits(a, separator) * lo_bits(b, separator) + hi_bits(x, separator);
    s1 = lo_bits(x, separator);
    s2 = hi_bits(x, separator);

    x = s1 + lo_bits(a, separator) * hi_bits(b, separator);
    s1 = lo_bits(x, separator);

    x = s2 + hi_bits(a, separator) * hi_bits(b, separator) + hi_bits(x, separator);
    s2 = lo_bits(x, separator);
    s3 = hi_bits(x, separator);
    ul_pair.result = s1 << separator / 2 | s0;
    ul_pair.carry = s3 << separator / 2 | s2;
}

BigInteger division_by_bin_search(const BigInteger &dividend, const BigInteger &divisor, BigInteger &left_limit,
                                  BigInteger &right_limit) {
    BigInteger middle = (left_limit + right_limit).divide_by_two();
    BigInteger product = divisor * middle;
    if (dividend == product || ((product < dividend) && (dividend < product + divisor))) {
        return middle;
    }
    if (dividend < product) {
        right_limit = middle;
    } else {
        left_limit = middle + 1;
    }
    return division_by_bin_search(dividend, divisor, left_limit, right_limit);
}

int get_separator() {
    if (std::numeric_limits<unsigned long int>::max() % 100 == 15) {
        return 64;
    }
    return 32;
}