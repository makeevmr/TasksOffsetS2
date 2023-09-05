#include "postmachine.h"
#include <iostream>
#include <string>

void input_file_parse(std::ifstream &input_file, unsigned long int &tape_size, unsigned long int &max_steps,
                      char *input_tape) {
    char new_character;
    char int_to_str[32];
    int current_index_str = 0;
    bool tape_size_is_formed = false;
    bool max_steps_is_formed = false;
    do {
        new_character = static_cast<char>(input_file.get());
        if (new_character == ' ' || new_character == EOF) {
            int_to_str[current_index_str] = '\0';
            current_index_str = 0;
            if (!tape_size_is_formed) {
                tape_size_is_formed = true;
                tape_size = atoi(int_to_str);
            } else if (!max_steps_is_formed) {
                max_steps_is_formed = true;
                max_steps = atoi(int_to_str);
            } else {
                int index = 0;
                do {
                    input_tape[index] = int_to_str[index];
                } while (int_to_str[index++] != '\0');
            }
        } else {
            int_to_str[current_index_str] = new_character;
            ++current_index_str;
        }
    } while (new_character != EOF);
}

int main(int argc, char *argv[]) {
    std::ifstream program_file(argv[1]);
    std::ifstream input_file(argv[2]);
    std::ofstream output_file(argv[3]);
    unsigned long int tape_size;
    unsigned long int max_steps = 0;
    char input_tape[32];
    if (program_file.is_open() && input_file.is_open()) {
        input_file_parse(input_file, tape_size, max_steps, input_tape);
        PostMachine m1(tape_size, program_file);
        output_file << m1.calc(std::string(input_tape), max_steps);
    }
    program_file.close();
    input_file.close();
    output_file.close();
    return 0;
}
