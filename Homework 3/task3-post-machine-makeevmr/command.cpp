#include "command.h"

command * program_parse(std::ifstream &input_file, int &capacity) {
    capacity = 5;
    command *commands_array = new command[capacity];
    int current_commands_array_length = 0;
    char command_number_char[11];
    int command_number_index = 0;
    bool command_number_is_formed = false;
    bool operation_is_formed = false;
    bool jump1_is_formed = false;
    char new_character;
    do {
        new_character = static_cast<char>(input_file.get());
        if (new_character == '%') {
            do {
            } while (input_file.get() != 10);
        } else if ((new_character == ' ' || new_character == '\n' || new_character == EOF) &&
                   command_number_index > 0) {
            if (current_commands_array_length == capacity) {
                capacity *= 2;
                command *tmp = new command[capacity];
                for (int i = 0; i < current_commands_array_length; ++i) {
                    tmp[i] = commands_array[i];
                }
                delete[] commands_array;
                commands_array = tmp;
            }
            command_number_char[command_number_index] = '\0';
            command_number_index = 0;
            if (!command_number_is_formed) {
                commands_array[current_commands_array_length].command_number = atoi(command_number_char);
                command_number_is_formed = true;
            } else if (!operation_is_formed) {
                commands_array[current_commands_array_length].operation = command_number_char[0];
                if (commands_array[current_commands_array_length].operation == '!') {
                    ++current_commands_array_length;
                    command_number_is_formed = false;
                } else {
                    operation_is_formed = true;
                }

            } else if (!jump1_is_formed) {
                commands_array[current_commands_array_length].jump_to_1 = atoi(command_number_char);
                jump1_is_formed = true;
                if (commands_array[current_commands_array_length].operation != '?') {
                    ++current_commands_array_length;
                    command_number_is_formed = false;
                    operation_is_formed = false;
                    jump1_is_formed = false;
                }
            } else if (commands_array[current_commands_array_length].operation == '?') {
                commands_array[current_commands_array_length].jump_to_2 = atoi(command_number_char);
                ++current_commands_array_length;
                command_number_is_formed = false;
                operation_is_formed = false;
                jump1_is_formed = false;
            }
        } else {
            if (new_character != '.') {
                command_number_char[command_number_index] = new_character;
                ++command_number_index;
            }
        }
    } while (new_character != EOF);
    if (current_commands_array_length < capacity) {
        command *tmp = new command[current_commands_array_length];
        for (int i = 0; i < current_commands_array_length; ++i) {
            tmp[i] = commands_array[i];
        }
        delete[] commands_array;
        capacity = current_commands_array_length;
        commands_array = tmp;
    }
    return commands_array;
}

void quick_sort(command *commands, int start, int end) {
    if (start < end) {
        int p = partition(commands, start, end);
        quick_sort(commands, start, p);
        quick_sort(commands, p + 1, end);
    }
}

int partition(command *commands, int start, int end) {
    int pivot = commands[(start + end) / 2].command_number;
    while (true) {
        while (commands[start].command_number < pivot) {
            ++start;
        }
        while (commands[end].command_number > pivot) {
            --end;
        }
        if (start >= end) {
            return end;
        }
        std::swap(commands[start++], commands[end--]);
    }
}

int find_command_number(const command *commands, int left, int right, int command_to_find) {
    while (left <= right) {
        int middle = (left + right) / 2;
        if (commands[middle].command_number == command_to_find) {
            return middle;
        }
        if (commands[middle].command_number < command_to_find) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    return -1;
}
