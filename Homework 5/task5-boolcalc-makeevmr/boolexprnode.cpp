#include "boolexprnode.h"

// class ConstantNode
ConstantNode::ConstantNode(bool value) : _value(value) {}

bool ConstantNode::calc() const { return _value; }

std::string ConstantNode::str() const { return std::to_string(static_cast<int>(_value)); }

// class VarialbeNode
VariableNode::VariableNode(int varialbe_number)
    : _variable_value(static_cast<bool>(0)), _variable_number(varialbe_number) {}

bool VariableNode::calc() const { return _variable_value; }

std::string VariableNode::str() const { return std::string("x") + std::to_string(_variable_number); }

void VariableNode::setValue(bool value) { _variable_value = value; }

bool VariableNode::getValue() const { return _variable_value; }

const int &VariableNode::getNumber() const { return _variable_number; }

bool VariableNode::compareByNumber(VariableNode *left, VariableNode *right) {
    return left->_variable_number < right->_variable_number;
}

// class UnaryNode
UnaryNode::UnaryNode(BooleanExpressionNode *next) : _next(next) {}

UnaryNode::~UnaryNode() {
    if (typeid(*_next) != typeid(VariableNode)) {
        delete _next;
        _next = nullptr;
    }
}

// class NegationNode
NegationNode::NegationNode(BooleanExpressionNode *next) : UnaryNode(next) {}

bool NegationNode::calc() const { return !(_next->calc()); }

std::string NegationNode::str() const {
    if (typeid(*_next) == typeid(VariableNode) || typeid(*_next) == typeid(ConstantNode)) {
        return (std::string("~") + _next->str());
    }
    return "~(" + _next->str() + std::string(")");
}

// class BinaryNode
BinaryNode::BinaryNode(const Binary &child_nodes) : _left(child_nodes._left), _right(child_nodes._right) {}

BinaryNode::~BinaryNode() {
    if (typeid(*_left) != typeid(VariableNode)) {
        delete _left;
        _left = nullptr;
    }
    if (typeid(*_right) != typeid(VariableNode)) {
        delete _right;
        _right = nullptr;
    }
}

// class Conjunction Node
ConjunctionNode::ConjunctionNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool ConjunctionNode::calc() const { return _left->calc() && _right->calc(); }

std::string ConjunctionNode::str() const {
    std::string conjunction_substr;
    if (typeid(*_left) == typeid(NegationNode) || typeid(*_left) == typeid(ConstantNode) ||
        typeid(*_left) == typeid(VariableNode)) {
        conjunction_substr += _left->str();
    } else {
        conjunction_substr += std::string("(") + _left->str() + std::string(")");
    }
    conjunction_substr += " & ";
    if (typeid(*_right) == typeid(NegationNode) || typeid(*_right) == typeid(ConstantNode) ||
        typeid(*_right) == typeid(VariableNode)) {
        conjunction_substr += _right->str();
    } else {
        conjunction_substr += std::string("(") + _right->str() + std::string(")");
    }
    return conjunction_substr;
}

// class DisjunctionNode
DisjunctionNode::DisjunctionNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool DisjunctionNode::calc() const { return _left->calc() || _right->calc(); }

std::string DisjunctionNode::str() const { return _left->str() + " v " + _right->str(); }

// class XORNode
XORNode::XORNode(BooleanExpressionNode *left, BooleanExpressionNode *right) : BinaryNode(Binary{left, right}) {}

bool XORNode::calc() const { return !(_left->calc() == _right->calc()); }

std::string XORNode::str() const { return _left->str() + " + " + _right->str(); }

// class ImplicationNode
ImplicationNode::ImplicationNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool ImplicationNode::calc() const { return !_left->calc() || _right->calc(); }

std::string ImplicationNode::str() const { return _left->str() + " > " + _right->str(); }

// class ReverseImplicationNode
ReverseImplicationNode::ReverseImplicationNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool ReverseImplicationNode::calc() const { return _left->calc() || !_right->calc(); }

std::string ReverseImplicationNode::str() const { return _left->str() + " < " + _right->str(); }

// class EquivalenceNode
EquivalenceNode::EquivalenceNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool EquivalenceNode::calc() const { return _left->calc() == _right->calc(); }

std::string EquivalenceNode::str() const { return _left->str() + " = " + _right->str(); }

// class SchaefferStrokeNode
SchaefferStrokeNode::SchaefferStrokeNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool SchaefferStrokeNode::calc() const { return !_left->calc() || !_right->calc(); }

std::string SchaefferStrokeNode::str() const { return _left->str() + " | " + _right->str(); }

// class PierceArrowNode
PierceArrowNode::PierceArrowNode(BooleanExpressionNode *left, BooleanExpressionNode *right)
    : BinaryNode(Binary{left, right}) {}

bool PierceArrowNode::calc() const { return !_left->calc() && !_right->calc(); }

std::string PierceArrowNode::str() const { return _left->str() + " ^ " + _right->str(); }