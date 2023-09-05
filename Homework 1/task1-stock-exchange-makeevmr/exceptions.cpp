#include <cstring>
#include "exceptions.h"

bool valid_command(const char *command) {
    const int commands_number = 9;
    const char valid_commands[commands_number][7] = {"load", "save", "add", "delete", "clear", "print", "dom", "export",
                                                     "quit"};
    for (int i = 0; i < commands_number; ++i) {
        if (strcmp(command, valid_commands[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool valid_date_format(const char *date) {
    int dot_counter = 0;
    while (*date != '\0') {
        if ((*date < '0' || *date > '9') && *date != '.') {
            return false;
        }
        if (*date == '.') {
            ++dot_counter;
        }
        ++date;
    }
    return dot_counter == 2;
}

bool valid_data_day(const char *data, char *number) {
    int parsed_data[3] = {0, 0, 0};
    data_and_time_parse(data, parsed_data);
    while (*data != '.') {
        *number = *data;
        ++number;
        ++data;
    }
    *number = '\0';
    if (parsed_data[0] < 1 || parsed_data[0] > 31) {
        return false;
    }
    if ((parsed_data[1] == 4 || parsed_data[1] == 6 || parsed_data[1] == 9 || parsed_data[1] == 11) &&
        parsed_data[0] == 31) {
        return false;
    }
    if (parsed_data[1] == 2 && parsed_data[0] > 28) {
        return false;
    }
    return true;
}

bool valid_data_month(const char *data, char *number) {
    int parsed_data[3] = {0, 0, 0};
    data_and_time_parse(data, parsed_data);
    while (*data != '.') {
        ++data;
    }
    ++data;
    while (*data != '.') {
        *number = *data;
        ++number;
        ++data;
    }
    *number = '\0';
    return parsed_data[1] >= 1 && parsed_data[1] <= 12;
}

bool valid_data_year(const char *data, char *number) {
    int parsed_data[3] = {0, 0, 0};
    data_and_time_parse(data, parsed_data);
    int dot_count = 0;
    while (dot_count < 2) {
        if (*data == '.') {
            ++dot_count;
        }
        ++data;
    }
    while (*data != '\0') {
        *number = *data;
        ++number;
        ++data;
    }
    *number = '\0';
    return parsed_data[2] >= 0 && parsed_data[2] <= 9999;
}

bool valid_time_format(const char *time) {
    int colon_counter = 0;
    while (*time != '\0') {
        if ((*time < '0' || *time > '9') && *time != ':') {
            return false;
        }
        if (*time == ':') {
            ++colon_counter;
        }
        ++time;
    }
    return colon_counter == 2;
}

bool valid_time(const char *time) {
    int parsed_time[3] = {0, 0, 0};
    data_and_time_parse(time, parsed_time);
    if (parsed_time[0] < 0 || parsed_time[0] > 23) {
        return false;
    }
    if (parsed_time[1] < 0 || parsed_time[1] > 59) {
        return false;
    }
    if (parsed_time[2] < 0 || parsed_time[2] > 59) {
        return false;
    }
    return true;
}

bool valid_request_type(const char *type) {
    const char valid_request_types[2][5] = {"buy", "sell"};
    for (int i = 0; i < 2; ++i) {
        if (strcmp(type, valid_request_types[i]) == 0) {
            return true;
        }
    }
    return false;
}