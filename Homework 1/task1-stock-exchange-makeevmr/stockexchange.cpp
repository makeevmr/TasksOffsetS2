#include <fstream>
#include "StockDB.h"


int main() {
    const int command_length = 60;
    int parsed_str_length = 0;
    char command_str[command_length];
    char parsed_str[7][12];
    int current_id_array_length = 0;
    int max_id_array_length = 20;
    int *id_array = new int[max_id_array_length];
    std::cout << "> ";
    std::cin.getline(command_str, command_length);
    command_parse(command_str, parsed_str, parsed_str_length);
    StockDB data(5);
    while (strcmp(parsed_str[0], "quit") != 0) {
        try {
            if (!valid_command(parsed_str[0])) {
                char exception[50] = "Unknown command: ";
                strncat(exception, parsed_str[0], 11);
                throw exception;
            }
            if (strcmp(parsed_str[0], "add") == 0) {
                if (!valid_date_format(parsed_str[1])) {
                    char exception[50] = "Wrong date format: ";
                    strncat(exception, parsed_str[1], 11);
                    throw exception;
                }
                char number[6];
                if (!valid_data_day(parsed_str[1], number)) {
                    char exception[50] = "Day value is invalid: ";
                    strncat(exception, number, 11);
                    throw exception;
                }
                if (!valid_data_month(parsed_str[1], number)) {
                    char exception[50] = "Month value is invalid: ";
                    strncat(exception, number, 11);
                    throw exception;
                }
                if (!valid_data_year(parsed_str[1], number)) {
                    char exception[50] = "Year value is invalid: ";
                    strncat(exception, number, 11);
                    throw exception;
                }
                if (!valid_time_format(parsed_str[2])) {
                    char exception[50] = "Wrong time format: ";
                    strncat(exception, parsed_str[2], 11);
                    throw exception;
                }
                if (!valid_time(parsed_str[2])) {
                    char exception[50] = "Time value is invalid: ";
                    strncat(exception, parsed_str[2], 11);
                    throw exception;
                }
                if (!valid_request_type(parsed_str[4])) {
                    char exception[50] = "Unknown command: ";
                    strncat(exception, parsed_str[4], 11);
                    throw exception;
                }
            }
        } catch (const char *error) {
            std::cout << error << std::endl;
        }
        if (strcmp(parsed_str[0], "load") == 0) {
            std::ifstream input_file(parsed_str[1], std::ios::binary);
            if (input_file.is_open()) {
                data.load(input_file, id_array, current_id_array_length, max_id_array_length);
                input_file.close();
            }
        } else if (strcmp(parsed_str[0], "save") == 0) {
            std::ofstream output_file(parsed_str[1], std::ios::binary);
            if (output_file.is_open()) {
                data.sort("data", "\0", id_array, current_id_array_length);
                data.save(output_file);
                output_file.close();
            }
        } else if (strcmp(parsed_str[0], "add") == 0) {
            data.add(parsed_str[1], parsed_str[2], parsed_str[3], parsed_str[4], atoi(parsed_str[5]),
                     atof(parsed_str[6]), id_array, current_id_array_length, max_id_array_length);
        } else if (strcmp(parsed_str[0], "delete") == 0) {
            data.delete_request(atoi(parsed_str[1]));
        } else if (strcmp(parsed_str[0], "clear") == 0) {
            data.clear();
        } else if (strcmp(parsed_str[0], "print") == 0) {
            data.sort("data", "\0", id_array, current_id_array_length);
            data.print();
        } else if (strcmp(parsed_str[0], "dom") == 0) {
            data.sort("dom", parsed_str[1], id_array, current_id_array_length);
            data.dom(parsed_str[1]);
        } else if (strcmp(parsed_str[0], "export") == 0) {
            std::ofstream output_file(parsed_str[1], std::ios::binary);
            if (output_file.is_open()) {
                data.sort("data", "\0", id_array, current_id_array_length);
                data.export_database(output_file);
                output_file.close();
            }
        }
        std::cout << "> ";
        std::cin.getline(command_str, command_length);
        parsed_str_length = 0;
        command_parse(command_str, parsed_str, parsed_str_length);
    }
    delete[] id_array;
    return 0;
}