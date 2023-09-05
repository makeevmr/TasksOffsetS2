#ifndef OFFSET_TASKS_S2_STOCKDB_H
#define OFFSET_TASKS_S2_STOCKDB_H

#include "request.h"
#include <cstring>

class StockDB {
    int unique_id_;
    int current_length_;
    int max_length_;
    Request *database_;

public:

    StockDB(int max_length);

    ~StockDB();

    void sort(const char *comparison_method, const char *ticker = "\0", const int *id_array = nullptr,
              int current_id_array_length = 0);

    void load(std::ifstream &input_file, int *&id_array, int &current_id_array_length, int &max_id_array_length);

    void save(std::ofstream &output_file) const;

    void add(char *date, char *time, char *ticker, char *type, int quantity, double price, int *&id_array,
             int &current_id_array_length, int &max_id_array_length);

    void delete_request(int id);

    void clear();

    void print() const;

    void dom(const char *ticker) const;

    void export_database(std::ofstream &output_file) const;
};

void command_parse(char *command_str, char parsed_str[][12], int &parsed_str_length);

template<typename T>
void merge(T *array, int left_index, int mid_index, int right_index,
           bool (*comparator)(const T &, const T &, const char *, const int *, const int), const char *ticker,
           const int *id_array, const int current_id_array_length) {
    int left_array_length = mid_index - left_index + 1;
    int right_array_length = right_index - mid_index;
    Request *left_array = new Request[left_array_length];
    Request *right_array = new Request[right_array_length];
    for (int i = 0; i < left_array_length; ++i) {
        left_array[i] = array[left_index + i];
    }
    for (int j = 0; j < right_array_length; ++j) {
        right_array[j] = array[mid_index + j + 1];
    }
    int left_array_index = 0;
    int right_array_index = 0;
    int array_index = left_index;
    while (left_array_index < left_array_length && right_array_index < right_array_length) {
        if (comparator(left_array[left_array_index], right_array[right_array_index], ticker, id_array,
                       current_id_array_length)) {
            array[array_index] = right_array[right_array_index];
            ++right_array_index;
        } else {
            array[array_index] = left_array[left_array_index];
            ++left_array_index;
        }
        ++array_index;
    }
    while (left_array_index < left_array_length) {
        array[array_index] = left_array[left_array_index];
        ++left_array_index;
        ++array_index;
    }
    while (right_array_index < right_array_length) {
        array[array_index] = right_array[right_array_index];
        ++right_array_index;
        ++array_index;
    }
    delete[] left_array;
    delete[] right_array;
}

template<typename T>
void merge_sort(T *array, int left_index, int right_index,
                bool (*comparator)(const T &, const T &, const char *, const int *, const int),
                const char *ticker = "\0", const int *id_array = nullptr, const int current_id_array_length = 0) {
    if (left_index >= right_index) {
        return;
    }
    int mid_index = (left_index + right_index) / 2;
    merge_sort(array, left_index, mid_index, comparator, ticker, id_array, current_id_array_length);
    merge_sort(array, mid_index + 1, right_index, comparator, ticker, id_array, current_id_array_length);
    merge(array, left_index, mid_index, right_index, comparator, ticker, id_array, current_id_array_length);
}

#endif //OFFSET_TASKS_S2_STOCKDB_H
