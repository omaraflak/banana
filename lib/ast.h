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

    void set_main(AbstractSyntaxTree* main);
    AbstractSyntaxTree* get_main() const;

    protected:
    AbstractSyntaxTree* main;
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
    VariableNode(const std::shared_ptr<const AbstractSyntaxTree>& frame);
    void write(std::vector<const Instruction*>& instructions);
    uint64_t get_address() const;
    
    private:
    std::shared_ptr<const AbstractSyntaxTree> frame;
    uint64_t address;

    static inline std::map<std::shared_ptr<const AbstractSyntaxTree>, uint64_t> latest_address;
};

enum AstBinaryOperation {
    AST_ADD, AST_SUB, AST_MUL, AST_DIV,
    AST_MOD, AST_BIN_AND, AST_BIN_OR,
    AST_XOR, AST_LT, AST_LTE, AST_GT,
    AST_GTE, AST_EQ, AST_NOT_EQ,
    AST_BOOL_AND, AST_BOOL_OR
};

class BinaryOperationNode: public AbstractSyntaxTree {
    public:
    BinaryOperationNode(
        const std::shared_ptr<AbstractSyntaxTree>& left,
        const std::shared_ptr<AbstractSyntaxTree>& right,
        const AstBinaryOperation& operation
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> left;
    std::shared_ptr<AbstractSyntaxTree> right;
    AstBinaryOperation operation;
};

enum AstUnaryOperation {
    AST_BIN_NOT, AST_BOOL_NOT
};

class UnaryOperationNode: public AbstractSyntaxTree {
    public:
    UnaryOperationNode(
        const std::shared_ptr<AbstractSyntaxTree>& expression,
        const AstUnaryOperation& operation
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> expression;
    AstUnaryOperation operation;
};

class BlockNode: public AbstractSyntaxTree {
    public:
    BlockNode();
    void write(std::vector<const Instruction*>& instructions);
    void add(const std::shared_ptr<AbstractSyntaxTree>& node);

    private:
    std::vector<std::shared_ptr<AbstractSyntaxTree>> nodes;
};

class AssignNode: public AbstractSyntaxTree {
    public:
    AssignNode(const std::shared_ptr<VariableNode>& node, const std::shared_ptr<AbstractSyntaxTree>& expression);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<VariableNode> node;
    std::shared_ptr<AbstractSyntaxTree> expression;
};

class IfNode: public AbstractSyntaxTree {
    public:
    IfNode(
        const std::shared_ptr<AbstractSyntaxTree>& condition,
        const std::shared_ptr<AbstractSyntaxTree>& if_block,
        const std::shared_ptr<AbstractSyntaxTree>& else_block = nullptr
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> condition;
    std::shared_ptr<AbstractSyntaxTree> if_block;
    std::shared_ptr<AbstractSyntaxTree> else_block;
};

class WhileNode: public AbstractSyntaxTree {
    public:
    WhileNode(const std::shared_ptr<AbstractSyntaxTree>& condition, const std::shared_ptr<AbstractSyntaxTree>& body);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> condition;
    std::shared_ptr<AbstractSyntaxTree> body;
};

class ForNode: public AbstractSyntaxTree {
    public:
    ForNode(
        const std::shared_ptr<AbstractSyntaxTree>& init,
        const std::shared_ptr<AbstractSyntaxTree>& condition,
        const std::shared_ptr<AbstractSyntaxTree>& increment,
        const std::shared_ptr<AbstractSyntaxTree>& body
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> init;
    std::shared_ptr<AbstractSyntaxTree> condition;
    std::shared_ptr<AbstractSyntaxTree> increment;
    std::shared_ptr<AbstractSyntaxTree> body;
};

class PrintNode: public AbstractSyntaxTree {
    public:
    PrintNode(const std::shared_ptr<AbstractSyntaxTree>& expression, const std::string& end = "\n");
    void write(std::vector<const Instruction*>& instructions);
    
    private:
    std::shared_ptr<AbstractSyntaxTree> expression;
    std::string end;
};

class PrintStringNode: public AbstractSyntaxTree {
    public:
    PrintStringNode(const std::string& str);
    void write(std::vector<const Instruction*>& instructions);
    
    private:
    std::string str;
};

class FunctionNode: public AbstractSyntaxTree {
    public:
    FunctionNode();
    FunctionNode(const std::shared_ptr<AbstractSyntaxTree>& body, const std::vector<std::shared_ptr<VariableNode>>& parameters);
    void write(std::vector<const Instruction*>& instructions);
    uint8_t get_parameters_count() const;

    void set_body(const std::shared_ptr<AbstractSyntaxTree>& body);
    void set_parameters(const std::vector<std::shared_ptr<VariableNode>>& parameters);

    private:
    std::shared_ptr<AbstractSyntaxTree> body;
    std::vector<std::shared_ptr<const VariableNode>> parameters;
};

class CallNode: public AbstractSyntaxTree {
    public:
    CallNode(const std::shared_ptr<FunctionNode>& function, const std::vector<std::shared_ptr<AbstractSyntaxTree>>& values);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<FunctionNode> function;
    std::vector<std::shared_ptr<AbstractSyntaxTree>> values;
};

class ReturnNode: public AbstractSyntaxTree {
    public:
    ReturnNode(const std::vector<std::shared_ptr<AbstractSyntaxTree>>& values = std::vector<std::shared_ptr<AbstractSyntaxTree>>());
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::vector<std::shared_ptr<AbstractSyntaxTree>> values;
};

class HaltNode: public AbstractSyntaxTree {
    public:
    HaltNode();
    void write(std::vector<const Instruction*>& instructions);
};

namespace ast {
    uint64_t count_bytes(const std::vector<const Instruction*>& instructions);
    std::vector<std::unique_ptr<const Instruction>> to_instructions(const std::shared_ptr<AbstractSyntaxTree>& root);
    std::vector<uint8_t> to_bytes(const std::vector<std::unique_ptr<const Instruction>>& instructions);
    std::vector<std::pair<uint64_t, std::string>> to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions);
};

#endif // AST