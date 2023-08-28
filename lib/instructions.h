#if !defined(INSTRUCTIONS)
#define INSTRUCTIONS

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <stdint.h>
#include "var.h"
#include "vm.h"

typedef uint64_t Address;

enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_XOR,
    OP_BINARY_AND,
    OP_BINARY_OR,
    OP_BINARY_NOT,
    OP_PUSH,
    OP_JUMP,
    OP_JUMP_IF,
    OP_JUMP_IF_FALSE,
    OP_CALL,
    OP_RET,
    OP_LT,
    OP_LTE,
    OP_GT,
    OP_GTE,
    OP_EQ,
    OP_NOT_EQ,
    OP_BOOLEAN_AND,
    OP_BOOLEAN_OR,
    OP_BOOLEAN_NOT,
    OP_PRINT,
    OP_STORE,
    OP_LOAD,
    OP_CONVERT,
    OP_NATIVE,
    OP_HALT,
    OP_OPERATIONS_COUNT
};

class Instruction {
    public:
    Instruction(const uint8_t& opcode);
    virtual ~Instruction();
    virtual void read(const std::vector<uint8_t>& buffer, Address* index);
    virtual void write(std::vector<uint8_t>& buffer) const;
    virtual void execute(Vm& vm) const;
    virtual void read_string(const std::vector<std::string>& strings);
    virtual std::string to_string() const;
    virtual uint8_t size() const;

    static std::shared_ptr<Instruction> from_opcode(const uint8_t& opcode);
    static std::shared_ptr<Instruction> from_opstring(const std::string& opstring);
    static std::shared_ptr<Instruction> from_string(const std::string& str);
    static std::vector<uint8_t> to_bytes(const std::vector<std::unique_ptr<const Instruction>>& instructions);
    static std::vector<std::pair<Address, std::string>> to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions);
    static std::shared_ptr<Instruction> const OP_INSTANCES[OP_OPERATIONS_COUNT];
    static Instruction* const OP_INSTANCES_PTR[OP_OPERATIONS_COUNT];

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

class XorInstruction: public Instruction {
    public:
    XorInstruction();
    void execute(Vm& vm) const;
};

class BinaryAndInstruction: public Instruction {
    public:
    BinaryAndInstruction();
    void execute(Vm& vm) const;
};

class BinaryOrInstruction: public Instruction {
    public:
    BinaryOrInstruction();
    void execute(Vm& vm) const;
};

class BinaryNotInstruction: public Instruction {
    public:
    BinaryNotInstruction();
    void execute(Vm& vm) const;
};

class PushInstruction: public Instruction {
    public:
    PushInstruction();
    PushInstruction(const Var& value);
    void read(const std::vector<uint8_t>& buffer, Address* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    Var value;
};

class JumpInstruction: public Instruction {
    public:
    JumpInstruction();
    JumpInstruction(const Address& address);
    JumpInstruction(const uint8_t& opcode);
    JumpInstruction(const uint8_t& opcode, const Address& address);
    void read(const std::vector<uint8_t>& buffer, Address* index);
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
    void read(const std::vector<uint8_t>& buffer, Address* index);
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
    void read(const std::vector<uint8_t>& buffer, Address* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    uint8_t values_count;
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

class BooleanAndInstruction: public Instruction {
    public:
    BooleanAndInstruction();
    void execute(Vm& vm) const;
};

class BooleanOrInstruction: public Instruction {
    public:
    BooleanOrInstruction();
    void execute(Vm& vm) const;
};

class BooleanNotInstruction: public Instruction {
    public:
    BooleanNotInstruction();
    void execute(Vm& vm) const;
};

class PrintInstruction: public Instruction {
    public:
    PrintInstruction();
    void execute(Vm& vm) const;
};

class StoreInstruction: public Instruction {
    public:
    StoreInstruction();
    StoreInstruction(const Address& address);
    void read(const std::vector<uint8_t>& buffer, Address* index);
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
    void read(const std::vector<uint8_t>& buffer, Address* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    Address address;
};

class ConvertInstruction: public Instruction {
    public:
    ConvertInstruction();
    ConvertInstruction(const var::DataType& type);
    void read(const std::vector<uint8_t>& buffer, Address* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    var::DataType type;
};

class NativeInstruction: public Instruction {
    public:
    NativeInstruction();
    NativeInstruction(const std::string& function_name);
    void read(const std::vector<uint8_t>& buffer, Address* index);
    void write(std::vector<uint8_t>& buffer) const;
    void execute(Vm& vm) const;
    void read_string(const std::vector<std::string>& strings);
    std::string to_string() const;
    uint8_t size() const;

    private:
    std::string function_name;
    uint64_t function_hash;
    
    inline static std::hash<std::string> hasher;
};

class HaltInstruction: public Instruction {
    public:
    HaltInstruction();
    void execute(Vm& vm) const;
};

#endif // INSTRUCTIONS
