#include "StockDB.h"
#include <fstream>
#include <iomanip>

StockDB::StockDB(int max_length) : unique_id_(1), current_length_(0), max_length_(max_length) {
    database_ = new Request[max_length_];
}

StockDB::~StockDB() {
    if (database_ != nullptr) {
        delete[] database_;
        database_ = nullptr;
    }
}

void StockDB::sort(const char *comparison_method, const char *ticker, const int *id_array,
                   const int current_id_array_length) {
    if (strcmp(comparison_method, "data") == 0) {
        merge_sort(this->database_, 0, this->current_length_ - 1, comp_by_data_and_time, ticker, id_array,
                   current_id_array_length);
    }
    if (strcmp(comparison_method, "buy") == 0) {
        merge_sort(this->database_, 0, this->current_length_ - 1, compare_by_buy, ticker);
    }
    if (strcmp(comparison_method, "sell") == 0) {
        merge_sort(this->database_, 0, this->current_length_ - 1, compare_by_sell, ticker);
    }
    if (strcmp(comparison_method, "dom") == 0) {
        merge_sort(this->database_, 0, this->current_length_ - 1, compare_by_dom, ticker, id_array,
                   current_id_array_length);
    }
}

void StockDB::load(std::ifstream &input_file, int *&id_array,
                   int &current_id_array_length, int &max_id_array_length) {
    const int command_length = 60;
    char command_str[command_length];
    char parsed_str[7][12];
    while (input_file.peek() != EOF) {
        input_file.getline(command_str, command_length);
        int parsed_str_length = 0;
        command_parse(command_str, parsed_str, parsed_str_length);
        this->add(parsed_str[0], parsed_str[1], parsed_str[2], parsed_str[3], atoi(parsed_str[4]), atof(parsed_str[5]),
                  id_array, current_id_array_length, max_id_array_length);
    }
}

void StockDB::save(std::ofstream &output_file) const {
    for (size_t i = 0; i < current_length_; ++i) {
        output_file << database_[i].date_ << ' ' << database_[i].time_ << ", " << database_[i].ticker_ << ", "
                    << database_[i].type_ << ", " << database_[i].quantity_ << ", " << database_[i].price_ << '\n';
    }
}

void StockDB::add(char *date, char *time, char *ticker, char *type, int quantity, double price, int *&id_array,
                  int &current_id_array_length, int &max_id_array_length) {
    if (current_length_ == max_length_) {
        max_length_ *= 2;
        Request *tmp = new Request[max_length_];
        for (int i = 0; i < current_length_; ++i) {
            tmp[i] = database_[i];
        }
        delete[] database_;
        database_ = tmp;
    }
    if (current_length_ > 0) {
        this->sort(type, ticker);
        int index = 0;
        while (quantity > 0 && strcmp(database_[index].ticker_, ticker) == 0 &&
               strcmp(type, strcmp(database_[index].type_, "buy") == 0 ? "sell" : "buy") == 0) {
            if (strcmp(type, "buy") == 0 && price < database_[index].price_) {
                break;
            }
            if (strcmp(type, "sell") == 0 && price > database_[index].price_) {
                break;
            }
            if (quantity >= database_[index].quantity_) {
                quantity -= database_[index].quantity_;
                database_[index].quantity_ = 0;
            } else {
                database_[index].quantity_ -= quantity;
                quantity = 0;
            }
            ++index;
        }
    }
    if (current_id_array_length >= max_id_array_length) {
        max_id_array_length *= 2;
        int *tmp = new int[max_id_array_length];
        for (int i = 0; i < current_id_array_length; ++i) {
            tmp[i] = id_array[i];
        }
        delete[] id_array;
        id_array = tmp;
    }
    id_array[current_id_array_length] = unique_id_;
    ++current_id_array_length;
    database_[current_length_] = Request(unique_id_, date, time, ticker, type, quantity, price);
    ++unique_id_;
    ++current_length_;
}

void StockDB::delete_request(int id) {
    if (current_length_ == 1) {
        delete[] database_;
        database_ = new Request[max_length_];
    } else {
        int index = 0;
        while (database_[index].id_ != id) {
            ++index;
        }
        for (int i = index + 1; i < current_length_; ++i) {
            database_[i - 1] = database_[i];
        }
    }
    --current_length_;
}

void StockDB::clear() {
    Request *tmp = new Request[max_length_];
    int tmp_length = 0;
    for (int i = 0; i < current_length_; ++i) {
        if (database_[i].quantity_ > 0) {
            tmp[tmp_length] = database_[i];
            ++tmp_length;
        }
    }
    current_length_ = tmp_length;
    delete[] database_;
    database_ = tmp;
}

void StockDB::print() const {
    for (size_t i = 0; i < current_length_; ++i) {
        std::cout << database_[i].id_ << ", " << database_[i];
    }
}

void StockDB::dom(const char *ticker) const {
    if (current_length_ > 0) {
        int index = 0;
        char database_lower_ticker[16];
        char lower_ticker[16];
        lower_case(database_[index].ticker_, database_lower_ticker);
        lower_case(ticker, lower_ticker);
        while (strcmp(database_lower_ticker, lower_ticker) == 0 && database_[index].quantity_ > 0) {
            std::cout << database_[index].type_ << ", ";
            std::cout << std::fixed << std::setprecision(2) << database_[index].price_ << ", ";
            std::cout << database_[index].quantity_ << ", ";
            char reversed_date[11] = "\0";
            data_reverse(database_[index].date_, reversed_date);
            std::cout << reversed_date << ' ';
            std::cout << database_[index].time_ << '\n';
            ++index;
            lower_case(database_[index].ticker_, database_lower_ticker);
            lower_case(ticker, lower_ticker);
        }
    }
}

void StockDB::export_database(std::ofstream &output_file) const {
    for (size_t i = 0; i < current_length_; ++i) {
        output_file << database_[i];
    }
}

void command_parse(char *command_str, char parsed_str[][12],
                   int &parsed_str_length) {
    bool subcommand_found = false;
    int subcommand_length = 0;
    while (*command_str != '\0') {
        if (*command_str == ' ' || *command_str == ',') {
            if (subcommand_found) {
                parsed_str[parsed_str_length][subcommand_length] = '\0';
                ++parsed_str_length;
                subcommand_length = 0;
                subcommand_found = false;
            }
        } else {
            if (!subcommand_found) {
                subcommand_found = true;
            }
            parsed_str[parsed_str_length][subcommand_length] = *command_str;
            ++subcommand_length;
        }
        ++command_str;
    }
    parsed_str[parsed_str_length][subcommand_length] = '\0';
    ++parsed_str_length;
}
