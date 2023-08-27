#include "instructions.h"
#include "maputils.h"
#include "byteutils.h"
#include "c_interface.h"
#include "c_functions.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <ffi.h>
#include <cmath>
#include <map>

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

Var pop_var(std::vector<Var>* vars) {
    Var var = vars->back();
    vars->pop_back();
    return var;
}

const std::map<cinterface::ArgType, var::DataType> C_TYPE_TO_DATA_TYPE {
    {cinterface::BOOL, var::BOOL},
    {cinterface::CHAR, var::CHAR},
    {cinterface::INT, var::INT},
    {cinterface::LONG, var::LONG},
};

const std::map<uint8_t, std::string> OP_STRINGS = {
    {OP_ADD, "add"},
    {OP_SUB, "sub"},
    {OP_MUL, "mul"},
    {OP_DIV, "div"},
    {OP_MOD, "mod"},
    {OP_XOR, "xor"},
    {OP_BINARY_AND, "bin_and"},
    {OP_BINARY_OR, "bin_or"},
    {OP_BINARY_NOT, "bin_not"},
    {OP_PUSH, "push"},
    {OP_JUMP, "jump"},
    {OP_JUMP_IF, "jump_if"},
    {OP_JUMP_IF_FALSE, "jump_if_false"},
    {OP_CALL, "call"},
    {OP_RET, "ret"},
    {OP_LT, "lt"},
    {OP_LTE, "lte"},
    {OP_GT, "gt"},
    {OP_GTE, "gte"},
    {OP_EQ, "eq"},
    {OP_NOT_EQ, "not_eq"},
    {OP_BOOLEAN_AND, "bool_and"},
    {OP_BOOLEAN_OR, "bool_or"},
    {OP_BOOLEAN_NOT, "bool_not"},
    {OP_PRINT, "print"},
    {OP_STORE, "store"},
    {OP_LOAD, "load"},
    {OP_CONVERT, "convert"},
    {OP_NATIVE, "native"},
    {OP_HALT, "halt"},
};

const std::map<std::string, uint8_t> OP_STRINGS_REV = maputils::reverse(OP_STRINGS);
}

Instruction::Instruction(const uint8_t& opcode) {
    this->opcode = opcode;
}

Instruction::~Instruction() {}

void Instruction::read(const std::vector<uint8_t>& buffer, Address* index) {}

void Instruction::write(std::vector<uint8_t>& buffer) const {
    buffer.push_back(opcode);
}

void Instruction::execute(Vm& vm) const {}

void Instruction::read_string(const std::vector<std::string>& strings) {}

std::string Instruction::to_string() const {
    return instructions::OP_STRINGS.at(opcode);
}

uint8_t Instruction::size() const {
    return SIZE_OF_BYTE;
}

std::shared_ptr<Instruction> Instruction::from_opcode(const uint8_t& opcode) {
    if (opcode < OP_OPERATIONS_COUNT) {
        return instructions::INSTANCES[opcode];
    }
    std::cout << "Opcode not recognized: " << (int) opcode << std::endl;
    exit(1);
}

std::shared_ptr<Instruction> Instruction::from_opstring(const std::string& opstring) {
    return Instruction::from_opcode(instructions::OP_STRINGS_REV.at(opstring));
}

std::shared_ptr<Instruction> Instruction::from_string(const std::string& str) {
    std::vector<std::string> parts = instructions::split_string(str, ' ');
    auto instruction = Instruction::from_opcode(instructions::OP_STRINGS_REV.at(parts[0]));
    instruction->read_string(std::vector<std::string>(parts.begin() + 1, parts.end()));
    return instruction;
}

AddInstruction::AddInstruction() : Instruction(OP_ADD) {}

void AddInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::add(left, right));
}

SubInstruction::SubInstruction() : Instruction(OP_SUB) {}

void SubInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::sub(left, right));
}

MulInstruction::MulInstruction() : Instruction(OP_MUL) {}

void MulInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::mul(left, right));
}

DivInstruction::DivInstruction() : Instruction(OP_DIV) {}

void DivInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::div(left, right));
}

ModInstruction::ModInstruction() : Instruction(OP_MOD) {}

void ModInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::mod(left, right));
}

XorInstruction::XorInstruction() : Instruction(OP_XOR) {}

void XorInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::binary_xor(left, right));
}

BinaryAndInstruction::BinaryAndInstruction() : Instruction(OP_BINARY_AND) {}

void BinaryAndInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::binary_and(left, right));
}

BinaryOrInstruction::BinaryOrInstruction() : Instruction(OP_BINARY_OR) {}

void BinaryOrInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::binary_or(left, right));
}

BinaryNotInstruction::BinaryNotInstruction() : Instruction(OP_BINARY_NOT) {}

void BinaryNotInstruction::execute(Vm& vm) const {
    vm.stack->push_back(var::binary_not(instructions::pop_var(vm.stack)));
}

PushInstruction::PushInstruction() : Instruction(OP_PUSH) {}

PushInstruction::PushInstruction(const Var& value) : Instruction(OP_PUSH) {
    this->value = value;
}

void PushInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    value = var::read(buffer, index);
}

void PushInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    var::push(value, buffer);
}
    
void PushInstruction::execute(Vm& vm) const {
    vm.stack->push_back(value);
}

void PushInstruction::read_string(const std::vector<std::string>& strings) {
    value = var::from_string(strings);
}

std::string PushInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << var::to_string(value);
    return ss.str();
}

uint8_t PushInstruction::size() const {
    return Instruction::size() + var::size(value);
}

JumpInstruction::JumpInstruction() : Instruction(OP_JUMP) {}

JumpInstruction::JumpInstruction(const Address& address) : Instruction(OP_JUMP) {
    this->address = address;
}

JumpInstruction::JumpInstruction(const uint8_t& opcode) : Instruction(opcode) {}

JumpInstruction::JumpInstruction(const uint8_t& opcode, const Address& address) : Instruction(opcode) {
    this->address = address;
}

void JumpInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    address = byteutils::read_ulong(buffer, *index);
    *index += SIZE_OF_LONG;
}

void JumpInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    byteutils::push_ulong(buffer, address);
}

void JumpInstruction::execute(Vm& vm) const {
    vm.ip = address;
}

void JumpInstruction::read_string(const std::vector<std::string>& strings) {
    address = stoul(strings[0]);
}

std::string JumpInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address;
    return ss.str();
}

uint8_t JumpInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

void JumpInstruction::set_address(const Address& address) {
    this->address = address;
}

JumpIfInstruction::JumpIfInstruction() : JumpInstruction((uint8_t) OP_JUMP_IF) {}

JumpIfInstruction::JumpIfInstruction(const Address& address) : JumpInstruction(OP_JUMP_IF, address) {}

void JumpIfInstruction::execute(Vm& vm) const {
    if (instructions::pop_var(vm.stack).data._bool) {
        vm.ip = address;
    }
}

JumpIfFalseInstruction::JumpIfFalseInstruction() : JumpInstruction((uint8_t) OP_JUMP_IF_FALSE) {}

JumpIfFalseInstruction::JumpIfFalseInstruction(const Address& address) : JumpInstruction(OP_JUMP_IF_FALSE, address) {}

void JumpIfFalseInstruction::execute(Vm& vm) const {
    if (!instructions::pop_var(vm.stack).data._bool) {
        vm.ip = address;
    }
}

CallInstruction::CallInstruction() : Instruction(OP_CALL) {}

CallInstruction::CallInstruction(const Address& address, const uint8_t& param_count) : Instruction(OP_CALL) {
    this->address = address;
    this->param_count = param_count;
}

void CallInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    address = byteutils::read_ulong(buffer, *index);
    *index += SIZE_OF_LONG;
    param_count = buffer[*index];
    *index += SIZE_OF_BYTE;
}

void CallInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    byteutils::push_ulong(buffer, address);
    buffer.push_back(param_count);
}

void CallInstruction::execute(Vm& vm) const {
    vm.call_stack.push(vm.ip);
    vm.ip = address;
    std::vector<Var>* old_stack = vm.stack;
    vm.push_frame();
    for (int i = 0; i < param_count; i++) {
        vm.stack->push_back(instructions::pop_var(old_stack));
    }
}

void CallInstruction::read_string(const std::vector<std::string>& strings) {
    address = stoul(strings[0]);
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

void RetInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    values_count = buffer[*index];
    *index += SIZE_OF_BYTE;
}

void RetInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    buffer.push_back(values_count);
}

void RetInstruction::execute(Vm& vm) const {
    vm.ip = vm.call_stack.top();
    vm.call_stack.pop();
    Var values[values_count];
    for (int i = 0; i < values_count; i++) {
        values[i] = instructions::pop_var(vm.stack);
    }
    vm.pop_frame();
    for (int i = 0; i < values_count; i++) {
        vm.stack->push_back(values[i]);
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

LtInstruction::LtInstruction() : Instruction(OP_LT) {}

void LtInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::lt(left, right));
}

LteInstruction::LteInstruction() : Instruction(OP_LTE) {}

void LteInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::lte(left, right));
}

GtInstruction::GtInstruction() : Instruction(OP_GT) {}

void GtInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::gt(left, right));
}

GteInstruction::GteInstruction() : Instruction(OP_GTE) {}

void GteInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::gte(left, right));
}

EqInstruction::EqInstruction() : Instruction(OP_EQ) {}

void EqInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::eq(left, right));
}

NotEqInstruction::NotEqInstruction() : Instruction(OP_NOT_EQ) {}

void NotEqInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::neq(left, right));
}

BooleanAndInstruction::BooleanAndInstruction() : Instruction(OP_BOOLEAN_AND) {}

void BooleanAndInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::boolean_and(left, right));
}

BooleanOrInstruction::BooleanOrInstruction() : Instruction(OP_BOOLEAN_OR) {}

void BooleanOrInstruction::execute(Vm& vm) const {
    Var right = instructions::pop_var(vm.stack);
    Var left = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::boolean_or(left, right));
}

BooleanNotInstruction::BooleanNotInstruction() : Instruction(OP_BOOLEAN_NOT) {}

void BooleanNotInstruction::execute(Vm& vm) const {
    vm.stack->push_back(var::boolean_not(instructions::pop_var(vm.stack)));
}

PrintInstruction::PrintInstruction() : Instruction(OP_PRINT) {}

void PrintInstruction::execute(Vm& vm) const {
    var::print(instructions::pop_var(vm.stack));
}

StoreInstruction::StoreInstruction() : Instruction(OP_STORE) {}

StoreInstruction::StoreInstruction(const Address& address) : Instruction(OP_STORE) {
    this->address = address;
}

void StoreInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    address = byteutils::read_ulong(buffer, *index);
    *index += SIZE_OF_LONG;
}

void StoreInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    byteutils::push_ulong(buffer, address);
}

void StoreInstruction::execute(Vm& vm) const {
    vm.heap[address] = instructions::pop_var(vm.stack);
}

void StoreInstruction::read_string(const std::vector<std::string>& strings) {
    address = stoul(strings[0]);
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

LoadInstruction::LoadInstruction(const Address& address) : Instruction(OP_LOAD) {
    this->address = address;
}

void LoadInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    address = byteutils::read_ulong(buffer, *index);
    *index += SIZE_OF_LONG;
}

void LoadInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    byteutils::push_ulong(buffer, address);
}

void LoadInstruction::execute(Vm& vm) const {
    vm.stack->push_back(vm.heap[address]);
}

void LoadInstruction::read_string(const std::vector<std::string>& strings) {
    address = stoul(strings[0]);
}

std::string LoadInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << address;
    return ss.str();
}

uint8_t LoadInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

ConvertInstruction::ConvertInstruction() : Instruction(OP_CONVERT) {}

ConvertInstruction::ConvertInstruction(const var::DataType& type) : Instruction(OP_CONVERT) {
    this->type = type;
}

void ConvertInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    type = (var::DataType) buffer[*index];
    *index += SIZE_OF_BYTE;
}

void ConvertInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    buffer.push_back(type);
}

void ConvertInstruction::execute(Vm& vm) const {
    Var var = instructions::pop_var(vm.stack);
    vm.stack->push_back(var::convert(var, type));
}

void ConvertInstruction::read_string(const std::vector<std::string>& strings) {
    type = (var::DataType) stoi(strings[0]);
}

std::string ConvertInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << var::TYPE_NAME.at(type);
    return ss.str();
}

uint8_t ConvertInstruction::size() const {
    return Instruction::size() + SIZE_OF_BYTE;
}

NativeInstruction::NativeInstruction() : Instruction(OP_NATIVE) {}

NativeInstruction::NativeInstruction(const std::string& function_name) : Instruction(OP_NATIVE) {
    this->function_name = function_name;
    this->function_hash = NativeInstruction::hasher(function_name);
}

void NativeInstruction::read(const std::vector<uint8_t>& buffer, Address* index) {
    function_hash = byteutils::read_ulong(buffer, *index);
    *index += SIZE_OF_LONG;
}

void NativeInstruction::write(std::vector<uint8_t>& buffer) const {
    Instruction::write(buffer);
    byteutils::push_ulong(buffer, function_hash);
}

void NativeInstruction::execute(Vm& vm) const {
    const auto& fun = vm.c_functions.get_function(function_hash);
    std::vector<Var> args;
    for (const auto& c_type : fun->get_arg_types()) {
        var::DataType data_type = instructions::C_TYPE_TO_DATA_TYPE.at(c_type);
        Var arg = instructions::pop_var(vm.stack);
        if (arg.type != data_type) {
            std::cout << "Expected arg '" << var::TYPE_NAME.at(data_type) << "', but got '" << var::TYPE_NAME.at(arg.type) << "' instead. "<< std::endl;
            exit(1);
        }
        args.push_back(arg);
    }
    Var result = CFunctions::call(fun, args);
    vm.stack->push_back(result);
}

void NativeInstruction::read_string(const std::vector<std::string>& strings) {
    function_name = strings[0];
    function_hash = NativeInstruction::hasher(function_name);
}

std::string NativeInstruction::to_string() const {
    std::stringstream ss;
    ss << Instruction::to_string() << " " << function_name;
    return ss.str();
}

uint8_t NativeInstruction::size() const {
    return Instruction::size() + SIZE_OF_LONG;
}

HaltInstruction::HaltInstruction() : Instruction(OP_HALT) {}

void HaltInstruction::execute(Vm& vm) const {
    vm.running = false;
}