#include "BigIntFractionMatrix.h"
#include "BigIntegerFraction.h"
#include "MatrixOperators.h"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

void binaryOperator(std::stack<BigIntFractionMatrix> &matrix_stack,
                    std::map<std::string, BigIntFractionMatrix> &matrix_map, unsigned char operator_name) {
    BigIntFractionMatrix result_matrix;
    BigIntFractionMatrix right = std::move(matrix_stack.top());
    matrix_stack.pop();
    BigIntFractionMatrix left = std::move(matrix_stack.top());
    matrix_stack.pop();
    BigIntFractionMatrix *right_variable = &right;
    BigIntFractionMatrix *left_variable = &left;
    bool is_left_variable = false;
    bool is_right_variable = false;
    if (!right.getVariableName().empty()) {
        right_variable = &matrix_map[right.getVariableName()];
        is_right_variable = true;
    }
    if (!left.getVariableName().empty()) {
        left_variable = &matrix_map[left.getVariableName()];
        is_left_variable = true;
    }
    switch (operator_name) {
    case '+':
        if (!is_left_variable) {
            matrix_stack.push(std::move(*left_variable += *right_variable));
        } else if (!is_right_variable) {
            matrix_stack.push(std::move(*right_variable += *left_variable));
        } else {
            matrix_stack.push(std::move(*left_variable + *right_variable));
        }
        break;
    case 'M':
        if (left_variable->isScalar() || right_variable->isScalar()) {
            if (left_variable->isScalar()) {
                if (!is_right_variable) {
                    matrix_stack.push(std::move(right_variable->multiplyByScapar(*left_variable)));
                } else {
                    BigIntFractionMatrix right_variable_copy = *right_variable;
                    matrix_stack.push(std::move(right_variable_copy.multiplyByScapar(*left_variable)));
                }
            } else {
                if (!is_left_variable) {
                    matrix_stack.push(std::move(left_variable->multiplyByScapar(*right_variable)));
                } else {
                    BigIntFractionMatrix left_variable_copy = *left_variable;
                    matrix_stack.push(std::move(left_variable_copy.multiplyByScapar(*right_variable)));
                }
            }
        } else {
            if (!is_left_variable) {
                matrix_stack.push(std::move(*left_variable *= *right_variable));
            } else {
                matrix_stack.push(std::move(*left_variable * *right_variable));
            }
        }
        break;
    case 'D':
        if (!is_left_variable && !is_right_variable) {
            matrix_stack.push(std::move(*left_variable /= *right_variable));
        } else {
            matrix_stack.push(std::move(*left_variable / *right_variable));
        }
        break;
    case 'm':
        if (!is_left_variable) {
            matrix_stack.push(std::move(left_variable->elementWiseMultiplication(*right_variable)));
        } else if (!is_right_variable) {
            matrix_stack.push(std::move(right_variable->elementWiseMultiplication(*left_variable)));
        } else {
            BigIntFractionMatrix left_variable_copy(*left_variable);
            matrix_stack.push(std::move(left_variable_copy.elementWiseMultiplication(*right_variable)));
        }
        break;
    case 'd':
        if (!is_left_variable) {
            matrix_stack.push(std::move(left_variable->elementWiseDivision(*right_variable)));
        } else {
            BigIntFractionMatrix left_variable_copy(*left_variable);
            matrix_stack.push(std::move(left_variable_copy.elementWiseDivision(*right_variable)));
        }
        break;
    case 'z':
        result_matrix.zeros(MatrixSize{left_variable->getScalarInt(), right_variable->getScalarInt()});
        matrix_stack.push(std::move(result_matrix));
        break;
    case 'o':
        result_matrix.ones(MatrixSize{left_variable->getScalarInt(), right_variable->getScalarInt()});
        matrix_stack.push(std::move(result_matrix));
        break;
    case 'n':
        if (!is_left_variable) {
            matrix_stack.push(std::move(left_variable->min(*right_variable)));
        } else if (!is_right_variable) {
            matrix_stack.push(std::move(right_variable->min(*left_variable)));
        } else {
            BigIntFractionMatrix left_variable_copy(*left_variable);
            matrix_stack.push(std::move(left_variable_copy.min(*right_variable)));
        }
        break;
    case 'x':
        if (!is_left_variable) {
            matrix_stack.push(std::move(left_variable->max(*right_variable)));
        } else if (!is_right_variable) {
            matrix_stack.push(std::move(right_variable->max(*left_variable)));
        } else {
            BigIntFractionMatrix left_variable_copy(*left_variable);
            matrix_stack.push(std::move(left_variable_copy.max(*right_variable)));
        }
        break;
    }
}

void unaryOperator(std::stack<BigIntFractionMatrix> &matrix_stack,
                   std::map<std::string, BigIntFractionMatrix> &matrix_map, unsigned char operator_name) {
    std::string variable_name = matrix_stack.top().getVariableName();
    BigIntFractionMatrix right;
    if (variable_name.empty()) {
        right = std::move(matrix_stack.top());
    } else {
        right = matrix_map[variable_name];
    }
    matrix_stack.pop();
    switch (operator_name) {
    case 'e':
        right.eye(right.getScalarInt());
        matrix_stack.push(std::move(right));
        break;
    case 't':
        right.transpose();
        matrix_stack.push(std::move(right));
        break;
    case '|':
        matrix_stack.push(right.det());
        break;
    case 'i':
        right.inv();
        matrix_stack.push(std::move(right));
        break;
    case 's':
        matrix_stack.push(std::move(right.sum()));
        break;
    case 'p':
        matrix_stack.push(std::move(right.prod()));
        break;
    }
}

void multiOperator(std::stack<BigIntFractionMatrix> &matrix_stack,
                   std::map<std::string, BigIntFractionMatrix> &matrix_map, const MatrixOperator &last_operator,
                   unsigned char operator_name) {
    std::vector<BigIntFractionMatrix> matrix_array;
    while (&matrix_stack.top() != last_operator.getStartPosition()) {
        std::string variable_name = matrix_stack.top().getVariableName();
        if (variable_name.empty()) {
            matrix_array.push_back(std::move(matrix_stack.top()));
        } else {
            matrix_array.push_back(matrix_map[variable_name]);
        }
        matrix_stack.pop();
    }
    BigIntFractionMatrix result_matrix;
    switch (operator_name) {
    case 'v':
        result_matrix.vertcat(matrix_array);
        matrix_stack.push(std::move(result_matrix));
        break;
    case 'h':
        result_matrix.horzcat(matrix_array);
        matrix_stack.push(std::move(result_matrix));
        break;
    case '-':
        if (matrix_array.size() == 2) {
            matrix_stack.push(std::move(matrix_array[1] -= matrix_array[0]));
        } else {
            matrix_stack.push(std::move(matrix_array[0].reverseMatrix()));
        }
        break;
    }
}

int main(int argc, char *argv[]) {
    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2]);
    if (input_file.is_open() && output_file.is_open()) {
        std::stack<MatrixOperator> operators_stack;
        std::stack<std::string> variables_stack;
        std::stack<BigIntFractionMatrix> matrix_stack;
        std::map<std::string, BigIntFractionMatrix> matrix_map;
        std::map<std::string, unsigned char> operators_map = {
            {"=", '='},       {"+", '+'},         {"-", '-'},    {".*", 'm'},  {"./", 'd'},       {"*", 'M'},
            {"/", 'D'},       {"zeors", 'z'},     {"ones", 'o'}, {"eye", 'e'}, {"linspace", 'l'}, {"vertcat", 'v'},
            {"horzcat", 'h'}, {"transpose", 't'}, {"det", '|'},  {"inv", 'i'}, {"sum", 's'},      {"prod", 'p'},
            {"min", 'n'},     {"max", 'x'},       {"disp", '<'}};
        std::string command;
        while (input_file.peek() != EOF) {
            char new_symbol = static_cast<char>(input_file.get());
            if (new_symbol == '(' || new_symbol == '\r' || new_symbol == '\n') {
                continue;
            }
            if ((new_symbol == ')' || new_symbol == ' ')) {
                if (!command.empty()) {
                    bool found_operator = false;
                    unsigned char matrix_operator;
                    BigIntFractionMatrix *start_position = nullptr;
                    std::map<std::string, unsigned char>::iterator operator_iter = operators_map.find(command);
                    if (operator_iter != operators_map.end()) {
                        found_operator = true;
                        matrix_operator = operator_iter->second;
                        if (matrix_operator == '-' || matrix_operator == 'v' || matrix_operator == 'h') {
                            start_position = &matrix_stack.top();
                        }
                    }
                    if (found_operator) {
                        operators_stack.push(MatrixOperator(matrix_operator, start_position));
                    } else {
                        if (('a' <= command[0] && command[0] <= 'z') || ('A' <= command[0] && command[0] <= 'Z')) {
                            if (matrix_map.find(command) != matrix_map.end()) {
                                matrix_stack.push(BigIntFractionMatrix(command));
                            } else {
                                variables_stack.push(command);
                            }
                        } else {
                            if (command.size() < 10) {
                                matrix_stack.push(std::move(
                                    BigIntFractionMatrix(BigIntegerFraction(IntFraction{std::stoi(command), 1}))));
                            } else {
                                matrix_stack.push(
                                    std::move(BigIntFractionMatrix(BigIntegerFraction(StringFraction{command, "1"}))));
                            }
                        }
                    }
                    command = "";
                }
                if (new_symbol == ')') {
                    MatrixOperator last_operator = operators_stack.top();
                    operators_stack.pop();
                    const unsigned char operator_symbol = last_operator.getOperator();
                    if (operator_symbol == '=') {
                        matrix_map.insert({variables_stack.top(), std::move(matrix_stack.top())});
                        variables_stack.pop();
                        matrix_stack.pop();
                    } else if (operator_symbol == '+' || operator_symbol == 'M' || operator_symbol == 'D' ||
                               operator_symbol == 'm' || operator_symbol == 'd' || operator_symbol == 'z' ||
                               operator_symbol == 'o' || operator_symbol == 'n' || operator_symbol == 'x') {
                        binaryOperator(matrix_stack, matrix_map, operator_symbol);
                    } else if (operator_symbol == 'e' || operator_symbol == 't' || operator_symbol == '|' ||
                               operator_symbol == 'i' || operator_symbol == 's' || operator_symbol == 'p') {
                        unaryOperator(matrix_stack, matrix_map, operator_symbol);
                    } else if (operator_symbol == '-' || operator_symbol == 'v' || operator_symbol == 'h') {
                        multiOperator(matrix_stack, matrix_map, last_operator, operator_symbol);
                    } else if (operator_symbol == 'l') {
                        BigIntFractionMatrix third_operand = std::move(matrix_stack.top());
                        matrix_stack.pop();
                        BigIntFractionMatrix second_operand = std::move(matrix_stack.top());
                        matrix_stack.pop();
                        BigIntFractionMatrix first_operand = std::move(matrix_stack.top());
                        matrix_stack.pop();
                        BigIntFractionMatrix *third_variable = &third_operand;
                        BigIntFractionMatrix *second_variable = &second_operand;
                        BigIntFractionMatrix *first_variable = &first_operand;
                        if (!third_operand.getVariableName().empty()) {
                            third_variable = &matrix_map[third_operand.getVariableName()];
                        }
                        if (!second_operand.getVariableName().empty()) {
                            second_variable = &matrix_map[second_operand.getVariableName()];
                        }
                        if (!first_operand.getVariableName().empty()) {
                            first_variable = &matrix_map[first_operand.getVariableName()];
                        }
                        BigIntFractionMatrix result_matrix;
                        result_matrix.linspace(NumberLine{*first_variable, *second_variable}, *third_variable);
                        matrix_stack.push(std::move(result_matrix));
                    } else if (operator_symbol == '<') {
                        BigIntFractionMatrix right = std::move(matrix_stack.top());
                        matrix_stack.pop();
                        if (!right.getVariableName().empty()) {
                            output_file << right.getVariableName() << " = "
                                        << std::string(matrix_map[right.getVariableName()]);
                        } else {
                            output_file << "ans = " << std::string(right);
                        }
                        output_file << '\n';
                    }
                }
            } else {
                command.push_back(new_symbol);
            }
        }
    }
    input_file.close();
    output_file.close();
    return 0;
}
