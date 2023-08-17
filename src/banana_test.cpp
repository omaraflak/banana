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

}