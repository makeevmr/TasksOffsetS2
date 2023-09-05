#ifndef OFFSET_TASKS_S2_POSTMACHINE_H
#define OFFSET_TASKS_S2_POSTMACHINE_H

#include <string>
#include "command.h"

class PostMachine {
    unsigned long int tape_size_;
    int program_size_;
    const command *program_;

public:
    PostMachine(unsigned long int tape_size, std::ifstream &program_file);

    ~PostMachine();

    const command *get_program() const {
        return program_;
    }

    int get_size() const {
        return program_size_;
    }

    std::string calc(std::string const &init, unsigned long int maxsteps);
};

#endif //OFFSET_TASKS_S2_POSTMACHINE_H
