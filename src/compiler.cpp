#include <iostream>
#include <vector>
#include "../lib/ast.h"

std::shared_ptr<FunctionNode> make_fibonacci() {
    auto zero = std::shared_ptr<LiteralNode>(new LiteralNode(0));
    auto one = std::shared_ptr<LiteralNode>(new LiteralNode(1));
    auto two = std::shared_ptr<LiteralNode>(new LiteralNode(2));

    auto block = std::shared_ptr<BlockNode>(new BlockNode());
    auto n = std::shared_ptr<VariableNode>(new VariableNode(block));
    auto fib = std::shared_ptr<FunctionNode>(new FunctionNode(block, {n}));
    
    auto n_eq_zero = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(n, zero, AST_EQ));
    auto n_eq_one = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(n, one, AST_EQ));

    auto return_n = std::shared_ptr<ReturnNode>(new ReturnNode({n}));
    auto if_zero = std::shared_ptr<IfNode>(new IfNode(n_eq_zero, return_n));
    auto if_one = std::shared_ptr<IfNode>(new IfNode(n_eq_one, return_n));

    auto n_minus_one = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(n, one, AST_SUB));
    auto n_minus_two = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(n, two, AST_SUB));
    auto fib_n_minus_one = std::shared_ptr<CallNode>(new CallNode(fib, {n_minus_one}));
    auto fib_n_minus_two = std::shared_ptr<CallNode>(new CallNode(fib, {n_minus_two}));

    auto fib_result = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(fib_n_minus_one, fib_n_minus_two, AST_ADD));
    auto ret_result = std::shared_ptr<ReturnNode>(new ReturnNode({fib_result}));

    block->add(if_zero);
    block->add(if_one);
    block->add(ret_result);

    return fib;
}

int main(int argc, char** argv) {
    auto ten = std::shared_ptr<LiteralNode>(new LiteralNode(10));
    auto fib = make_fibonacci();

    auto main = std::shared_ptr<BlockNode>(new BlockNode());
    auto fib_ten = std::shared_ptr<CallNode>(new CallNode(fib, {ten}));
    auto print_fib_ten = std::shared_ptr<PrintNode>(new PrintNode(fib_ten));
    main->add(print_fib_ten);

    auto all = std::shared_ptr<BlockNode>(new BlockNode());
    all->set_main(main.get());
    all->add(fib);
    all->add(main);

    std::vector<std::unique_ptr<const Instruction>> instructions = ast::to_instructions(all);

    for (auto pair : ast::to_asm(instructions)) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
    std::cout << "------" << std::endl;

    Vm vm(ast::to_bytes(instructions));
    vm.execute();
}