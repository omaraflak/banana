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
}

Vm::Vm(std::vector<uint8_t> program) {
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
    if (debug) {
        vm::print(program, "program");
    }
    while (running) {
        uint8_t opcode = program[ip++];
        if (debug) {
            std::cout << "opcode: " << (int) opcode << std::endl;
            vm::print(stack, "stack");
            vm::print(call_stack, "call stack");
            std::cout << "Press any key to continue...\n";
            getchar();
        }
        Instruction* instruction = Instruction::from_opcode(opcode);
        instruction->read(program.data(), &ip);
        instruction->execute(*this);
        delete instruction;
    }
}