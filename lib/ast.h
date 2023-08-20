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

    Address get_program_address() const;
    bool is_written() const;

    void set_main(AbstractSyntaxTree* main);
    AbstractSyntaxTree* get_main() const;

    protected:
    AbstractSyntaxTree* main;
    Address program_address;
    bool written;
};

class LiteralNode: public AbstractSyntaxTree {
    public:
    LiteralNode(const Var& value);
    void write(std::vector<const Instruction*>& instructions);

    private:
    Var value;
};

namespace ast {
enum AstVarType {
    BOOL, CHAR, INT, LONG,
};
}

class VariableNode: public AbstractSyntaxTree {
    public:
    VariableNode(const std::shared_ptr<const AbstractSyntaxTree>& frame, const ast::AstVarType& type);
    void write(std::vector<const Instruction*>& instructions);
    Address get_address() const;
    ast::AstVarType get_type() const;
    
    private:
    std::shared_ptr<const AbstractSyntaxTree> frame;
    Address address;
    ast::AstVarType type;

    static inline std::map<std::shared_ptr<const AbstractSyntaxTree>, Address> latest_address;
};

namespace ast {
enum AstBinaryOperation {
    ADD, SUB, MUL, DIV,
    MOD, BIN_AND, BIN_OR,
    XOR, LT, LTE, GT,
    GTE, EQ, NOT_EQ,
    BOOL_AND, BOOL_OR
};
}

class BinaryOperationNode: public AbstractSyntaxTree {
    public:
    BinaryOperationNode(
        const std::shared_ptr<AbstractSyntaxTree>& left,
        const std::shared_ptr<AbstractSyntaxTree>& right,
        const ast::AstBinaryOperation& operation
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> left;
    std::shared_ptr<AbstractSyntaxTree> right;
    ast::AstBinaryOperation operation;
};

class BooleanNotNode: public AbstractSyntaxTree {
    public:
    BooleanNotNode(const std::shared_ptr<AbstractSyntaxTree>& expression);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> expression;
};

class BinaryNotNode: public AbstractSyntaxTree {
    public:
    BinaryNotNode(const std::shared_ptr<AbstractSyntaxTree>& expression);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> expression;
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
    FunctionNode(
        const std::shared_ptr<AbstractSyntaxTree>& body,
        const std::vector<std::shared_ptr<VariableNode>>& parameters,
        const ast::AstVarType& return_type
    );
    void write(std::vector<const Instruction*>& instructions);
    std::vector<std::shared_ptr<const VariableNode>> get_parameters() const;
    uint8_t get_parameters_count() const;
    ast::AstVarType get_return_type() const;

    void set_body(const std::shared_ptr<AbstractSyntaxTree>& body);
    void set_parameters(const std::vector<std::shared_ptr<VariableNode>>& parameters);
    void set_return_type(const ast::AstVarType& return_type);

    private:
    std::shared_ptr<AbstractSyntaxTree> body;
    std::vector<std::shared_ptr<const VariableNode>> parameters;
    ast::AstVarType return_type;
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

class ConvertNode: public AbstractSyntaxTree {
    public:
    ConvertNode(const std::shared_ptr<AbstractSyntaxTree>& expression, const ast::AstVarType& type);
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::shared_ptr<AbstractSyntaxTree> expression;
    ast::AstVarType type;
};

class NativeNode: public AbstractSyntaxTree {
    public:
    NativeNode(
        const std::string& function_name,
        const std::vector<std::shared_ptr<VariableNode>>& values
    );
    void write(std::vector<const Instruction*>& instructions);

    private:
    std::string function_name;
    std::vector<std::shared_ptr<VariableNode>> values;
};

class HaltNode: public AbstractSyntaxTree {
    public:
    HaltNode();
    void write(std::vector<const Instruction*>& instructions);
};

namespace ast {
    Address count_bytes(const std::vector<const Instruction*>& instructions);
    std::vector<std::unique_ptr<const Instruction>> to_instructions(const std::shared_ptr<AbstractSyntaxTree>& root);
    std::vector<uint8_t> to_bytes(const std::vector<std::unique_ptr<const Instruction>>& instructions);
    std::vector<std::pair<Address, std::string>> to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions);
};

#endif // AST