#include <iostream>
#include <sstream>
#include "../lib/ast.h"
#include "../lib/scanner.h"
#include "../lib/parser.h"
#include "../lib/vm.h"
#include "gtest/gtest.h"

namespace {
std::string exe(const std::string& code) {
    std::vector<Token> tokens = scanner::scan(code.c_str());
    std::shared_ptr<AbstractSyntaxTree> root = parser::parse(tokens);
    std::stringstream ss;
    auto origin = std::cout.rdbuf(ss.rdbuf());
    Vm(ast::to_bytes(ast::to_instructions(root))).execute();
    std::cout.rdbuf(origin);
    return ss.str();
}

TEST(Print, Literal) {
  EXPECT_EQ("1\n", exe("print 1;"));
  EXPECT_EQ("-5\n", exe("print -5;"));
}

TEST(Print, Variable) {
  EXPECT_EQ("true\n", exe("bool x = true; print x;"));
  EXPECT_EQ("false\n", exe("bool x = false; print x;"));
  EXPECT_EQ("A\n", exe("char x = 65; print x;"));
  EXPECT_EQ("5\n", exe("short x = 5; print x;"));
  EXPECT_EQ("8\n", exe("int x = 8; print x;"));
  EXPECT_EQ("-13\n", exe("long x = -13; print x;"));
}

TEST(expression, BinaryOperators) {
  EXPECT_EQ("4\n", exe("print 1 + 3;"));
  EXPECT_EQ("-2\n", exe("print 1 - 3;"));
  EXPECT_EQ("6\n", exe("print 2 * 3;"));
  EXPECT_EQ("2\n", exe("print 6 / 3;"));
  EXPECT_EQ("2\n", exe("print 6 % 4;"));
  EXPECT_EQ("3\n", exe("print 1 ^ 2;"));
  EXPECT_EQ("6\n", exe("int x = 5; x++; print x;"));
  EXPECT_EQ("6\n", exe("int x = 5; x+=1; print x;"));
  EXPECT_EQ("12\n", exe("int x = 5; x+=7; print x;"));
  EXPECT_EQ("4\n", exe("int x = 5; x--; print x;"));
  EXPECT_EQ("4\n", exe("int x = 5; x-=1; print x;"));
  EXPECT_EQ("-2\n", exe("int x = 5; x-=7; print x;"));
  EXPECT_EQ("10\n", exe("int x = 5; x*=2; print x;"));
  EXPECT_EQ("3\n", exe("int x = 6; x/=2; print x;"));
  EXPECT_EQ("1\n", exe("int x = 1; x%=2; print x;"));
  EXPECT_EQ("3\n", exe("int x = 1; x^=2; print x;"));
}

TEST(Expression, OrderOfOperations) {
  EXPECT_EQ("16\n", exe("print 1 + 3 * 5;"));
  EXPECT_EQ("20\n", exe("print (1 + 3) * 5;"));
  EXPECT_EQ("4\n", exe("print (1 + 3) * 5 / 5;"));
  EXPECT_EQ("1\n", exe("print 1 + 6 % 2;"));
  EXPECT_EQ("3\n", exe("print 1 % 6 + 2;"));
  EXPECT_EQ("1\n", exe("print 1 % (6 + 2);"));
  EXPECT_EQ("-11\n", exe("print 1 + 6 - 9 * 2;"));
  EXPECT_EQ("-1\n", exe("print 0 - 1;"));
}

TEST(Expression, BooleanOperators) {
  EXPECT_EQ("false\n", exe("print 1 == 2;"));
  EXPECT_EQ("true\n", exe("print (1 + 3) * 5 == 20;"));
  EXPECT_EQ("true\n", exe("print 1 < 2;"));
  EXPECT_EQ("true\n", exe("print 2 > 0;"));
  EXPECT_EQ("true\n", exe("print 1 <= 1;"));
  EXPECT_EQ("true\n", exe("print 1 >= 1;"));
  EXPECT_EQ("true\n", exe("print 1 != 6;"));
  EXPECT_EQ("false\n", exe("print 1 != 1;"));
  EXPECT_EQ("true\n", exe("print true + true;"));
  EXPECT_EQ("false\n", exe("print false + false;"));
  EXPECT_EQ("true\n", exe("print true == true;"));
  EXPECT_EQ("true\n", exe("print false == false;"));
  EXPECT_EQ("false\n", exe("print true == false;"));
}

TEST(Expression, SubstituteVariable) {
  EXPECT_EQ("true\n", exe("int x = 1; print 1 == x;"));
  EXPECT_EQ("true\n", exe("int x = 1; print x == 1;"));
  EXPECT_EQ("false\n", exe("short x = 25; print x <= 7;"));
  EXPECT_EQ("0\n", exe("short x = 25; short y = 24; print x - y - 1;"));
}

TEST(Expression, CrossType) {
  EXPECT_EQ("2\n", exe("bool x = 1; char y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("bool x = 1; short y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("bool x = 1; int y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("bool x = 1; long y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("char x = 1; short y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("char x = 1; int y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("char x = 1; long y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("short x = 1; int y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("short x = 1; long y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("int x = 1; long y = 1; print x + y;"));
  EXPECT_EQ("2\n", exe("bool x = 1; char y = 1; char z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("bool x = 1; short y = 1; short z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("bool x = 1; int y = 1; int z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("bool x = 1; long y = 1; long z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("char x = 1; short y = 1; short z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("char x = 1; int y = 1; int z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("char x = 1; long y = 1; long z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("short x = 1; int y = 1; int z = x + y; print z;"));
  EXPECT_EQ("2\n", exe("short x = 1; long y = 1; long z = x + y; print z;"));
}

TEST(IfCondition, EvaluateCondition) {
  EXPECT_EQ("1\n", exe("if (1 == 1) { print 1; }"));
  EXPECT_EQ("", exe("if (1 == 2) { print 1; }"));
  EXPECT_EQ("1\n", exe("if (true) { print 1; }"));
  EXPECT_EQ("", exe("if (false) { print 1; }"));
  EXPECT_EQ("1\n", exe("bool x = true; if (x) { print 1; }"));
  EXPECT_EQ("1\n", exe("int x = 5; if (x == 5) { print 1; }"));
  EXPECT_EQ("1\n", exe("int x = 5; if (x >= 5) { print 1; }"));
}

TEST(IfElseCondition, EvaluateCondition) {
  EXPECT_EQ("1\n", exe("if (1 == 1) { print 1; } else { print 2; }"));
  EXPECT_EQ("2\n", exe("if (1 != 1) { print 1; } else { print 2; }"));
}

TEST(ForLoop, Loop) {
  EXPECT_EQ("0\n1\n2\n", exe("for (int i=0; i<3; i++) { print i; }"));
  EXPECT_EQ("0\n1\n2\n", exe("int i=1; for (i=0; i<3; i++) { print i; }"));
  EXPECT_EQ("0\n1\n2\n", exe("int i=1; for (i=0; i<3; i++) { print i; }"));
  EXPECT_EQ("3\n2\n1\n", exe("for (int i=3; i>0; i--) { print i; }"));
  EXPECT_EQ("0\n4\n8\n", exe("for (int i=0; i<10; i+=4) { print i; }"));
}

TEST(WhileLoop, Loop) {
  EXPECT_EQ("0\n1\n2\n", exe("long i=0; while (i < 3) { print i; i++; }"));
}

TEST(Function, CallAndReturn) {
  EXPECT_EQ("13\n", exe("int add(int a, int b) { return a + b; } int main() { int x = add(6, 7); print x; }"));
  EXPECT_EQ("13\n3\n7\n", exe("int add(int a, int b) { return a + b; } int main() { print add(6, 7); print add(1, 2); print add(3, 4); }"));
}

TEST(Function, NoParameters) {
  EXPECT_EQ("true\n", exe("int sayHello() { print true; return 1; } int main() { sayHello(); }"));
}
}