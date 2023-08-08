#include <iostream>
#include <vector>
#include "ast.h"


int main(int argc, char** argv) {
    LiteralNode zero(0);
    LiteralNode one(1);
    LiteralNode two(2);
    LiteralNode ten(10);

    BlockNode block;
    VariableNode n(&block);
    FunctionNode fib(&block, {&n});

    BinaryOperationNode n_eq_zero(&n, &zero, EQ);
    BinaryOperationNode n_eq_one(&n, &one, EQ);

    ReturnNode return_n(&n);
    IfNode if_zero(&n_eq_zero, &return_n);
    IfNode if_one(&n_eq_one, &return_n);

    BinaryOperationNode n_minus_one(&n, &one, SUB);
    BinaryOperationNode n_minus_two(&n, &two, SUB);

    CallNode fib_n_minus_one(&fib, {&n_minus_one});
    CallNode fib_n_minus_two(&fib, {&n_minus_two});

    BinaryOperationNode fib_result(&fib_n_minus_one, &fib_n_minus_two, ADD);
    ReturnNode ret_result(&fib_result);

    block.add(&if_zero);
    block.add(&if_one);
    block.add(&ret_result);

    BlockNode main;
    CallNode fib_ten(&fib, {&ten});
    PrintNode print_fib_ten(&fib_ten);
    main.add(&print_fib_ten);

    BlockNode all;
    all.add(&fib);
    all.add(&main);


    // assemble

    std::vector<std::unique_ptr<const Instruction>> instructions = ast::to_instructions(&all, &main);

    for (auto pair : ast::to_asm(instructions)) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
    std::cout << "------" << std::endl;

    Vm vm(ast::to_bytes(instructions));
    vm.execute();
}