#include "vm.h"
#include "instructions.h"
#include <string>

Vm::Vm(std::vector<uint8_t> program) {
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
    stacks.push_front(std::vector<uint8_t>());
    stack = &stacks.front();
    heaps.push_front(new uint8_t[HEAP_MEMORY]);
    heap = heaps.front();
}

void Vm::pop_frame() {
    stacks.pop_front();
    stack = &stacks.front();
    delete heaps.front();
    heaps.pop_front();
    heap = heaps.front();
}