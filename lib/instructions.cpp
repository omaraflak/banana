#include "instructions.h"
#include <sstream>
#include <map>

namespace instructions {
template <class K, class V> 
std::map<V, K> reverse_map(const std::map<K, V>& m) {
    std::map<V, K> reversed;
    for (auto pair : m) {
        reversed[pair.second] = pair.first;
    }
    return reversed;
}

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

const std::map<uint8_t, std::string> OP_STRINGS = {
    {OP_ADD, "add"},
    {OP_SUB, "sub"},
    {OP_MUL, "mul"},
    {OP_DIV, "div"},
    {OP_MOD, "mod"},
    {OP_AND, "and"},
    {OP_OR, "or"},
    {OP_XOR, "xor"},
    {OP_NOT, "not"},
    {OP_PUSH, "push"},
    {OP_JUMP, "jump"},
    {OP_JUMP_IF, "jump_if"},
    {OP_JUMP_IF_FALSE, "jump_if_false"},
    {OP_CALL, "call"},
    {OP_RET, "ret"},
    {OP_DUP, "dup"},
    {OP_SWAP, "swap"},
    {OP_LT, "lt"},
    {OP_LTE, "lte"},
    {OP_GT, "gt"},
    {OP_GTE, "gte"},
    {OP_EQ, "eq"},
    {OP_NOT_EQ, "not_eq"},
    {OP_BOOL_AND, "b_and"},
    {OP_BOOL_OR, "b_or"},
    {OP_BOOL_NOT, "b_not"},
    {OP_PRINT, "print"},
    {OP_PRINT_C, "printc"},
    {OP_STORE, "store"},
    {OP_LOAD, "load"},
    {OP_HALT, "halt"},
};

const std::map<std::string, uint8_t> OP_STRINGS_REV = instructions::reverse_map(OP_STRINGS);

Instruction::Instruction(const uint8_t& opcode) {
    this->opcode = opcode;
}

void Instruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {}

void Instruction::write(std::vector<uint8_t>& buffer) const {
    buffer.push_back(opcode);
}

void Instruction::execute(Vm& vm) const {}

void Instruction::read_string(const std::vector<std::string>& strings) {}

std::string Instruction::to_string() const {
    return OP_STRINGS.at(opcode);
}

uint8_t Instruction::size() const {
    return SIZE_OF_BYTE;
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
        case OP_BOOL_AND:
            return new BoolAndInstruction();
        case OP_BOOL_OR:
            return new BoolOrInstruction();
        case OP_BOOL_NOT:
            return new BoolNotInstruction();
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

Instruction* Instruction::from_opstring(const std::string& opstring) {
    return Instruction::from_opcode(OP_STRINGS_REV.at(opstring));
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

void PushInstruction::write(std::vector<uint8_t>& buffer) const {
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
    std::stringstream ss;
    ss << Instruction::to_string() << " " << value;
    return ss.str();
}

uint8_t PushInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
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

void JumpInstruction::write(std::vector<uint8_t>& buffer) const {
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
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address;
    return ss.str();
}

uint8_t JumpInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

void JumpInstruction::set_address(const uint64_t& address) {
    this->address = address;
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

CallInstruction::CallInstruction(const uint64_t& address, const uint8_t& param_count) : Instruction(OP_CALL) {
    this->address = address;
    this->param_count = param_count;
}

void CallInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
    param_count = bytes::read_byte(buffer, *index);
    *index += SIZE_OF_BYTE;
}

void CallInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
    bytes::push_byte(buffer, param_count);
}

void CallInstruction::execute(Vm& vm) const {
    vm.call_stack.push_back(vm.ip);
    vm.ip = address;
    std::vector<uint8_t>& old_stack = *vm.stack;
    vm.new_frame();
    for (int i = 0; i < param_count; i++) {
        bytes::push_long(*vm.stack, bytes::pop_long(old_stack));
    }
}

void CallInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
    param_count = stol(strings[1]);
}

std::string CallInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address << " " << (int) param_count;
    return ss.str();
}

uint8_t CallInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG + SIZE_OF_BYTE;
}

RetInstruction::RetInstruction() : Instruction(OP_RET) {}

RetInstruction::RetInstruction(const uint8_t& values_count) : Instruction(OP_RET) {
    this->values_count = values_count;
}

void RetInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    values_count = bytes::read_byte(buffer, *index);
    *index += SIZE_OF_BYTE;
}

void RetInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    bytes::push_byte(buffer, values_count);
}

void RetInstruction::execute(Vm& vm) const {
    vm.ip = vm.call_stack.back();
    vm.call_stack.pop_back();
    std::vector<uint64_t> values;
    for (int i = 0; i < values_count; i++) {
        values.push_back(bytes::pop_long(*vm.stack));
    }
    vm.pop_frame();
    for (auto value : values) {
        bytes::push_long(*vm.stack, value);
    }
}

void RetInstruction::read_string(const std::vector<std::string>& strings) {
    values_count = stol(strings[0]);
}

std::string RetInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << (int) values_count;
    return ss.str();
}

uint8_t RetInstruction::size() const {
    return Instruction::size() + SIZE_OF_BYTE;
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

BoolInstruction::BoolInstruction(const uint8_t& opcode, std::function<uint8_t(const bool&, const bool&)> operation) : Instruction(opcode) {
    this->operation = operation;
}

void BoolInstruction::execute(Vm& vm) const {
    uint8_t right = bytes::pop_byte(*vm.stack);
    uint8_t left = bytes::pop_byte(*vm.stack);
    bytes::push_byte(*vm.stack, operation(left, right));   
}

BoolAndInstruction::BoolAndInstruction() : BoolInstruction(OP_BOOL_AND, bytes::bool_and) {}

BoolOrInstruction::BoolOrInstruction() : BoolInstruction(OP_BOOL_OR, bytes::bool_or) {}

BoolNotInstruction::BoolNotInstruction() : Instruction(OP_BOOL_NOT) {}

void BoolNotInstruction::execute(Vm& vm) const {
    bool value = bytes::pop_byte(*vm.stack);
    bytes::push_byte(*vm.stack, !value);
}

PrintInstruction::PrintInstruction() : Instruction(OP_PRINT) {}

void PrintInstruction::execute(Vm& vm) const {
    std::cout << (int64_t) bytes::pop_long(*vm.stack);
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

void StoreInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
}

void StoreInstruction::execute(Vm& vm) const {
    bytes::write_long(*vm.heap, address, bytes::pop_long(*vm.stack));
}

void StoreInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
}

std::string StoreInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address;
    return ss.str();
}

uint8_t StoreInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

LoadInstruction::LoadInstruction() : Instruction(OP_LOAD) {}

LoadInstruction::LoadInstruction(const uint64_t& address) : Instruction(OP_LOAD) {
    this->address = address;
}

void LoadInstruction::read(const std::vector<uint8_t>& buffer, uint64_t* index) {
    address = bytes::read_long(buffer, *index);
    *index += SIZE_OF_LONG;
}

void LoadInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    bytes::push_long(buffer, address);
}

void LoadInstruction::execute(Vm& vm) const {
    bytes::push_long(*vm.stack, bytes::read_long(*vm.heap, address));
}

void LoadInstruction::read_string(const std::vector<std::string>& strings) {
    address = stol(strings[0]);
}

std::string LoadInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address;
    return ss.str();
}

uint8_t LoadInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

HaltInstruction::HaltInstruction() : Instruction(OP_HALT) {}

void HaltInstruction::execute(Vm& vm) const {
    std::cout << "Halt!" << std::endl;
    vm.running = false;
}