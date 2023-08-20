#include "vm.h"
#include "instructions.h"
#include <string>
#include <cassert>
#include <dlfcn.h>
#include <iostream>
#include <functional>

Vm::~Vm() {
    c_functions.clear();
    for (auto handle : handles) {
        dlclose(handle);
    }
}

Vm::Vm(const std::vector<uint8_t>& program, const std::vector<std::string>& shared_libraries) {
    this->program = program;
    ip = 0;
    running = true;
    push_frame();
    load_libraries(shared_libraries);
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

void Vm::load_libraries(const std::vector<std::string>& shared_libraries) {
    std::hash<std::string> hasher;
    for (auto path : shared_libraries) {
        void* handle = dlopen(path.c_str(), RTLD_NOW);
        if (handle == nullptr) {
            std::cout << dlerror() << std::endl;
            exit(1);
        }
        handles.push_back(handle);
        void* ptr = dlsym(handle, "get_classes");
        if (ptr == nullptr) {
            std::cout << dlerror() << std::endl;
            exit(1);
        }
        std::function<std::vector<CFunction*>()> get_classes = reinterpret_cast<std::vector<CFunction*>(*)()>(ptr);
        for (auto c_function_ptr : get_classes()) {
            c_functions[hasher(c_function_ptr->get_name())] = std::shared_ptr<CFunction>(c_function_ptr);
        }
    }
}
