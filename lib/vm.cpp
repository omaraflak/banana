#include "vm.h"
#include "instructions.h"
#include <string>
#include <dlfcn.h>
#include <iostream>
#include <functional>

namespace vm {
Instruction* OP_INSTANCES[OP_OPERATIONS_COUNT];

// Must be ordered like OP_* enum
void init_instances() {
    int i = 0;
    OP_INSTANCES[i++] = new AddInstruction();
    OP_INSTANCES[i++] = new SubInstruction();
    OP_INSTANCES[i++] = new MulInstruction();
    OP_INSTANCES[i++] = new DivInstruction();
    OP_INSTANCES[i++] = new ModInstruction();
    OP_INSTANCES[i++] = new XorInstruction();
    OP_INSTANCES[i++] = new BinaryAndInstruction();
    OP_INSTANCES[i++] = new BinaryOrInstruction();
    OP_INSTANCES[i++] = new BinaryNotInstruction();
    OP_INSTANCES[i++] = new PushInstruction();
    OP_INSTANCES[i++] = new JumpInstruction();
    OP_INSTANCES[i++] = new JumpIfInstruction();
    OP_INSTANCES[i++] = new JumpIfFalseInstruction();
    OP_INSTANCES[i++] = new CallInstruction();
    OP_INSTANCES[i++] = new RetInstruction();
    OP_INSTANCES[i++] = new LtInstruction();
    OP_INSTANCES[i++] = new LteInstruction();
    OP_INSTANCES[i++] = new GtInstruction();
    OP_INSTANCES[i++] = new GteInstruction();
    OP_INSTANCES[i++] = new EqInstruction();
    OP_INSTANCES[i++] = new NotEqInstruction();
    OP_INSTANCES[i++] = new BooleanAndInstruction();
    OP_INSTANCES[i++] = new BooleanOrInstruction();
    OP_INSTANCES[i++] = new BooleanNotInstruction();
    OP_INSTANCES[i++] = new PrintInstruction();
    OP_INSTANCES[i++] = new StoreInstruction();
    OP_INSTANCES[i++] = new LoadInstruction();
    OP_INSTANCES[i++] = new ConvertInstruction();
    OP_INSTANCES[i++] = new NativeInstruction();
    OP_INSTANCES[i++] = new HaltInstruction();
}

void free_instances() {
    for (const auto& instance : OP_INSTANCES) {
        delete instance;
    }
}
}

Vm::~Vm() {
    vm::free_instances();
}

Vm::Vm(const std::vector<uint8_t>& program, const std::vector<std::string>& shared_libraries) {
    this->program = program;
    ip = 0;
    running = true;
    push_frame();
    c_functions.load(shared_libraries);
    vm::init_instances();
}

void Vm::execute() {
    while (running) {
        uint8_t opcode = program[ip++];
        auto instruction = vm::OP_INSTANCES[opcode];
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
