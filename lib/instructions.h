#if !defined(INSTRUCTIONS)
#define INSTRUCTIONS

#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>
#include <functional>
#include "byteutils.h"
#include "vm.h"

typedef int64_t Address;
typedef uint64_t Value;

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
    OP_BOOL_AND,
    OP_BOOL_OR,
    OP_BOOL_NOT,
    OP_PRINT,
    OP_PRINT_C,
    OP_STORE,
    OP_LOAD,
    OP_HALT
};

class Instruction {
    public:
    Instruction(const uint8_t& opcode);
    virtual void read(const std::vector<uint8_t>& buffer, Value* index);
    virtual void write(std::vector<uint8_t>& buffer) const;
    virtual void execute(Vm& vm) const;
    virtual void read_string(const std::vector<std::string>& strings);
    virtual std::string to_string() const;
    virtual uint8_t size() const;

    static Instruction* from_opcode(const uint8_t& opcode);
    static Instruction* from_opstring(const std::string& opstring);
    static Instruction* from_string(const std::string& str);

    private:
    uint8_t opcode;
};

class BinaryOperationInstruction: public Instruction {
    public:
    BinaryOperationInstruction(const uint8_t& opcode, std::function<Value(const Value&, const Value&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<Value(const Value&, const Value&)> operation;
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
    UnaryOperationInstruction(const uint8_t& opcode, std::function<Value(const Value&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<Value(const Value&)> operation;
};

class NotInstruction: public UnaryOperationInstruction {
    public:
    NotInstruction();
};

class PushInstruction: public Instruction {
    public:
    PushInstruction();
    PushInstruction(const Value& value);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;


    private:
    Value value;
};

class JumpInstruction: public Instruction {
    public:
    JumpInstruction();
    JumpInstruction(const Address& address);
    JumpInstruction(const uint8_t& opcode);
    JumpInstruction(const uint8_t& opcode, const Address& address);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    void set_address(const Address& address);
    uint8_t size() const;

    protected:
    Address address;
};

class JumpIfInstruction: public JumpInstruction {
    public:
    JumpIfInstruction();
    JumpIfInstruction(const Address& address);
    void execute(Vm& vm) const;
};

class JumpIfFalseInstruction: public JumpInstruction {
    public:
    JumpIfFalseInstruction();
    JumpIfFalseInstruction(const Address& address);
    void execute(Vm& vm) const;
};

class CallInstruction: public Instruction {
    public:
    CallInstruction();
    CallInstruction(const Address& address, const uint8_t& param_count);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    Address address;
    uint8_t param_count;
};

class RetInstruction: public Instruction {
    public:
    RetInstruction();
    RetInstruction(const uint8_t& values_count);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint8_t values_count;
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
    CompareInstruction(const uint8_t& opcode, std::function<uint8_t(const Value&, const Value&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<uint8_t(const Value&, const Value&)> operation;
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

class BoolInstruction: public Instruction {
    public:
    BoolInstruction(const uint8_t& opcode, std::function<uint8_t(const bool&, const bool&)> operation);
    void execute(Vm& vm) const;

    private:
    std::function<uint8_t(const bool&, const bool&)> operation;
};

class BoolAndInstruction: public BoolInstruction {
    public:
    BoolAndInstruction();
};

class BoolOrInstruction: public BoolInstruction {
    public:
    BoolOrInstruction();
};

class BoolNotInstruction: public Instruction {
    public:
    BoolNotInstruction();
    void execute(Vm& vm) const;
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
    StoreInstruction(const Address& address);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    Address address;
};

class LoadInstruction: public Instruction {
    public:
    LoadInstruction();
    LoadInstruction(const Address& address);
    void read(const std::vector<uint8_t>& buffer, Value* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    Address address;
};

class HaltInstruction: public Instruction {
    public:
    HaltInstruction();
    void execute(Vm& vm) const;
};

#endif // INSTRUCTIONS
