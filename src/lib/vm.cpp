#include "vm.h"
#include "instructions.h"
#include <string>
#include <dlfcn.h>
#include <iostream>
#include <functional>

Vm::Vm(const std::vector<uint8_t>& program, const std::vector<std::string>& shared_libraries) {
    this->program = program;
    ip = 0;
    running = true;
    push_frame();
    c_functions.load(shared_libraries);
}

void Vm::execute() {
    while (running) {
        uint8_t opcode = program[ip++];
        auto instruction = Instruction::OP_INSTANCES_PTR[opcode];
        instruction->read(program, &ip);
        instruction->execute(*this);
    }
}

void Vm::push_frame() {
    stacks.push(std::vector<Var>());
    stack = &stacks.top();
    heaps.push(new Var[HEAP_MEMORY]);
    heap = heaps.top();
}

void Vm::pop_frame() {
    stacks.pop();
    stack = &stacks.top();
    delete heaps.top();
    heaps.pop();
    heap = heaps.top();
}
