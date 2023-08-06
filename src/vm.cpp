#include "vm.h"
#include "instructions.h"
#include <string>

template <class T>
void print(const std::vector<T>& stack, const std::string& name) {
    std::cout << name << ": ";
    for (T byte : stack) {
        std::cout << (int) byte << " ";
    }
    std::cout << std::endl;
}

Vm::Vm(uint8_t* program) {
    this->program = program;
    this->memory = new uint8_t[MAX_MEMORY];
    this->ip = 0;
    this->running = true;
    this->debug = false;
}

Vm::~Vm() {
    delete this->memory;
}

void Vm::execute() {
    while (running) {
        uint8_t opcode = program[ip++];
        if (debug) {
            std::cout << "opcode: " << (int) opcode << std::endl;
            print(stack, "stack");
            print(call_stack, "call stack");
            std::cout << "Press any key to continue...\n";
            getchar();
        }
        Instruction* instruction = Instruction::get_instruction(opcode);
        instruction->read(program, &ip);
        instruction->execute(*this);
        delete instruction;
    }
}