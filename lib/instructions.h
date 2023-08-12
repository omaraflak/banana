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

class AddInstruction: public Instruction {
    public:
    AddInstruction();
    void execute(Vm& vm) const;
};

class SubInstruction: public Instruction {
    public:
    SubInstruction();
    void execute(Vm& vm) const;
};

class MulInstruction: public Instruction {
    public:
    MulInstruction();
    void execute(Vm& vm) const;
};

class DivInstruction: public Instruction {
    public:
    DivInstruction();
    void execute(Vm& vm) const;
};

class ModInstruction: public Instruction {
    public:
    ModInstruction();
    void execute(Vm& vm) const;
};

class AndInstruction: public Instruction {
    public:
    AndInstruction();
    void execute(Vm& vm) const;
};

class OrInstruction: public Instruction {
    public:
    OrInstruction();
    void execute(Vm& vm) const;
};

class XorInstruction: public Instruction {
    public:
    XorInstruction();
    void execute(Vm& vm) const;
};

class NotInstruction: public Instruction {
    public:
    NotInstruction();
    void execute(Vm& vm) const;
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

class LtInstruction: public Instruction {
    public:
    LtInstruction();
    void execute(Vm& vm) const;
};

class LteInstruction: public Instruction {
    public:
    LteInstruction();
    void execute(Vm& vm) const;
};

class GtInstruction: public Instruction {
    public:
    GtInstruction();
    void execute(Vm& vm) const;
};

class GteInstruction: public Instruction {
    public:
    GteInstruction();
    void execute(Vm& vm) const;
};

class EqInstruction: public Instruction {
    public:
    EqInstruction();
    void execute(Vm& vm) const;
};

class NotEqInstruction: public Instruction {
    public:
    NotEqInstruction();
    void execute(Vm& vm) const;
};

class BoolAndInstruction: public Instruction {
    public:
    BoolAndInstruction();
    void execute(Vm& vm) const;
};

class BoolOrInstruction: public Instruction {
    public:
    BoolOrInstruction();
    void execute(Vm& vm) const;
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
