#ifndef BOOLCALC_BOOL_EXPR_NODE
#define BOOLCALC_BOOL_EXPR_NODE
#include <string>
#include <typeinfo>

class BooleanExpressionNode {
public:
    virtual bool calc() const = 0;
    virtual std::string str() const = 0;
    virtual ~BooleanExpressionNode() {}
};

class ConstantNode : public BooleanExpressionNode {
    bool _value;

public:
    ConstantNode(const bool &value);
    bool calc() const override;
    std::string str() const override;
};

class VariableNode : public BooleanExpressionNode {
    bool _variable_value;
    const int _variable_number;

public:
    VariableNode(const int &varialbe_number);
    bool calc() const override;
    std::string str() const override;
    void setValue(const bool &value);
    const bool &getValue() const;
    const int &getNumber() const;
    static bool compareByNumber(VariableNode *left, VariableNode *right);
};

class UnaryNode : public BooleanExpressionNode {
protected:
    BooleanExpressionNode *_next;

public:
    UnaryNode(BooleanExpressionNode *next);
    ~UnaryNode();
};

class NegationNode : public UnaryNode {
public:
    NegationNode(BooleanExpressionNode *next);
    bool calc() const override;
    std::string str() const override;
};

struct Binary {
    BooleanExpressionNode *_left;
    BooleanExpressionNode *_right;
};

class BinaryNode : public BooleanExpressionNode {
protected:
    BooleanExpressionNode *_left, *_right;

public:
    BinaryNode(const Binary &child_nodes);
    ~BinaryNode();
};

class ConjunctionNode : public BinaryNode {
public:
    ConjunctionNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class DisjunctionNode : public BinaryNode {
public:
    DisjunctionNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

// Modulo 2 addition
class XORNode : public BinaryNode {
public:
    XORNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class ImplicationNode : public BinaryNode {
public:
    ImplicationNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class ReverseImplicationNode : public BinaryNode {
public:
    ReverseImplicationNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class EquivalenceNode : public BinaryNode {
public:
    EquivalenceNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class SchaefferStrokeNode : public BinaryNode {
public:
    SchaefferStrokeNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};

class PierceArrowNode : public BinaryNode {
public:
    PierceArrowNode(BooleanExpressionNode *left, BooleanExpressionNode *right);
    bool calc() const override;
    std::string str() const override;
};
#endif /* BOOLCALC_BOOL_EXPR_NODE */