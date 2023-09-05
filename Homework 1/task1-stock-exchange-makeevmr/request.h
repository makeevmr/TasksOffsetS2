#ifndef OFFSET_TASKS_S2_REQUEST_H
#define OFFSET_TASKS_S2_REQUEST_H

#include "exceptions.h"
#include <iostream>
#include <fstream>

struct Request {
    int id_;
    char date_[11];
    char time_[9];
    char ticker_[16];
    char type_[5];
    int quantity_;
    double price_;
public:
    Request(int id = 0, char *date = nullptr, char *time = nullptr, char *ticker = nullptr, char *type = nullptr,
            int quantity = 0, double price = 0);
};

bool comp_by_data_and_time(const Request &request_1, const Request &request_2, const char *ticker,
                           const int *id_array, int current_id_array_length);

bool compare_by_date(const char *date_str_1, const char *date_str_2);

bool compare_by_time(const char *time_str_1, const char *time_str_2, const int *id_array = nullptr,
                     int current_id_array_length = 0);

bool check_on_equality(const char *str_1, const char *str_2);

void data_and_time_parse(const char data[11], int *parsed_str);

void data_reverse(const char data[11], char *reversed_data);

void lower_case(const char *initial_str, char *lower_str);

bool compare_by_buy(const Request &request_1, const Request &request_2, const char *ticker,
                    const int *id_array = nullptr, int current_id_array_length = 0);

bool compare_by_sell(const Request &request_1, const Request &request_2, const char *ticker,
                     const int *id_array = nullptr, int current_id_array_length = 0);

bool compare_by_dom(const Request &request_1, const Request &request_2, const char *ticker, const int *id_array,
                    int current_id_array_length);

std::ostream &operator<<(std::ostream &out, const Request &request);

std::ofstream &operator<<(std::ofstream &out, const Request &request);

#endif //OFFSET_TASKS_S2_REQUEST_H