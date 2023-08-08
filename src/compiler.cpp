#include <iostream>
#include <vector>
#include "ast.h"

int main(int argc, char** argv) {
    BlockNode root(nullptr);
    PrintStringNode print(&root, "Hello, world!\n");
    HaltNode halt(&root);

    root.add(&print);
    root.add(&halt);

    for (auto pair : ast::to_asm(&root)) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
    std::cout << "------" << std::endl;

    Vm vm(ast::to_bytes(&root));
    vm.execute();
}