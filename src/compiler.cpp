#include <iostream>
#include <vector>
#include "ast.h"

int main(int argc, char** argv) {
    BlockNode root(nullptr);
    LiteralNode five(5);
    LiteralNode six(6);
    LiteralNode eleven(11);
    VariableNode a(&root);
    VariableNode b(&root);
    AssignNode assign1(&a, &five);
    AssignNode assign2(&b, &six);
    BinaryOperationNode add(&a, &b, ADD);
    BinaryOperationNode check(&add, &eleven, EQ);
    PrintNode print(&add);
    IfNode branch(&check, &print, nullptr);
    HaltNode halt;

    root.add(&assign1);
    root.add(&assign2);
    root.add(&branch);
    root.add(&halt);


    // BlockNode root(nullptr);

    for (auto pair : ast::to_asm(&root)) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
    std::cout << "------" << std::endl;

    Vm vm(ast::to_bytes(&root));
    vm.execute();
}