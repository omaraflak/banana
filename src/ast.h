#if !defined(AST)
#define AST

#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include "instructions.h"

// Abstract Base Classes
class AbstractSyntaxTree {
    public:
    AbstractSyntaxTree(const AbstractSyntaxTree* frame);
    virtual void write(std::vector<Instruction*>& instructions) const = 0;

    protected:
    const AbstractSyntaxTree* frame;
    static inline std::map<const AbstractSyntaxTree*, uint64_t> LATEST_ADDRESS;
    static inline uint64_t next_address(const AbstractSyntaxTree* frame);
};

class LiteralNode: public AbstractSyntaxTree {
    public:
    LiteralNode(const AbstractSyntaxTree* frame, const uint64_t& value);
    void write(std::vector<Instruction*>& instructions) const;

    private:
    uint64_t value;
};

class VariableNode: public AbstractSyntaxTree {
    public:
    VariableNode(const AbstractSyntaxTree* frame);
    void write(std::vector<Instruction*>& instructions) const;
    
    uint64_t get_address() const;
    
    private:
    uint64_t address;
};

enum {
    ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, LT, LTE, GT, GTE, EQ, NOT_EQ
};

class BinaryOperationNode: public AbstractSyntaxTree {
    public:
    BinaryOperationNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* left, const AbstractSyntaxTree* right, uint8_t operation);
    void write(std::vector<Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* left;
    const AbstractSyntaxTree* right;
    uint8_t operation;
};

class BlockNode: public AbstractSyntaxTree {
    public:
    BlockNode(const AbstractSyntaxTree* frame);
    void write(std::vector<Instruction*>& instructions) const;
    void add(const AbstractSyntaxTree* node);

    private:
    std::vector<const AbstractSyntaxTree*> nodes;
};

class AssignNode: public AbstractSyntaxTree {
    public:
    AssignNode(const AbstractSyntaxTree* frame, const VariableNode* node, const AbstractSyntaxTree* expression);
    void write(std::vector<Instruction*>& instructions) const;

    private:
    const VariableNode* node;
    const AbstractSyntaxTree* expression;
};

class IfNode: public AbstractSyntaxTree {
    public:
    IfNode(AbstractSyntaxTree* frame, const AbstractSyntaxTree* expression, const AbstractSyntaxTree* if_block, const AbstractSyntaxTree* else_block = nullptr);
    void write(std::vector<Instruction*>& instructions) const;

    private:
    const AbstractSyntaxTree* expression;
    const AbstractSyntaxTree* if_block;
    const AbstractSyntaxTree* else_block;
};

class WhileNode: public AbstractSyntaxTree {
};

class ForNode: public AbstractSyntaxTree {
};

class PrintNode: public AbstractSyntaxTree {
};

class FunctionNode: public AbstractSyntaxTree {
};

class CallNode: public AbstractSyntaxTree {
};

class PrintStringNode: public AbstractSyntaxTree {
};

class HaltNode: public AbstractSyntaxTree {
};

class ReturnNode: public AbstractSyntaxTree {
};

#endif // AST