#include <iostream>
#include <vector>
#include "vm.h"
#include "instructions.h"

std::vector<Instruction*> create_program() {
    // return {
    //     new PushInstruction(400),
    //     new PushInstruction(500),
    //     new AddInstruction(),
    //     new PrintInstruction(),
    //     new PushInstruction(10),
    //     new PrintCharInstruction(),
    //     new HaltInstruction()
    // };
    return {
        new PushInstruction(50),
        new PushInstruction(20),
        new CallInstruction(40, 2),
        new PrintInstruction(),
        new PushInstruction(10),
        new PrintCharInstruction(),
        new HaltInstruction(),
        new AddInstruction(),
        new RetInstruction(),
    };
}

void delete_program(std::vector<Instruction*>& instructions) {
    for (auto i : instructions) {
        delete i;
    }
}

void write_program(std::vector<uint8_t>& program) {
    std::vector<Instruction*> instructions = create_program();
    for (auto i : instructions) {
        i->write(program);
    }
    delete_program(instructions);
}

int main(int argc, char** argv) {
    std::vector<uint8_t> program;
    write_program(program);
    Vm vm(program.data());
    // vm.debug = true;
    vm.execute();
    return 0;
}