#include "vm.h"
#include "instructions.h"
#include <string>

namespace vm {
template <class T>
void print(const std::vector<T>& stack, const std::string& name) {
    std::cout << name << ": ";
    for (T byte : stack) {
        std::cout << (int) byte << " ";
    }
    std::cout << std::endl;
}

void print_debug(Vm& vm) {
    vm.stacks.reverse();
    int indent = 0;
    for (auto stack : vm.stacks) {
        std::cout << std::string(indent, ' ');
        print(stack, "stack");
        indent++;
    }
    vm.stacks.reverse();
    print(vm.call_stack, "call stack");
    std::cout << "Press any key to continue...\n";
    getchar();
}
}

Vm::Vm(std::vector<uint8_t> program) {
    this->program = program;
    ip = 0;
    running = true;
    debug = false;
    new_frame();
}

void Vm::execute() {
    if (debug) {
        vm::print(program, "program");
    }
    while (running) {
        uint8_t opcode = program[ip++];
        if (debug) {
            std::cout << "opcode: " << (int) opcode << std::endl;
            vm::print_debug(*this);
        }
        Instruction* instruction = Instruction::from_opcode(opcode);
        instruction->read(program, &ip);
        instruction->execute(*this);
        delete instruction;
    }
}

void Vm::new_frame() {
    stacks.push_front(std::vector<uint8_t>());
    stack = &stacks.front();
    heaps.push_front(std::vector<uint8_t>(MAX_MEMORY));
    heap = &heaps.front();
}

void Vm::pop_frame() {
    stacks.pop_front();
    stack = &stacks.front();
    heaps.pop_front();
    heap = &heaps.front();
}