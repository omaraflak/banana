#include "ast.h"

AbstractSyntaxTree::AbstractSyntaxTree(const AbstractSyntaxTree* frame) {
    this->frame = frame;
}

uint64_t AbstractSyntaxTree::next_address(const AbstractSyntaxTree* frame) {
    uint64_t address = LATEST_ADDRESS[frame];
    LATEST_ADDRESS[frame] += SIZE_OF_LONG;
    return address;
}

LiteralNode::LiteralNode(const AbstractSyntaxTree* frame, const uint64_t& value) : AbstractSyntaxTree(frame) {
    this->value = value;
}

void LiteralNode::write(std::vector<Instruction*>& instructions) const {
    instructions.push_back(new PushInstruction(value));
}

VariableNode::VariableNode(const AbstractSyntaxTree* frame) : AbstractSyntaxTree(frame) {}

void VariableNode::write(std::vector<Instruction*>& instructions) const {
    instructions.push_back(new LoadInstruction(AbstractSyntaxTree::next_address(frame)));
}

uint64_t VariableNode::get_address() const {
    return address;
}

AssignNode::AssignNode(const AbstractSyntaxTree* frame, const VariableNode* node, const AbstractSyntaxTree* expression) : AbstractSyntaxTree(frame) {
    this->node = node;
    this->expression = expression;
}

void AssignNode::write(std::vector<Instruction*>& instructions) const {
    expression->write(instructions);
    instructions.push_back(new StoreInstruction(node->get_address()));
}

BlockNode::BlockNode(const AbstractSyntaxTree* frame) : AbstractSyntaxTree(frame) {}

void BlockNode::write(std::vector<Instruction*>& instructions) const {
    for (auto node : nodes) {
        node->write(instructions);
    }
}

void BlockNode::add(const AbstractSyntaxTree* node) {
    nodes.push_back(node);
}

BinaryOperationNode::BinaryOperationNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* left, const AbstractSyntaxTree* right, uint8_t operation) : AbstractSyntaxTree(frame) {
    this->left = left;
    this->right = right;
    this->operation = operation;
}

void BinaryOperationNode::write(std::vector<Instruction*>& instructions) const {
    left->write(instructions);
    right->write(instructions);
    switch (operation) {
        case ADD:
            instructions.push_back(new AddInstruction());
        case SUB:
            instructions.push_back(new SubInstruction());
        case MUL:
            instructions.push_back(new MulInstruction());
        case DIV:
            instructions.push_back(new DivInstruction());
        case MOD:
            instructions.push_back(new ModInstruction());
        case AND:
            instructions.push_back(new AndInstruction());
        case OR:
            instructions.push_back(new OrInstruction());
        case XOR:
            instructions.push_back(new XorInstruction());
        case LT:
            instructions.push_back(new LtInstruction());
        case LTE:
            instructions.push_back(new LteInstruction());
        case GT:
            instructions.push_back(new GtInstruction());
        case GTE:
            instructions.push_back(new GteInstruction());
        case EQ:
            instructions.push_back(new EqInstruction());
        case NOT_EQ:
            instructions.push_back(new NotEqInstruction());
    }
}

IfNode::IfNode(AbstractSyntaxTree* frame, const AbstractSyntaxTree* expression, const AbstractSyntaxTree* if_block, const AbstractSyntaxTree* else_block) : AbstractSyntaxTree(frame) {
    this->expression = expression;
    this->if_block = if_block;
    this->else_block = else_block;
}

void IfNode::write(std::vector<Instruction*>& instructions) const {
    expression->write(instructions);
    instructions.push_back(new JumpIfFalseInstruction(0));
}