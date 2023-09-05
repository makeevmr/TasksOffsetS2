#ifndef OFFSET_TASKS_S2_COMMAND_H
#define OFFSET_TASKS_S2_COMMAND_H

#include <fstream>

struct command {
    int command_number;
    char operation;
    int jump_to_1;
    int jump_to_2;
};

command *program_parse(std::ifstream &input_file, int &capacity);

void quick_sort(command *commands, int start, int end);

int partition(command *commands, int start, int end);

int find_command_number(const command *commands, int left, int right, int command_to_find);

#endif //OFFSET_TASKS_S2_COMMAND_H
