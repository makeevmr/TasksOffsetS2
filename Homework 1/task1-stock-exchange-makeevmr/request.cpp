#include "request.h"
#include <string.h>
#include <iomanip>

Request::Request(int id, char *date, char *time, char *ticker, char *type,
                 const int quantity, double price) : id_(id), quantity_(quantity), price_(price) {
    if (id != 0) {
        strncpy(date_, date, 11);
        strncpy(time_, time, 9);
        strncpy(ticker_, ticker, 16);
        strncpy(type_, type, 5);
    }
}

bool comp_by_data_and_time(const Request &request_1, const Request &request_2, const char *ticker, const int *id_array,
                           const int current_id_array_length) {
    if (compare_by_date(request_1.date_, request_2.date_)) {
        if (check_on_equality(request_1.date_, request_2.date_)) {
            if (compare_by_time(request_1.time_, request_2.time_)) {
                return true;
            }
            if (check_on_equality(request_1.time_, request_2.time_)) {
                int index = 0;
                while (id_array[index] != request_1.id_ && id_array[index] != request_2.id_) {
                    ++index;
                }
                return id_array[index] != request_1.id_;
            }
            return false;
        }
        return true;
    }
    return false;
}


bool compare_by_date(const char *date_str_1, const char *date_str_2) {
    int sep_str_1[3] = {0, 0, 0};
    int sep_str_2[3] = {0, 0, 0};
    data_and_time_parse(date_str_1, sep_str_1);
    data_and_time_parse(date_str_2, sep_str_2);
    for (int i = 2; i >= 0; --i) {
        if (sep_str_1[i] > sep_str_2[i]) {
            return true;
        }
        if (sep_str_1[i] < sep_str_2[i]) {
            return false;
        }
    }
    return true;
}

bool compare_by_time(const char *time_str_1, const char *time_str_2, const int *id_array,
                     const int current_id_array_length) {
    int sep_str_1[3] = {0, 0, 0};
    int sep_str_2[3] = {0, 0, 0};
    data_and_time_parse(time_str_1, sep_str_1);
    data_and_time_parse(time_str_2, sep_str_2);
    for (int i = 0; i < 3; ++i) {
        if (sep_str_1[i] > sep_str_2[i]) {
            return true;
        }
        if (sep_str_1[i] < sep_str_2[i]) {
            return false;
        }
    }
    return false;
}

bool check_on_equality(const char *str_1, const char *str_2) {
    int sep_str_1[3] = {0, 0, 0};
    int sep_str_2[3] = {0, 0, 0};
    data_and_time_parse(str_1, sep_str_1);
    data_and_time_parse(str_2, sep_str_2);
    for (int i = 0; i < 3; ++i) {
        if (sep_str_1[i] != sep_str_2[i]) {
            return false;
        }
    }
    return true;
}

void data_and_time_parse(const char data[11], int *parsed_str) {
    char number[5] = "";
    int number_length = 0;
    int parsed_str_length = 0;
    while (*data != '\0') {
        if (*data == '.' || *data == ':') {
            for (int i = number_length; i < 5; ++i) {
                number[i] = '\0';
            }
            parsed_str[parsed_str_length] = atoi(number);
            ++parsed_str_length;
            number_length = 0;
        } else {
            number[number_length] = *data;
            ++number_length;
        }
        ++data;
    }
    if (number_length > 0) {
        parsed_str[parsed_str_length] = atoi(number);
    }
}

void data_reverse(const char data[11], char *reversed_data) {
    char splitted_data[3][5] = {"\0", "\0", "\0"};
    int i = 0;
    int j = 0;
    do {
        if (*data == '.') {
            ++i;
            j = 0;
        } else {
            splitted_data[i][j] = *data;
            ++j;
        }
        ++data;
    } while (*data != '\0');
    int reversed_data_length = 0;
    i = 2;
    j = 0;
    while (i >= 0) {
        if (splitted_data[i][j] == '\0') {
            j = 0;
            if (i > 0) {
                reversed_data[reversed_data_length] = '.';
            }
            --i;
        } else {
            reversed_data[reversed_data_length] = splitted_data[i][j];
            ++j;
        }
        ++reversed_data_length;
    }
}

void lower_case(const char *initial_str, char *lower_str) {
    while (*initial_str != '\0') {
        *lower_str = static_cast<char>(std::tolower(*initial_str));
        ++lower_str;
        ++initial_str;
    }
    *lower_str = '\0';
}

bool compare_by_buy(const Request &request_1, const Request &request_2, const char *ticker, const int *id_array,
                    const int current_id_array_length) {
    int compare_ticker_1 = strcmp(request_1.ticker_, ticker);
    int compare_ticker_2 = strcmp(request_2.ticker_, ticker);
    if (compare_ticker_1 != 0 && compare_ticker_2 == 0) {
        return true;
    }
    if (compare_ticker_1 == 0 && compare_ticker_2 != 0) {
        return false;
    }
    if (compare_ticker_1 == 0) {
        int compare_type_1 = strcmp(request_1.type_, "sell");
        int compare_type_2 = strcmp(request_2.type_, "sell");
        if (compare_type_1 != 0 && compare_type_2 == 0) {
            return true;
        }
        if (compare_type_1 == 0 && compare_type_2 != 0) {
            return false;
        }
        if (compare_type_1 == 0) {
            return request_1.price_ > request_2.price_;
        }
        return false;
    }
    return false;
}

bool compare_by_sell(const Request &request_1, const Request &request_2, const char *ticker, const int *id_array,
                     const int current_id_array_length) {
    int compare_ticker_1 = strcmp(request_1.ticker_, ticker);
    int compare_ticker_2 = strcmp(request_2.ticker_, ticker);
    if (compare_ticker_1 != 0 && compare_ticker_2 == 0) {
        return true;
    }
    if (compare_ticker_1 == 0 && compare_ticker_2 != 0) {
        return false;
    }
    if (compare_ticker_1 == 0) {
        int compare_type_1 = strcmp(request_1.type_, "buy");
        int compare_type_2 = strcmp(request_2.type_, "buy");
        if (compare_type_1 != 0 && compare_type_2 == 0) {
            return true;
        }
        if (compare_type_1 == 0 && compare_type_2 != 0) {
            return false;
        }
        if (compare_type_1 == 0) {
            return request_1.price_ < request_2.price_;
        }
        return false;
    }
    return false;
}

bool compare_by_dom(const Request &request_1, const Request &request_2, const char *ticker, const int *id_array,
                    const int current_id_array_length) {
    char request_1_lower_ticker[16];
    char request_2_lower_ticker[16];
    char lower_ticker[16];
    lower_case(request_1.ticker_, request_1_lower_ticker);
    lower_case(request_2.ticker_, request_2_lower_ticker);
    lower_case(ticker, lower_ticker);
    int compare_ticker_1 = strcmp(request_1_lower_ticker, lower_ticker);
    int compare_ticker_2 = strcmp(request_2_lower_ticker, lower_ticker);
    if (compare_ticker_1 != 0 && compare_ticker_2 == 0) {
        return true;
    }
    if (compare_ticker_1 == 0 && compare_ticker_2 != 0) {
        return false;
    }
    if (compare_ticker_1 == 0) {
        if (request_1.quantity_ == 0) {
            return true;
        }
        if (request_2.quantity_ == 0) {
            return false;
        }
        if (request_1.price_ < request_2.price_) {
            return true;
        }
        if (request_1.price_ > request_2.price_) {
            return false;
        }
        if (strcmp(request_1.type_, "buy") == 0 && strcmp(request_2.type_, "sell") == 0) {
            return true;
        }
        if (strcmp(request_1.type_, "sell") == 0 && strcmp(request_2.type_, "buy") == 0) {
            return false;
        }
        if (strcmp(request_1.type_, "sell") == 0) {
            if (!compare_by_date(request_1.date_, request_2.date_)) {
                return true;
            }
            if (check_on_equality(request_1.date_, request_2.date_)) {
                if (compare_by_time(request_1.time_, request_2.time_)) {
                    return false;
                }
                if (check_on_equality(request_1.time_, request_2.time_)) {
                    int index = 0;
                    while (id_array[index] != request_1.id_ && id_array[index] != request_2.id_) {
                        ++index;
                    }
                    return id_array[index] == request_1.id_;
                }
                return true;
            }
            return false;
        }
        if (strcmp(request_1.type_, "buy") == 0) {
            if (compare_by_date(request_1.date_, request_2.date_)) {
                if (check_on_equality(request_1.date_, request_2.date_)) {
                    if (compare_by_time(request_1.time_, request_2.time_)) {
                        return true;
                    }
                    if (check_on_equality(request_1.time_, request_2.time_)) {
                        int index = 0;
                        while (id_array[index] != request_1.id_ && id_array[index] != request_2.id_) {
                            ++index;
                        }
                        return id_array[index] == request_1.id_;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &out, const Request &request) {
    char reversed_date[11] = "\0";
    data_reverse(request.date_, reversed_date);
    out << reversed_date << ' ' << request.time_ << ", " << request.ticker_ << ", " << request.type_ << ", " <<
        request.quantity_ << ", " << std::fixed << std::setprecision(2) << request.price_ << '\n';
    return out;
}

std::ofstream &operator<<(std::ofstream &out, const Request &request) {
    char reversed_date[11] = "\0";
    data_reverse(request.date_, reversed_date);
    out << reversed_date << ' ' << request.time_ << ", " << request.ticker_ << ", " << request.type_ << ", " <<
        request.quantity_ << ", " << std::fixed << std::setprecision(2) << request.price_ << '\n';
    return out;
}
