#include "boolexpr.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

int main(int argc, char *argv[]) {
    bool cnf = false;
    bool dnf = false;
    bool zh = false;
    bool is_full = false;
    bool exception_thrown = false;
    size_t infix_str_length = 0;
    const size_t infix_str_capacity = 100;
    char *infix_str = new char[infix_str_capacity];
    std::vector<BooleanExpression *> expr_array;
    if (argc > 2) {
        if (strcmp(argv[1], "-cnf") == 0) {
            cnf = true;
        } else if (strcmp(argv[1], "-dnf") == 0) {
            dnf = true;
        } else if (strcmp(argv[1], "-zh") == 0) {
            zh = true;
        } else {
            is_full = true;
        }
        std::ifstream input_file(argv[2]);
        std::ofstream output_file(argv[3]);
        if (input_file.is_open() && output_file.is_open()) {
            bool first_line = true;
            while (input_file.peek() != EOF) {
                char new_symbol = static_cast<char>(input_file.get());
                if (new_symbol != '\n' && new_symbol != '\r') {
                    infix_str[infix_str_length] = new_symbol;
                    ++infix_str_length;
                } else if (infix_str_length > 0) {
                    try {
                        infix_str[infix_str_length] = '\0';
                        infix_str_length = 0;
                        if (!first_line && !is_full) {
                            output_file << '\n';
                        }
                        first_line = false;
                        expr_array.push_back(new BooleanExpression(infix_str));
                        if (cnf) {
                            output_file << expr_array.back()->cnf();
                        }
                        if (dnf) {
                            output_file << expr_array.back()->dnf();
                        }
                        if (zh) {
                            output_file << expr_array.back()->zhegalkin();
                        }
                    } catch (const Error &error) {
                        exception_thrown = true;
                        output_file << "error";
                        break;
                    }
                }
            }
            if (infix_str_length > 0) {
                try {
                    infix_str[infix_str_length] = '\0';
                    if (!first_line && !is_full) {
                        output_file << '\n';
                    }
                    expr_array.push_back(new BooleanExpression(infix_str));
                    if (cnf) {
                        output_file << expr_array.back()->cnf();
                    }
                    if (dnf) {
                        output_file << expr_array.back()->dnf();
                    }
                    if (zh) {
                        output_file << expr_array.back()->zhegalkin();
                    }
                } catch (const Error &error) {
                    exception_thrown = true;
                    output_file << "error";
                }
            }
        }
        if (is_full && !exception_thrown) {
            if (isFullSystem(expr_array)) {
                output_file << "yes";
            } else {
                output_file << "no";
            }
        }
        input_file.close();
        output_file.close();
        for (size_t i = 0; i < expr_array.size(); ++i) {
            delete expr_array[i];
            expr_array[i] = nullptr;
        }
    } else {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "?") == 0) {
            std::ifstream input_file("help.txt");
            if (input_file.is_open()) {
                while (input_file.peek() != EOF) {
                    std::cout << static_cast<char>(input_file.get());
                }
            }
            std::cout << '\n';
            input_file.close();
        }
    }
    delete[] infix_str;
    return 0;
}