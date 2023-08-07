#include "instructions.h"
#include <sstream>

Instruction::Instruction(const uint8_t& opcode) {
    this->opcode = opcode;
}

void Instruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {}

void Instruction::write(std::vector<uint8_t>& buffer) {
    buffer.push_back(opcode);
}

void Instruction::execute(Vm& vm) const {}

void Instruction::read_string(const std::vector<std::string>& strings) {}

std::string Instruction::to_string() const {
    return OP_STRINGS.at(opcode);
}

Instruction* Instruction::from_opcode(const uint8_t& opcode) {
    switch (opcode) {
        case OP_ADD:
            return new AddInstruction();
        case OP_SUB:
            return new SubInstruction();
        case OP_MUL:
            return new MulInstruction();
        case OP_DIV:
            return new DivInstruction();
        case OP_MOD:
            return new ModInstruction();
        case OP_AND:
            return new AndInstruction();
        case OP_OR:
            return new OrInstruction();
        case OP_XOR:
            return new XorInstruction();
        case OP_NOT:
            return new NotInstruction();
        case OP_PUSH:
            return new PushInstruction();
        case OP_JUMP:
            return new JumpInstruction();
        case OP_JUMP_IF:
            return new JumpIfInstruction();
        case OP_JUMP_IF_FALSE:
            return new JumpIfFalseInstruction();
        case OP_CALL:
            return new CallInstruction();
        case OP_RET:
            return new RetInstruction();
        case OP_DUP:
            return new DupInstruction();
        case OP_SWAP:
            return new SwapInstruction();
        case OP_LT:
            return new LtInstruction();
        case OP_LTE:
            return new LteInstruction();
        case OP_GT:
            return new GtInstruction();
        case OP_GTE:
            return new GteInstruction();
        case OP_EQ:
            return new EqInstruction();
        case OP_NOT_EQ:
            return new NotEqInstruction();
        case OP_PRINT:
            return new PrintInstruction();
        case OP_PRINT_C:
            return new PrintCharInstruction();
        case OP_STORE:
            return new StoreInstruction();
        case OP_LOAD:
            return new LoadInstruction();
        case OP_HALT:
            return new HaltInstruction();
    }
    std::cout << "Opcode not recognized: " << (int) opcode << std::endl;
    exit(1);
}

namespace instructions {
std::vector<std::string> split_string(const std::string& str, const char& separator) {
    std::stringstream ss(str);
    std::vector<std::string> result;
    std::string s;
    while (getline(ss, s, separator)) {
        result.push_back(s);
    }
    return result;
}
}

Instruction* Instruction::from_string(const std::string& str) {
    std::string opstring = str;
    int index = str.find(" ");
    if (index != -1) {
        opstring = str.substr(0, index);
    }
    std::string rest = index == -1 ? "" : str.substr(index + 1);
    uint8_t opcode = OP_STRINGS_REV.at(opstring);
    Instruction* instruction = Instruction::from_opcode(opcode);
    instruction->read_string(instructions::split_string(rest, ' '));
    return instruction;
}

BinaryOperationInstruction::BinaryOperationInstruction(const uint8_t& opcode, std::function<uint64_t(const uint64_t&, const uint64_t&)> operation) : Instruction(opcode) {
    this->operation = operation;
}

void BinaryOperationInstruction::execute(Vm& vm) const {
    uint64_t right = bytes::pop_long(*vm.stack);
    uint64_t left = bytes::pop_long(*vm.stack);
    bytes::push_long(*vm.stack, operation(left, right));
}

AddInstruction::AddInstruction() : BinaryOperationInstruction(OP_ADD, bytes::add_long) {}

SubInstruction::SubInstruction() : BinaryOperationInstruction(OP_SUB, bytes::sub_long) {}

MulInstruction::MulInstruction() : BinaryOperationInstruction(OP_MUL, bytes::mul_long) {}

DivInstruction::DivInstruction() : BinaryOperationInstruction(OP_DIV, bytes::div_long) {}

ModInstruction::ModInstruction() : BinaryOperationInstruction(OP_MOD, bytes::mod_long) {}

AndInstruction::AndInstruction() : BinaryOperationInstruction(OP_AND, bytes::and_long) {}

OrInstruction::OrInstruction() : BinaryOperationInstruction(OP_OR, bytes::or_long) {}

XorInstruction::XorInstruction() : BinaryOperationInstruction(OP_XOR, bytes::xor_long) {}

UnaryOperationInstruction::UnaryOperationInstruction(const uint8_t& opcode, std::function<uint64_t(const uint64_t&)> operation) : Instruction(opcode) {
    this->operation = operation;
}

void UnaryOperationInstruction::execute(Vm& vm) const {
    bytes::push_long(*vm.stack, operation(bytes::pop_long(*vm.stack)));
}

NotInstruction::NotInstruction() : UnaryOperationInstruction(OP_NOT, bytes::not_long) {}

PushInstruction::PushInstruction() : Instruction(OP_PUSH) {}

PushInstruction::PushInstruction(const uint64_t& value) : Instruction(OP_PUSH) {
    this->value = value;
}

void PushInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    value = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void PushInstruction::write(std::vector<uint8_t>& buffer) {
    Instruction::write(buffer);
    bytes::push_long(buffer, value);
}
    
void PushInstruction::execute(Vm& vm) const {
    bytes::push_long(*vm.stack, value);
}

void PushInstruction::read_string(const std::vector<std::string>& strings) {
    value = stol(strings[0]);
}

std::string PushInstruction::to_string() const {
    std::stringstream ss(Instruction::to_string());
    ss << " " << value;
    return ss.str();
}

JumpInstruction::JumpInstruction() : Instruction(OP_JUMP) {}

JumpInstruction::JumpInstruction(const uint64_t& address) : Instruction(OP_JUMP) {
    this->address = address;
}

JumpInstruction::JumpInstruction(const uint8_t& opcode) : Instruction(opcode) {}

JumpInstruction::JumpInstruction(const uint8_t& opcode, const uint64_t& address) : Instruction(opcode) {
    this->address = address;
}

void JumpInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void JumpInstruction::write(std::vector<uint8_t>& buffer) {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
}

void JumpInstruction::execute(Vm& vm) const {
    vm.ip = address;
}

void JumpInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
}

std::string JumpInstruction::to_string() const {
    std::stringstream ss(Instruction::to_string());
    ss << " " << address;
    return ss.str();
}

JumpIfInstruction::JumpIfInstruction() : JumpInstruction((uint8_t) OP_JUMP_IF) {}

JumpIfInstruction::JumpIfInstruction(const uint64_t& address) : JumpInstruction(OP_JUMP_IF, address) {}

void JumpIfInstruction::execute(Vm& vm) const {
    if (bytes::pop_byte(*vm.stack) == 1) {
        vm.ip = address;
    }
}

JumpIfFalseInstruction::JumpIfFalseInstruction() : JumpInstruction((uint8_t) OP_JUMP_IF_FALSE) {}

JumpIfFalseInstruction::JumpIfFalseInstruction(const uint64_t& address) : JumpInstruction(OP_JUMP_IF_FALSE, address) {}

void JumpIfFalseInstruction::execute(Vm& vm) const {
    if (bytes::pop_byte(*vm.stack) == 0) {
        vm.ip = address;
    }
}

CallInstruction::CallInstruction() : Instruction(OP_CALL) {}

CallInstruction::CallInstruction(const uint64_t& address, const uint64_t& param_count) : Instruction(OP_CALL) {
    this->address = address;
    this->param_count = param_count;
}

void CallInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
    param_count = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void CallInstruction::write(std::vector<uint8_t>& buffer) {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
    bytes::push_long(buffer, param_count);
}

void CallInstruction::execute(Vm& vm) const {
    vm.call_stack.push_back(vm.ip);
    vm.ip = address;
    std::vector<uint8_t>* old_stack = vm.stack;
    vm.new_stack();
    for (int i = 0; i < param_count; i++) {
        bytes::push_long(*vm.stack, bytes::pop_long(*old_stack));
    }
}

void CallInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
    param_count = stol(strings[1]);
}

std::string CallInstruction::to_string() const {
    std::stringstream ss(Instruction::to_string());
    ss << " " << address;
    return ss.str();
}

RetInstruction::RetInstruction() : Instruction(OP_RET) {}

void RetInstruction::execute(Vm& vm) const {
    vm.ip = vm.call_stack.back();
    vm.call_stack.pop_back();
    uint64_t value = bytes::pop_long(*vm.stack);
    vm.pop_stack();
    bytes::push_long(*vm.stack, value);
}

DupInstruction::DupInstruction() : Instruction(OP_DUP) {}

void DupInstruction::execute(Vm& vm) const {
    uint64_t value = bytes::read_long(*vm.stack, vm.stack->size() - SIZE_OF_LONG);
    bytes::push_long(*vm.stack, value);
}

SwapInstruction::SwapInstruction() : Instruction(OP_SWAP) {}

void SwapInstruction::execute(Vm& vm) const {
    uint64_t a = bytes::pop_long(*vm.stack);
    uint64_t b = bytes::pop_long(*vm.stack);
    bytes::push_long(*vm.stack, a);
    bytes::push_long(*vm.stack, b);
}

CompareInstruction::CompareInstruction(const uint8_t& opcode, std::function<uint8_t(const uint64_t&, const uint64_t&)> operation) : Instruction(opcode) {
    this->operation = operation;
}

void CompareInstruction::execute(Vm& vm) const {
    uint64_t right = bytes::pop_long(*vm.stack);
    uint64_t left = bytes::pop_long(*vm.stack);
    bytes::push_byte(*vm.stack, operation(left, right));
}

LtInstruction::LtInstruction() : CompareInstruction(OP_LT, bytes::lt_long) {}

LteInstruction::LteInstruction() : CompareInstruction(OP_LTE, bytes::lte_long) {}

GtInstruction::GtInstruction() : CompareInstruction(OP_GT, bytes::gt_long) {}

GteInstruction::GteInstruction() : CompareInstruction(OP_GTE, bytes::gte_long) {}

EqInstruction::EqInstruction() : CompareInstruction(OP_EQ, bytes::eq_long) {}

NotEqInstruction::NotEqInstruction() : CompareInstruction(OP_NOT_EQ, bytes::not_eq_long) {}

PrintInstruction::PrintInstruction() : Instruction(OP_PRINT) {}

void PrintInstruction::execute(Vm& vm) const {
    std::cout << bytes::pop_long(*vm.stack);
}

PrintCharInstruction::PrintCharInstruction() : Instruction(OP_PRINT_C) {}

void PrintCharInstruction::execute(Vm& vm) const {
    std::cout << (char) bytes::pop_long(*vm.stack);
}

StoreInstruction::StoreInstruction() : Instruction(OP_STORE) {}

StoreInstruction::StoreInstruction(const uint64_t& address) : Instruction(OP_STORE) {
    this->address = address;
}

void StoreInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void StoreInstruction::write(std::vector<uint8_t>& buffer) {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
}

void StoreInstruction::execute(Vm& vm) const {
    bytes::write_long(vm.memory, address, bytes::pop_long(*vm.stack));
}

void StoreInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
}

std::string StoreInstruction::to_string() const {
    std::stringstream ss(Instruction::to_string());
    ss << " " << address;
    return ss.str();
}

LoadInstruction::LoadInstruction() : Instruction(OP_LOAD) {}

LoadInstruction::LoadInstruction(const uint64_t& address) : Instruction(OP_LOAD) {
    this->address = address;
}

void LoadInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void LoadInstruction::write(std::vector<uint8_t>& buffer) {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
}

void LoadInstruction::execute(Vm& vm) const {
    bytes::push_long(*vm.stack, bytes::read_long(vm.memory, address));
}

void LoadInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
}

std::string LoadInstruction::to_string() const {
    std::stringstream ss(Instruction::to_string());
    ss << " " << address;
    return ss.str();
}

HaltInstruction::HaltInstruction() : Instruction(OP_HALT) {}

void HaltInstruction::execute(Vm& vm) const {
    std::cout << "Halt!" << std::endl;
    vm.running = false;
}