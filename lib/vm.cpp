#include "vm.h"
#include "instructions.h"
#include <string>

Vm::Vm(const std::vector<uint8_t>& program) {
    this->program = program;
    ip = 0;
    running = true;
    push_frame();
}

void Vm::execute() {
    while (running) {
        uint8_t opcode = program[ip++];
        Instruction* instruction = Instruction::from_opcode(opcode);
        instruction->read(program, &ip);
        instruction->execute(*this);
        delete instruction;
    }
}

void Vm::push_frame() {
    stacks.push(std::vector<uint8_t>());
    stack = &stacks.top();
    heaps.push(new uint8_t[HEAP_MEMORY]);
    heap = heaps.top();
}

void Vm::pop_frame() {
    stacks.pop();
    stack = &stacks.top();
    delete heaps.top();
    heaps.pop();
    heap = heaps.top();
}