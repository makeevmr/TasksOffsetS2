#ifndef BOOLCALC_ERROR
#define BOOLCALC_ERROR
#include <string>

class Error {
public:
    virtual std::string what() const { return std::string("Unknown error"); }
};

class ErrorInvalidConstant : public Error {
public:
    std::string what() const { return std::string("Invalid constant (only 0 and 1 allowed)"); }
};

class ErrorTwoOperands : public Error {
public:
    std::string what() const { return std::string("Two operands in a row"); }
};

class ErrorTwoOperators : public Error {
public:
    std::string what() const { return std::string("Two binary operators in a row"); }
};

class ErrorOpeningBracket : public Error {
public:
    std::string what() const { return std::string("No closing bracket for opening bracket"); }
};

class ErrorClosingBracket : public Error {
public:
    std::string what() const { return std::string("No opening bracket for closing bracket"); }
};

class ErrorInvalidVariableFormat : public Error {
    std::string _failed_str;
    size_t _index;

public:
    ErrorInvalidVariableFormat(std::string failed_str, size_t index)
        : _failed_str(std::move(failed_str)), _index(index) {}

    std::string what() const {
        return std::string("Invalid variable format in \"") + _failed_str + std::string("\" at position: ") +
               std::to_string(_index);
    }
};

class ErrorUnknownSymbol : public Error {
    char _unknown_symbol;

public:
    ErrorUnknownSymbol(char unknown_symbol) : _unknown_symbol(unknown_symbol) {}

    std::string what() const { return std::string("Unknown symbol: ") + std::string(&_unknown_symbol); }
};

#endif /* BOOLCALC_ERROR */