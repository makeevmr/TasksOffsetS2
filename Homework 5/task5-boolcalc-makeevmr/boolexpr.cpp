#include "boolexpr.h"

void BooleanExpression::validOperand(const std::string &operand_buffer) {
    if (operand_buffer.length() > 1 || (operand_buffer.length() == 1 && operand_buffer[0] == 'x')) {
        if (operand_buffer.length() == 1 && operand_buffer[0] == 'x') {
            throw ErrorInvalidVariableFormat(operand_buffer, 0);
        }
        for (size_t i = 0; i < operand_buffer.length(); ++i) {
            if (i == 0 && operand_buffer[i] != 'x') {
                throw ErrorInvalidVariableFormat(operand_buffer, 1);
            }
            if (i > 0 && (operand_buffer[i] < '0' || operand_buffer[i] > '9')) {
                throw ErrorInvalidVariableFormat(operand_buffer, i);
            }
        }
    } else {
        if (operand_buffer[0] != '1' && operand_buffer[0] != '0') {
            throw ErrorInvalidConstant();
        }
    }
}

int BooleanExpression::precedence(unsigned char new_symbol) {
    if (new_symbol == '~') {
        return 3;
    }
    if (new_symbol == '&') {
        return 2;
    }
    return 1;
}

void BooleanExpression::pushOperand(std::string &operand_buffer, std::string &postfix_str, bool &is_last_operand) {
    BooleanExpression::validOperand(operand_buffer);
    if (operand_buffer.length() == 1) {
        postfix_str.push_back('c');
        postfix_str.push_back(operand_buffer[0]);
    } else {
        for (size_t j = 0; j < operand_buffer.length(); ++j) {
            postfix_str.push_back(operand_buffer[j]);
        }
    }
    if (!is_last_operand) {
        is_last_operand = true;
    } else {
        throw ErrorTwoOperands();
    }
    operand_buffer = "";
}

void BooleanExpression::addVariable(std::stack<BooleanExpressionNode *> &nodes_stack, std::string &variable_number,
                                    bool &variable_found) {
    VariableNode *new_variable = nullptr;
    for (size_t i = 0; i < _variables_array.size(); ++i) {
        if (_variables_array[i]->getNumber() == std::stoi(variable_number)) {
            new_variable = _variables_array[i];
        }
    }
    if (new_variable == nullptr) {
        new_variable = new VariableNode(std::stoi(variable_number));
        _variables_array.push_back(new_variable);
    }
    nodes_stack.push(new_variable);
    variable_number = "";
    variable_found = false;
}

std::string BooleanExpression::infixToPostfix(const char *infix_str) {
    bool is_last_operand = false; // to check for two operands and two binary operators in a row
    std::stack<char> stack;
    std::string operand_buffer;
    std::string postfix_str;
    const std::string bool_operators = "~&v+><=|^";
    size_t index = 0;
    while (infix_str[index] != '\0') {
        char new_symbol = infix_str[index];
        if (new_symbol == 'x' || ('0' <= new_symbol && new_symbol <= '9')) {
            operand_buffer.push_back(new_symbol);
        } else {
            if (operand_buffer.length() > 0) {
                pushOperand(operand_buffer, postfix_str, is_last_operand);
            }
            if (new_symbol == ' ') {
                ++index;
                continue;
            }
            if (new_symbol == '(') {
                stack.push(new_symbol);
            } else if (new_symbol == ')') {
                while (!stack.empty() && (stack.top() != '(')) {
                    postfix_str.push_back(stack.top());
                    stack.pop();
                }
                if (stack.empty()) {
                    throw ErrorClosingBracket();
                }
                stack.pop();
            } else if (std::find(bool_operators.begin(), bool_operators.end(), new_symbol) != bool_operators.end()) {
                if (new_symbol != '~') {
                    if (is_last_operand) {
                        is_last_operand = false;
                    } else {
                        throw ErrorTwoOperators();
                    }
                }
                if ((stack.empty() || (!stack.empty() && stack.top() == '(')) ||
                    (precedence(new_symbol) > precedence(stack.top())) ||
                    ((precedence(new_symbol) == precedence(stack.top())) && (new_symbol == '~'))) {
                    stack.push(new_symbol);
                } else {
                    while (!stack.empty() && stack.top() != '(' &&
                           (precedence(new_symbol) <= precedence(stack.top())) &&
                           (new_symbol != '~' || stack.top() != '~')) {
                        postfix_str.push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(new_symbol);
                }
            } else {
                throw ErrorUnknownSymbol(new_symbol);
            }
        }
        ++index;
    }
    if (operand_buffer.length() > 0) {
        pushOperand(operand_buffer, postfix_str, is_last_operand);
    }
    while (!stack.empty()) {
        if (stack.top() == '(') {
            throw ErrorOpeningBracket();
        }
        postfix_str.push_back(stack.top());
        stack.pop();
    }
    return postfix_str;
}

BooleanExpressionNode *BooleanExpression::postfixToTree(const std::string &postfix_str) {
    std::stack<BooleanExpressionNode *> nodes_stack;
    BooleanExpressionNode *result;
    BooleanExpressionNode *left;
    BooleanExpressionNode *right;
    VariableNode *new_variable;
    std::string variable_number;
    bool variable_found = false;
    bool constant_found = false;
    for (char symbol : postfix_str) {
        if (variable_found) {
            if (symbol < '0' || symbol > '9') {
                addVariable(nodes_stack, variable_number, variable_found);
            } else {
                variable_number.push_back(symbol);
            }
        }
        result = nullptr;
        if (constant_found && (symbol == '0' || symbol == '1')) {
            result = new ConstantNode(static_cast<bool>(static_cast<int>(symbol) - 48));
            constant_found = false;
        }
        switch (symbol) {
        case 'x':
            variable_found = true;
            break;
        case 'c':
            constant_found = true;
            break;
        case '~':
            right = nodes_stack.top();
            nodes_stack.pop();
            result = new NegationNode(right);
            break;
        case '&':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new ConjunctionNode(left, right);
            break;
        case 'v':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new DisjunctionNode(left, right);
            break;
        case '+':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new XORNode(left, right);
            break;
        case '>':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new ImplicationNode(left, right);
            break;
        case '<':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new ReverseImplicationNode(left, right);
            break;
        case '=':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new EquivalenceNode(left, right);
            break;
        case '|':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new SchaefferStrokeNode(left, right);
            break;
        case '^':
            right = nodes_stack.top();
            nodes_stack.pop();
            left = nodes_stack.top();
            nodes_stack.pop();
            result = new PierceArrowNode(left, right);
            break;
        }
        if (result != nullptr) {
            nodes_stack.push(result);
        }
    }
    if (!variable_number.empty()) {
        addVariable(nodes_stack, variable_number, variable_found);
    }
    std::sort(_variables_array.begin(), _variables_array.end(), VariableNode::compareByNumber);
    if (!nodes_stack.empty()) {
        return nodes_stack.top();
    }
    return nullptr;
}

void BooleanExpression::setVariablesInitialPosition() {
    for (size_t i = 0; i < _variables_array.size(); ++i) {
        _variables_array[i]->setValue(false);
    }
}

void BooleanExpression::setVariablesNextPosition() {
    size_t index = _variables_array.size() - 1;
    while (_variables_array[index]->getValue()) {
        _variables_array[index]->setValue(false);
        --index;
    }
    _variables_array[index]->setValue(true);
}

BooleanExpression::BooleanExpression(const char *infix_str) {
    std::string postfix_str = infixToPostfix(infix_str);
    _root = postfixToTree(postfix_str);
    if (_root != nullptr) {
        _truth_table_size = 1;
    } else {
        _truth_table_size = 0;
    }
    for (size_t i = 0; i < _variables_array.size(); ++i) {
        _truth_table_size *= 2;
    }
    _truth_table = new bool[_truth_table_size];
    size_t iteration = 0;
    while (iteration < _truth_table_size) {
        _truth_table[iteration] = _root->calc();
        ++iteration;
        if (iteration != _truth_table_size) {
            setVariablesNextPosition();
        }
    }
}

BooleanExpression::operator std::string() const {
    if (_root != nullptr) {
        return _root->str();
    }
    return "";
}

std::string BooleanExpression::cnf() {
    if (_truth_table_size == 0) {
        return "";
    }
    if (typeid(*_root) == typeid(ConstantNode)) {
        return std::string(*this);
    }
    setVariablesInitialPosition();
    size_t iteration = 0;
    bool first_conjunct = true;
    std::string cnf_string;
    while (iteration < _truth_table_size) {
        if (!_truth_table[iteration]) {
            if (!first_conjunct) {
                cnf_string += " & ";
            }
            first_conjunct = false;
            cnf_string.push_back('(');
            bool first_member = true;
            for (size_t i = 0; i < _variables_array.size(); ++i) {
                if (!first_member) {
                    cnf_string += " v ";
                }
                first_member = false;
                if (_variables_array[i]->getValue()) {
                    cnf_string.push_back('~');
                }
                cnf_string += _variables_array[i]->str();
            }
            cnf_string.push_back(')');
        }
        ++iteration;
        if (iteration != _truth_table_size) {
            setVariablesNextPosition();
        }
    }
    return cnf_string;
}

std::string BooleanExpression::dnf() {
    if (_truth_table_size == 0) {
        return "";
    }
    if (typeid(*_root) == typeid(ConstantNode)) {
        return std::string(*this);
    }
    setVariablesInitialPosition();
    size_t iteration = 0;
    bool first_disjunct = true;
    std::string dnf_string;
    while (iteration < _truth_table_size) {
        if (_truth_table[iteration]) {
            if (!first_disjunct) {
                dnf_string += " v ";
            }
            first_disjunct = false;
            bool first_member = true;
            for (size_t i = 0; i < _variables_array.size(); ++i) {
                if (!first_member) {
                    dnf_string += " & ";
                }
                first_member = false;
                if (!_variables_array[i]->getValue()) {
                    dnf_string.push_back('~');
                }
                dnf_string += _variables_array[i]->str();
            }
        }
        ++iteration;
        if (iteration != _truth_table_size) {
            setVariablesNextPosition();
        }
    }
    return dnf_string;
}

std::string BooleanExpression::zhegalkin() {
    if (_truth_table_size == 0) {
        return "";
    }
    if (typeid(*_root) == typeid(ConstantNode)) {
        return std::string(*this);
    }
    setVariablesInitialPosition();
    bool first_member = true;
    std::string zhegalkin_string;
    size_t triangle_column_size = _truth_table_size;
    bool *triangle_column = new bool[triangle_column_size];
    for (size_t i = 0; i < triangle_column_size; ++i) {
        triangle_column[i] = _truth_table[i];
    }
    if (triangle_column[0]) {
        zhegalkin_string += "1";
        first_member = false;
    }
    size_t iteration = 1;
    while (iteration < _truth_table_size) {
        setVariablesNextPosition();
        --triangle_column_size;
        for (size_t i = 0; i < triangle_column_size; ++i) {
            if (triangle_column[i] == triangle_column[i + 1]) {
                triangle_column[i] = false;
            } else {
                triangle_column[i] = true;
            }
        }
        if (triangle_column[0]) {
            if (!first_member) {
                zhegalkin_string += " + ";
            }
            first_member = false;
            bool first_conjunct = true;
            for (size_t i = 0; i < _variables_array.size(); ++i) {
                if (_variables_array[i]->getValue()) {
                    if (!first_conjunct) {
                        zhegalkin_string += " & ";
                    }
                    first_conjunct = false;
                    zhegalkin_string += _variables_array[i]->str();
                }
            }
        }
        ++iteration;
    }
    delete[] triangle_column;
    return zhegalkin_string;
}

const bool *BooleanExpression::getTruthTable() const { return _truth_table; }

const size_t &BooleanExpression::getTruthTableSize() const { return _truth_table_size; }

BooleanExpression::~BooleanExpression() {
    if (_root != nullptr && typeid(*_root) != typeid(VariableNode)) {
        delete _root;
    }
    for (std::vector<VariableNode *>::iterator iter = _variables_array.begin(); iter != _variables_array.end();
         ++iter) {
        delete *iter;
    }
    delete[] _truth_table;
    _root = nullptr;
}

bool isFullSystem(const std::vector<BooleanExpression *> &expr_array) {
    bool is_keep_zero = true;
    bool is_keep_one = true;
    bool is_linear = true;
    bool is_self_dual = true;
    bool is_monotonous = true;
    for (size_t i = 0; i < expr_array.size(); ++i) {
        if (is_keep_zero && expr_array[i]->getTruthTable()[0]) {
            is_keep_zero = false;
        }
        size_t current_table_size = expr_array[i]->getTruthTableSize();
        if (is_keep_one && !expr_array[i]->getTruthTable()[current_table_size - 1]) {
            is_keep_one = false;
        }
        if (current_table_size == 1) {
            continue;
        }
        if (is_linear && expr_array[i]->zhegalkin().find('&') != std::string::npos) {
            is_linear = false;
        }
        if (is_self_dual) {
            for (size_t j = 0; j < current_table_size / 2; ++j) {
                if (expr_array[i]->getTruthTable()[j] == expr_array[i]->getTruthTable()[current_table_size - 1 - j]) {
                    is_self_dual = false;
                }
            }
        }
        if (is_monotonous) {
            unsigned char *param_table = new unsigned char[current_table_size];
            for (unsigned char current_param = 0; current_param < static_cast<unsigned char>(current_table_size);
                 ++current_param) {
                param_table[current_param] = current_param;
            }
            for (size_t j = 0; j < current_table_size - 1; ++j) {
                for (size_t k = j + 1; k < current_table_size; ++k) {
                    if (((param_table[j] & param_table[k]) == param_table[j]) && expr_array[i]->getTruthTable()[j] &&
                        !expr_array[i]->getTruthTable()[k]) {
                        is_monotonous = false;
                    }
                }
            }
            delete[] param_table;
        }
        if (!is_keep_zero && !is_keep_one && !is_linear && !is_self_dual && !is_monotonous) {
            break;
        }
    }
    return !is_keep_zero && !is_keep_one && !is_linear && !is_self_dual && !is_monotonous;
}