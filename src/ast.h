#if !defined(AST)
#define AST

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include "instructions.h"

class AbstractSyntaxTree {
    public:
    AbstractSyntaxTree(const AbstractSyntaxTree* frame);
    virtual void write(std::vector<const Instruction*>& instructions) const = 0;

    protected:
    const AbstractSyntaxTree* frame;
    static inline std::map<const AbstractSyntaxTree*, uint64_t> latest_address;
    static inline uint64_t next_address(const AbstractSyntaxTree* frame);
};

class LiteralNode: public AbstractSyntaxTree {
    public:
    LiteralNode(const AbstractSyntaxTree* frame, const uint64_t& value);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    uint64_t value;
};

class VariableNode: public AbstractSyntaxTree {
    public:
    VariableNode(const AbstractSyntaxTree* frame);
    void write(std::vector<const Instruction*>& instructions) const;
    
    uint64_t get_address() const;
    
    private:
    uint64_t address;
};

enum {
    ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, LT, LTE, GT, GTE, EQ, NOT_EQ
};

class BinaryOperationNode: public AbstractSyntaxTree {
    public:
    BinaryOperationNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* left, const AbstractSyntaxTree* right, const uint8_t& operation);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* left;
    const AbstractSyntaxTree* right;
    uint8_t operation;
};

class BlockNode: public AbstractSyntaxTree {
    public:
    BlockNode(const AbstractSyntaxTree* frame);
    void write(std::vector<const Instruction*>& instructions) const;
    void add(const AbstractSyntaxTree* node);

    private:
    std::vector<const AbstractSyntaxTree*> nodes;
};

class AssignNode: public AbstractSyntaxTree {
    public:
    AssignNode(const AbstractSyntaxTree* frame, const VariableNode* node, const AbstractSyntaxTree* expression);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    const VariableNode* node;
    const AbstractSyntaxTree* expression;
};

class IfNode: public AbstractSyntaxTree {
    public:
    IfNode(AbstractSyntaxTree* frame, const AbstractSyntaxTree* condition, const AbstractSyntaxTree* if_block, const AbstractSyntaxTree* else_block = nullptr);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* condition;
    const AbstractSyntaxTree* if_block;
    const AbstractSyntaxTree* else_block;
};

class WhileNode: public AbstractSyntaxTree {
    public:
    WhileNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* condition, const AbstractSyntaxTree* body);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* condition;
    const AbstractSyntaxTree* body;
};

class ForNode: public AbstractSyntaxTree {
    public:
    ForNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* init, const AbstractSyntaxTree* condition, const AbstractSyntaxTree* increment, const AbstractSyntaxTree* body);
    void write(std::vector<const Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* init;
    const AbstractSyntaxTree* condition;
    const AbstractSyntaxTree* increment;
    const AbstractSyntaxTree* body;
};

class PrintNode: public AbstractSyntaxTree {
    public:
    PrintNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* expression);
    void write(std::vector<const Instruction*>& instructions) const;
    
    private:
    const AbstractSyntaxTree* expression;
};

class PrintStringNode: public AbstractSyntaxTree {
    public:
    PrintStringNode(const AbstractSyntaxTree* frame, const std::string& str);
    void write(std::vector<const Instruction*>& instructions) const;
    
    private:
    std::string str;
};

class FunctionNode: public AbstractSyntaxTree {
};

class CallNode: public AbstractSyntaxTree {
};

class ReturnNode: public AbstractSyntaxTree {
};

class HaltNode: public AbstractSyntaxTree {
    public:
    HaltNode(const AbstractSyntaxTree* frame);
    void write(std::vector<const Instruction*>& instructions) const;
};

namespace ast {
    uint64_t count_bytes(const std::vector<const Instruction*>& instructions);
    std::vector<uint8_t> to_bytes(const AbstractSyntaxTree* node);
    std::vector<std::pair<uint64_t, std::string>> to_asm(const AbstractSyntaxTree* node);
};

#endif // AST