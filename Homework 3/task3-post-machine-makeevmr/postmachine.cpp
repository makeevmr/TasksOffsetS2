#include "postmachine.h"

PostMachine::PostMachine(unsigned long int tape_size, std::ifstream &program_file) : tape_size_(tape_size) {
    program_size_ = 0;
    command *commands = program_parse(program_file, program_size_);
    quick_sort(commands, 0, program_size_ - 1);
    program_ = commands;
}

PostMachine::~PostMachine() {
    delete[] program_;
    program_ = nullptr;
}

std::string PostMachine::calc(std::string const &init, unsigned long int maxsteps) {
    unsigned long int current_steps = 0;
    unsigned long int current_position = 0;
    bool jump_to_second_command = false;
    bool program_stopped = false;
    bool error_occured = false;
    int current_command_index = 0;
    std::string tape = init + std::string(this->tape_size_ - init.size(), '0');
    std::string result;
    while (current_steps < maxsteps) {
        ++current_steps;
        if (program_[current_command_index].operation == 'R') {
            current_position = (current_position + 1) % tape_size_;
        }
        if (program_[current_command_index].operation == 'L') {
            if (current_position == 0) {
                current_position = tape_size_ - 1;
            } else {
                --current_position;
            }
        }
        if (program_[current_command_index].operation == 'V') {
            tape[current_position] = '1';
        }
        if (program_[current_command_index].operation == 'X') {
            tape[current_position] = '0';
        }
        if (program_[current_command_index].operation == '?') {
            if (tape[current_position] == '1') {
                jump_to_second_command = true;
            }
        }
        if (program_[current_command_index].operation == '!') {
            program_stopped = true;
            break;
        }
        if (jump_to_second_command) {
            current_command_index = find_command_number(program_, 0, program_size_ - 1,
                                                        program_[current_command_index].jump_to_2);
            jump_to_second_command = false;
        } else {
            current_command_index = find_command_number(program_, 0, program_size_ - 1,
                                                        program_[current_command_index].jump_to_1);
        }
        if (current_command_index == -1) {
            result = "Error";
            error_occured = true;
        }
    }
    if (program_stopped) {
        std::string tmp;
        for (unsigned long int i = current_position; i < tape_size_; ++i) {
            if (tape[i] == '1') {
                result += tmp + '1';
                tmp = "";
            } else {
                tmp += tape[i];
            }
        }
        for (unsigned long int i = 0; i < current_position; ++i) {
            if (tape[i] == '1') {
                result += tmp + '1';
                tmp = "";
            } else {
                tmp += tape[i];
            }
        }
    } else if (!error_occured) {
        result = "Not applicable";
    }
    return result;
}