#ifndef BOOLCALC_BOOL_EXPR
#define BOOLCALC_BOOL_EXPR
#include "boolexprnode.h"
#include "error.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

class BooleanExpression {
private:
    BooleanExpressionNode *_root;
    bool *_truth_table;
    size_t _truth_table_size;
    std::vector<VariableNode *> _variables_array;
    static void validOperand(const std::string &operand_buffer);
    static int precedence(unsigned char new_symbol);
    static void pushOperand(std::string &operand_buffer, std::string &postfix_str, bool &is_last_operand);
    void addVariable(std::stack<BooleanExpressionNode *> &nodes_stack, std::string &variable_number,
                     bool &variable_found);
    static std::string infixToPostfix(const char *infix_str);
    BooleanExpressionNode *postfixToTree(const std::string &postfix_str);
    void setVariablesInitialPosition();
    void setVariablesNextPosition();

public:
    BooleanExpression(const char *infix_str);
    explicit operator std::string() const;
    std::string cnf();
    std::string dnf();
    std::string zhegalkin();
    const bool *getTruthTable() const;
    const size_t &getTruthTableSize() const;
    ~BooleanExpression();
};

bool isFullSystem(const std::vector<BooleanExpression *> &expr_array);

#endif /* BOOLCALC_BOOL_EXPR */