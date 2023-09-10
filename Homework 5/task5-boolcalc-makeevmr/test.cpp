#include <gtest/gtest.h>
#include <string>

#include "boolexpr.h"

TEST(BooleanExpressionToString, EmptyStringTest) {
    BooleanExpression bool_expr1("");
    EXPECT_EQ(std::string(bool_expr1), "");
}

TEST(BooleanExpressionToString, NoOperationsTest) {
    BooleanExpression bool_expr1("x1");
    EXPECT_EQ(std::string(bool_expr1), "x1");
    BooleanExpression bool_expr2("0");
    EXPECT_EQ(std::string(bool_expr2), "0");
    BooleanExpression bool_expr3("(x1)");
    EXPECT_EQ(std::string(bool_expr3), "x1");
}

TEST(BooleanExpressionToString, OneOperationTests) {
    BooleanExpression bool_expr1("~x1");
    EXPECT_EQ(std::string(bool_expr1), "~x1");
    BooleanExpression bool_expr2("x1 & x2");
    EXPECT_EQ(std::string(bool_expr2), "x1 & x2");
    BooleanExpression bool_expr3("x1 v x2");
    EXPECT_EQ(std::string(bool_expr3), "x1 v x2");
    BooleanExpression bool_expr4("x1 + x2");
    EXPECT_EQ(std::string(bool_expr4), "x1 + x2");
    BooleanExpression bool_expr5("x1 > x2");
    EXPECT_EQ(std::string(bool_expr5), "x1 > x2");
    BooleanExpression bool_expr6("x1 < x2");
    EXPECT_EQ(std::string(bool_expr6), "x1 < x2");
    BooleanExpression bool_expr7("x1 = x2");
    EXPECT_EQ(std::string(bool_expr7), "x1 = x2");
    BooleanExpression bool_expr8("x1 | x2");
    EXPECT_EQ(std::string(bool_expr8), "x1 | x2");
    BooleanExpression bool_expr9("x1 ^ x2");
    EXPECT_EQ(std::string(bool_expr9), "x1 ^ x2");
}

TEST(BooleanExpressionToString, TwoOperationsTests) {
    BooleanExpression bool_expr1("~(x1 v x2)");
    EXPECT_EQ(std::string(bool_expr1), "~(x1 v x2)");
    BooleanExpression bool_expr2("~x1 v x2");
    EXPECT_EQ(std::string(bool_expr2), "~x1 v x2");
    BooleanExpression bool_expr3("x1 v x2 & x3");
    EXPECT_EQ(std::string(bool_expr3), "x1 v x2 & x3");
    BooleanExpression bool_expr4("(x1 v x2) & x3");
    EXPECT_EQ(std::string(bool_expr4), "(x1 v x2) & x3");
}

TEST(BooleanExpressionToString, ThreeOperationsTests) {
    BooleanExpression bool_expr1("~(x1 v x2) & x3");
    EXPECT_EQ(std::string(bool_expr1), "~(x1 v x2) & x3");
    BooleanExpression bool_expr2("~((x1 ^ x2) < x3)");
    EXPECT_EQ(std::string(bool_expr2), "~(x1 ^ x2 < x3)");
    BooleanExpression bool_expr3("~~x1 v x2");
    EXPECT_EQ(std::string(bool_expr3), "~(~x1) v x2");
    BooleanExpression bool_expr4("x1 < x2 | x3 v x4");
    EXPECT_EQ(std::string(bool_expr4), "x1 < x2 | x3 v x4");
}

TEST(BooleanExpressionToCNF, EmptyStringTest) {
    BooleanExpression bool_expr1("");
    EXPECT_EQ(bool_expr1.cnf(), "");
}

TEST(BooleanExpressionToCNF, NoOperationsTests) {
    BooleanExpression bool_expr1("x1");
    EXPECT_EQ(bool_expr1.cnf(), "(x1)");
    BooleanExpression bool_expr2("0");
    EXPECT_EQ(bool_expr2.cnf(), "0");
    BooleanExpression bool_expr3("1");
    EXPECT_EQ(bool_expr3.cnf(), "1");
}

TEST(BooleanExpressionToCNF, TwoOperationsTests) {
    BooleanExpression bool_expr1("x1 v x2 > x3");
    EXPECT_EQ(bool_expr1.cnf(), "(x1 v ~x2 v x3) & (~x1 v x2 v x3) & (~x1 v ~x2 v x3)");
    BooleanExpression bool_expr2("x1 v (x2 > x3)");
    EXPECT_EQ(bool_expr2.cnf(), "(x1 v ~x2 v x3)");
    BooleanExpression bool_expr3("~(x1 | x2)");
    EXPECT_EQ(bool_expr3.cnf(), "(x1 v x2) & (x1 v ~x2) & (~x1 v x2)");
    BooleanExpression bool_expr4("x1 & (x2 > x3)");
    EXPECT_EQ(bool_expr4.cnf(),
              "(x1 v x2 v x3) & (x1 v x2 v ~x3) & (x1 v ~x2 v x3) & (x1 v ~x2 v ~x3) & (~x1 v ~x2 v x3)");
}

TEST(BooleanExpressionToCNF, ThreeOperationsTests) {
    BooleanExpression bool_expr1("(x1 + x2) > x3 v x4");
    EXPECT_EQ(bool_expr1.cnf(), "(x1 v ~x2 v x3 v x4) & (~x1 v x2 v x3 v x4)");
    BooleanExpression bool_expr2("(x1 ^ x2) v (x3 | x4)");
    EXPECT_EQ(bool_expr2.cnf(), "(x1 v ~x2 v ~x3 v ~x4) & (~x1 v x2 v ~x3 v ~x4) & (~x1 v ~x2 v ~x3 v ~x4)");
    BooleanExpression bool_expr3("x1 + x2 + x3 + x1");
    EXPECT_EQ(bool_expr3.cnf(), "(x1 v x2 v x3) & (x1 v ~x2 v ~x3) & (~x1 v x2 v x3) & (~x1 v ~x2 v ~x3)");
    BooleanExpression bool_expr4("(x1 < x2) | (x3 > x4)");
    EXPECT_EQ(bool_expr4.cnf(),
              "(x1 v x2 v x3 v x4) & (x1 v x2 v x3 v ~x4) & (x1 v x2 v ~x3 v ~x4) & (~x1 v x2 v x3 v x4) & (~x1 v "
              "x2 v x3 v ~x4) & (~x1 v x2 v ~x3 v ~x4) & (~x1 v ~x2 v x3 v x4) & (~x1 v ~x2 v x3 v ~x4) & (~x1 v "
              "~x2 v ~x3 v ~x4)");
}

TEST(BooleanExpressionToDNF, EmptyStringTest) {
    BooleanExpression bool_expr1("");
    EXPECT_EQ(bool_expr1.dnf(), "");
}

TEST(BooleanExpressionToDNF, NoOperationsTests) {
    BooleanExpression bool_expr1("x1");
    EXPECT_EQ(bool_expr1.dnf(), "x1");
    BooleanExpression bool_expr2("0");
    EXPECT_EQ(bool_expr2.dnf(), "0");
    BooleanExpression bool_expr3("1");
    EXPECT_EQ(bool_expr3.dnf(), "1");
}

TEST(BooleanExpressionToDNF, TwoOperationsTests) {
    BooleanExpression bool_expr1("x1 v x2 > x3");
    EXPECT_EQ(bool_expr1.dnf(), "~x1 & ~x2 & ~x3 v ~x1 & ~x2 & x3 v ~x1 & x2 & x3 v x1 & ~x2 & x3 v x1 & x2 & x3");
    BooleanExpression bool_expr2("x1 v (x2 > x3)");
    EXPECT_EQ(bool_expr2.dnf(), "~x1 & ~x2 & ~x3 v ~x1 & ~x2 & x3 v ~x1 & x2 & x3 v x1 & ~x2 & ~x3 v x1 & ~x2 & x3 v "
                                "x1 & x2 & ~x3 v x1 & x2 & x3");
    BooleanExpression bool_expr3("(x1 < x2) & x3");
    EXPECT_EQ(bool_expr3.dnf(), "~x1 & ~x2 & x3 v x1 & ~x2 & x3 v x1 & x2 & x3");
    BooleanExpression bool_expr4("x1 + x2 > x3");
    EXPECT_EQ(bool_expr4.dnf(),
              "~x1 & ~x2 & ~x3 v ~x1 & ~x2 & x3 v ~x1 & x2 & x3 v x1 & ~x2 & x3 v x1 & x2 & ~x3 v x1 & x2 & x3");
}

TEST(BooleanExpressionDNF, ThreeOperationsTests) {
    BooleanExpression bool_expr1("x1 + (x2 ^ x3) + x1");
    EXPECT_EQ(bool_expr1.dnf(), "~x1 & ~x2 & ~x3 v x1 & ~x2 & ~x3");
    BooleanExpression bool_expr2("(x1 v x2) & (x3 > x4)");
    EXPECT_EQ(bool_expr2.dnf(), "~x1 & x2 & ~x3 & ~x4 v ~x1 & x2 & ~x3 & x4 v ~x1 & x2 & x3 & x4 v x1 & ~x2 & ~x3 & "
                                "~x4 v x1 & ~x2 & ~x3 & x4 v x1 & ~x2 & x3 & x4 v x1 & x2 & ~x3 & ~x4 v x1 & x2 & ~x3 "
                                "& x4 v x1 & x2 & x3 & x4");
    BooleanExpression bool_expr3("x1 & x2 ^ x3 ^ x4");
    EXPECT_EQ(
        bool_expr3.dnf(),
        "~x1 & ~x2 & x3 & ~x4 v ~x1 & x2 & x3 & ~x4 v x1 & ~x2 & x3 & ~x4 v x1 & x2 & ~x3 & ~x4 v x1 & x2 & x3 & ~x4");
    BooleanExpression bool_expr4("(x1 + x2) & ~x3");
    EXPECT_EQ(bool_expr4.dnf(), "~x1 & x2 & ~x3 v x1 & ~x2 & ~x3");
}

TEST(BooleanExpressionToZhegalkin, EmptyStringTest) {
    BooleanExpression bool_expr1("");
    EXPECT_EQ(bool_expr1.zhegalkin(), "");
}

TEST(BooleanExpressionToZhegalkin, NoOperationsTests) {
    BooleanExpression bool_expr1("x1");
    EXPECT_EQ(bool_expr1.zhegalkin(), "x1");
    BooleanExpression bool_expr2("0");
    EXPECT_EQ(bool_expr2.zhegalkin(), "0");
    BooleanExpression bool_expr3("1");
    EXPECT_EQ(bool_expr3.zhegalkin(), "1");
}

TEST(BooleanExpressionToZhegalkin, TwoOperationsTests) {
    BooleanExpression bool_expr1("x1 > x2 v x3");
    EXPECT_EQ(bool_expr1.zhegalkin(), "1 + x1 + x1 & x3 + x1 & x2 + x1 & x2 & x3");
    BooleanExpression bool_expr2("(x1 + x2) & x3");
    EXPECT_EQ(bool_expr2.zhegalkin(), "x2 & x3 + x1 & x3");
    BooleanExpression bool_expr3("~x1 ^ x2");
    EXPECT_EQ(bool_expr3.zhegalkin(), "x1 + x1 & x2");
    BooleanExpression bool_expr4("x1 < x2 | x3");
    EXPECT_EQ(bool_expr4.zhegalkin(), "1 + x3 + x2 & x3 + x1 & x2 & x3");
}

TEST(BooleanExpressionToZhegalkin, ThreeOperationsTests) {
    BooleanExpression bool_expr1("x1 + x2 + x3 + x4");
    EXPECT_EQ(bool_expr1.zhegalkin(), "x4 + x3 + x2 + x1");
    BooleanExpression bool_expr2("(x1 > x2) & (x3 < x4)");
    EXPECT_EQ(bool_expr2.zhegalkin(),
              "1 + x4 + x3 & x4 + x1 + x1 & x4 + x1 & x3 & x4 + x1 & x2 + x1 & x2 & x4 + x1 & x2 & x3 & x4");
    BooleanExpression bool_expr3("x1 | x2 | (x3 v x4)");
    EXPECT_EQ(bool_expr3.zhegalkin(), "1 + x4 + x3 + x3 & x4 + x1 & x2 & x4 + x1 & x2 & x3 + x1 & x2 & x3 & x4");
    BooleanExpression bool_expr4("x1 & x2 v x2 & x3");
    EXPECT_EQ(bool_expr4.zhegalkin(), "x2 & x3 + x1 & x2 + x1 & x2 & x3");
}

TEST(BooleanExpressionIsFull, ExpectTrue1) {
    std::vector<BooleanExpression *> expr_array;
    expr_array.push_back(new BooleanExpression("x1 | x2"));
    EXPECT_EQ(isFullSystem(expr_array), true);
    for (size_t i = 0; i < expr_array.size(); ++i) {
        delete expr_array[i];
        expr_array[i] = nullptr;
    }
}

TEST(BooleanExpressionIsFull, ExpectTrue2) {
    std::vector<BooleanExpression *> expr_array;
    expr_array.push_back(new BooleanExpression("x1 + x2 + x3"));
    expr_array.push_back(new BooleanExpression("x1 v x2"));
    expr_array.push_back(new BooleanExpression("1"));
    expr_array.push_back(new BooleanExpression("0"));
    EXPECT_EQ(isFullSystem(expr_array), true);
    for (size_t i = 0; i < expr_array.size(); ++i) {
        delete expr_array[i];
        expr_array[i] = nullptr;
    }
}

TEST(BooleanExpressionIsFull, ExpectFalse1) {
    std::vector<BooleanExpression *> expr_array;
    expr_array.push_back(new BooleanExpression("x1 & x2"));
    expr_array.push_back(new BooleanExpression("x1 v x3"));
    EXPECT_EQ(isFullSystem(expr_array), false);
    for (size_t i = 0; i < expr_array.size(); ++i) {
        delete expr_array[i];
        expr_array[i] = nullptr;
    }
}

TEST(BooleanExpressionIsFull, ExpectFalse2) {
    std::vector<BooleanExpression *> expr_array;
    expr_array.push_back(new BooleanExpression("x1 & x2"));
    expr_array.push_back(new BooleanExpression("(x2 v x3)"));
    expr_array.push_back(new BooleanExpression("x4"));
    EXPECT_EQ(isFullSystem(expr_array), false);
    for (size_t i = 0; i < expr_array.size(); ++i) {
        delete expr_array[i];
        expr_array[i] = nullptr;
    }
}