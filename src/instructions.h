#if !defined(INSTRUCTIONS)
#define INSTRUCTIONS

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>
#include <functional>
#include <map>
#include "byteutils.h"
#include "vm.h"

enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_PUSH,
    OP_JUMP,
    OP_JUMP_IF,
    OP_JUMP_IF_FALSE,
    OP_CALL,
    OP_RET,
    OP_DUP,
    OP_SWAP,
    OP_LT,
    OP_LTE,
    OP_GT,
    OP_GTE,
    OP_EQ,
    OP_NOT_EQ,
    OP_PRINT,
    OP_PRINT_C,
    OP_STORE,
    OP_LOAD,
    OP_HALT
};

static std::map<uint8_t, std::string> OP_STRINGS = {
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
    {OP_PRINT, "print"},
    {OP_PRINT_C, "printc"},
    {OP_STORE, "store"},
    {OP_LOAD, "load"},
    {OP_HALT, "halt"},
};

static std::map<std::string, uint8_t> OP_STRINGS_REV = {
    {"add", OP_ADD},
    {"sub", OP_SUB},
    {"mul", OP_MUL},
    {"div", OP_DIV},
    {"mod", OP_MOD},
    {"and", OP_AND},
    {"or", OP_OR},
    {"xor", OP_XOR},
    {"not", OP_NOT},
    {"push", OP_PUSH},
    {"jump", OP_JUMP},
    {"jump_if", OP_JUMP_IF},
    {"jump_if_false", OP_JUMP_IF_FALSE},
    {"call", OP_CALL},
    {"ret", OP_RET},
    {"dup", OP_DUP},
    {"swap", OP_SWAP},
    {"lt", OP_LT},
    {"lte", OP_LTE},
    {"gt", OP_GT},
    {"gte", OP_GTE},
    {"eq", OP_EQ},
    {"not_eq", OP_NOT_EQ},
    {"print", OP_PRINT},
    {"printc", OP_PRINT_C},
    {"store", OP_STORE},
    {"load", OP_LOAD},
    {"halt", OP_HALT},
};

class Instruction {
    public:
    Instruction(const uint8_t& opcode);
    virtual void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    virtual void write(std::vector<uint8_t>& buffer) const;
    virtual void execute(Vm& vm) const;
    virtual void read_string(const std::vector<std::string>& strings);
    virtual std::string to_string() const;
    virtual uint8_t size() const;

    static Instruction* from_opcode(const uint8_t& opcode);
    static Instruction* from_string(const std::string& str);

    private:
    uint8_t opcode;
};

class BinaryOperationInstruction: public Instruction {
    public:
    BinaryOperationInstruction(const uint8_t& opcode, std::function<uint64_t(const uint64_t&, const uint64_t&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<uint64_t(const uint64_t&, const uint64_t&)> operation;
};

class AddInstruction: public BinaryOperationInstruction {
    public:
    AddInstruction();
};

class SubInstruction: public BinaryOperationInstruction {
    public:
    SubInstruction();
};

class MulInstruction: public BinaryOperationInstruction {
    public:
    MulInstruction();
};

class DivInstruction: public BinaryOperationInstruction {
    public:
    DivInstruction();
};

class ModInstruction: public BinaryOperationInstruction {
    public:
    ModInstruction();
};

class AndInstruction: public BinaryOperationInstruction {
    public:
    AndInstruction();
};

class OrInstruction: public BinaryOperationInstruction {
    public:
    OrInstruction();
};

class XorInstruction: public BinaryOperationInstruction {
    public:
    XorInstruction();
};

class UnaryOperationInstruction: public Instruction {
    public:
    UnaryOperationInstruction(const uint8_t& opcode, std::function<uint64_t(const uint64_t&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<uint64_t(const uint64_t&)> operation;
};

class NotInstruction: public UnaryOperationInstruction {
    public:
    NotInstruction();
};

class PushInstruction: public Instruction {
    public:
    PushInstruction();
    PushInstruction(const uint64_t& value);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;


    private:
    uint64_t value;
};

class JumpInstruction: public Instruction {
    public:
    JumpInstruction();
    JumpInstruction(const uint64_t& address);
    JumpInstruction(const uint8_t& opcode);
    JumpInstruction(const uint8_t& opcode, const uint64_t& address);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    void set_address(const uint64_t& address);
    uint8_t size() const;

    protected:
    uint64_t address;
};

class JumpIfInstruction: public JumpInstruction {
    public:
    JumpIfInstruction();
    JumpIfInstruction(const uint64_t& address);
    void execute(Vm& vm) const;
};

class JumpIfFalseInstruction: public JumpInstruction {
    public:
    JumpIfFalseInstruction();
    JumpIfFalseInstruction(const uint64_t& address);
    void execute(Vm& vm) const;
};

class CallInstruction: public Instruction {
    public:
    CallInstruction();
    CallInstruction(const uint64_t& address, const uint64_t& param_count);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint64_t address;
    uint64_t param_count;
};

class RetInstruction: public Instruction {
    public:
    RetInstruction();
    RetInstruction(const uint64_t& values_count);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint64_t values_count;
};

class DupInstruction: public Instruction {
    public:
    DupInstruction();
    void execute(Vm& vm) const;
};

class SwapInstruction: public Instruction {
    public:
    SwapInstruction();
    void execute(Vm& vm) const;
};

class CompareInstruction: public Instruction {
    public:
    CompareInstruction(const uint8_t& opcode, std::function<uint8_t(const uint64_t&, const uint64_t&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<uint8_t(const uint64_t&, const uint64_t&)> operation;
};


class LtInstruction: public CompareInstruction {
    public:
    LtInstruction();
};

class LteInstruction: public CompareInstruction {
    public:
    LteInstruction();
};

class GtInstruction: public CompareInstruction {
    public:
    GtInstruction();
};

class GteInstruction: public CompareInstruction {
    public:
    GteInstruction();
};

class EqInstruction: public CompareInstruction {
    public:
    EqInstruction();
};

class NotEqInstruction: public CompareInstruction {
    public:
    NotEqInstruction();
};

class PrintInstruction: public Instruction {
    public:
    PrintInstruction();
    void execute(Vm& vm) const;
};

class PrintCharInstruction: public Instruction {
    public:
    PrintCharInstruction();
    void execute(Vm& vm) const;
};

class StoreInstruction: public Instruction {
    public:
    StoreInstruction();
    StoreInstruction(const uint64_t& address);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint64_t address;
};

class LoadInstruction: public Instruction {
    public:
    LoadInstruction();
    LoadInstruction(const uint64_t& address);
    void read(const std::vector<uint8_t>& buffer, uint64_t* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint64_t address;
};

class HaltInstruction: public Instruction {
    public:
    HaltInstruction();
    void execute(Vm& vm) const;
};

#endif // INSTRUCTIONS
