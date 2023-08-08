#if !defined(AST)
#define AST

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <stdint.h>
#include "instructions.h"

class AbstractSyntaxTree {
    public:
    AbstractSyntaxTree();
    virtual void write(std::vector<const Instruction*>& instructions);

    uint64_t get_program_address() const;
    bool is_written() const;

    protected:
    uint64_t program_address;
    bool written;
};

class LiteralNode: public AbstractSyntaxTree {
    public:
    LiteralNode(const uint64_t& value);
    void write(std::vector<const Instruction*>& instructions);

    private:
    uint64_t value;
};

class VariableNode: public AbstractSyntaxTree {
    public:
    VariableNode(const AbstractSyntaxTree* frame);
    void write(std::vector<const Instruction*>& instructions);
    uint64_t get_address() const;
    
    private:
    const AbstractSyntaxTree* frame;
    uint64_t address;

    static inline std::map<const AbstractSyntaxTree*, uint64_t> latest_address;
};

enum {
    ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, LT, LTE, GT, GTE, EQ, NOT_EQ
};

class BinaryOperationNode: public AbstractSyntaxTree {
    public:
    BinaryOperationNode(
        AbstractSyntaxTree* left,
        AbstractSyntaxTree* right,
        const uint8_t& operation
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    AbstractSyntaxTree* left;
    AbstractSyntaxTree* right;
    uint8_t operation;
};

class BlockNode: public AbstractSyntaxTree {
    public:
    BlockNode();
    void write(std::vector<const Instruction*>& instructions);
    void add(AbstractSyntaxTree* node);

    private:
    std::vector<AbstractSyntaxTree*> nodes;
};

class AssignNode: public AbstractSyntaxTree {
    public:
    AssignNode(VariableNode* node, AbstractSyntaxTree* expression);
    void write(std::vector<const Instruction*>& instructions);

    private:
    VariableNode* node;
    AbstractSyntaxTree* expression;
};

class IfNode: public AbstractSyntaxTree {
    public:
    IfNode(
        AbstractSyntaxTree* condition,
        AbstractSyntaxTree* if_block,
        AbstractSyntaxTree* else_block = nullptr
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    AbstractSyntaxTree* condition;
    AbstractSyntaxTree* if_block;
    AbstractSyntaxTree* else_block;
};

class WhileNode: public AbstractSyntaxTree {
    public:
    WhileNode( AbstractSyntaxTree* condition, AbstractSyntaxTree* body);
    void write(std::vector<const Instruction*>& instructions);

    private:
    AbstractSyntaxTree* condition;
    AbstractSyntaxTree* body;
};

class ForNode: public AbstractSyntaxTree {
    public:
    ForNode(
        AbstractSyntaxTree* init,
        AbstractSyntaxTree* condition,
        AbstractSyntaxTree* increment,
        AbstractSyntaxTree* body
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    AbstractSyntaxTree* init;
    AbstractSyntaxTree* condition;
    AbstractSyntaxTree* increment;
    AbstractSyntaxTree* body;
};

class PrintNode: public AbstractSyntaxTree {
    public:
    PrintNode(AbstractSyntaxTree* expression);
    void write(std::vector<const Instruction*>& instructions);
    
    private:
    AbstractSyntaxTree* expression;
};

class PrintStringNode: public AbstractSyntaxTree {
    public:
    PrintStringNode( const std::string& str);
    void write(std::vector<const Instruction*>& instructions);
    
    private:
    std::string str;
};

class FunctionNode: public AbstractSyntaxTree {
    public:
    FunctionNode(AbstractSyntaxTree* body, const std::vector<VariableNode*>& parameters);
    void write(std::vector<const Instruction*>& instructions);
    uint64_t get_parameters_count() const;

    private:
    AbstractSyntaxTree* body;
    std::vector<const VariableNode*> parameters;
};

class CallNode: public AbstractSyntaxTree {
    public:
    CallNode(FunctionNode* function, const std::vector<AbstractSyntaxTree*>& values);
    void write(std::vector<const Instruction*>& instructions);

    private:
    FunctionNode* function;
    std::vector<AbstractSyntaxTree*> values;
};

class ReturnNode: public AbstractSyntaxTree {
    public:
    ReturnNode(AbstractSyntaxTree* value = nullptr);
    void write(std::vector<const Instruction*>& instructions);

    private:
    AbstractSyntaxTree* value;
};

class HaltNode: public AbstractSyntaxTree {
    public:
    HaltNode();
    void write(std::vector<const Instruction*>& instructions);
};

namespace ast {
    uint64_t count_bytes(const std::vector<const Instruction*>& instructions);
    std::vector<std::unique_ptr<const Instruction>> to_instructions(AbstractSyntaxTree* root, AbstractSyntaxTree* main = nullptr);
    std::vector<uint8_t> to_bytes(const std::vector<std::unique_ptr<const Instruction>>& instructions);
    std::vector<std::pair<uint64_t, std::string>> to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions);
};

#endif // AST