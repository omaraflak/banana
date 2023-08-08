#include "ast.h"

AbstractSyntaxTree::AbstractSyntaxTree(const AbstractSyntaxTree* frame) {
    this->frame = frame;
}

uint64_t AbstractSyntaxTree::next_address(const AbstractSyntaxTree* frame) {
    if (frame == nullptr) {
        std::cout << "Trying to get address without a frame" << std::endl;
        exit(1);
    }
    if (LATEST_ADDRESS.find(frame) == LATEST_ADDRESS.end()) {
        LATEST_ADDRESS[frame] = 0;
    }
    uint64_t address = LATEST_ADDRESS[frame];
    LATEST_ADDRESS[frame] += SIZE_OF_LONG;
    return address;
}

LiteralNode::LiteralNode(const AbstractSyntaxTree* frame, const uint64_t& value) : AbstractSyntaxTree(frame) {
    this->value = value;
}

void LiteralNode::write(std::vector<const Instruction*>& instructions) const {
    instructions.push_back(new PushInstruction(value));
}

VariableNode::VariableNode(const AbstractSyntaxTree* frame) : AbstractSyntaxTree(frame) {
    this->address = AbstractSyntaxTree::next_address(frame);
}

void VariableNode::write(std::vector<const Instruction*>& instructions) const {
    instructions.push_back(new LoadInstruction(address));
}

uint64_t VariableNode::get_address() const {
    return address;
}

AssignNode::AssignNode(
    const AbstractSyntaxTree* frame,
    const VariableNode* node,
    const AbstractSyntaxTree* expression
) : AbstractSyntaxTree(frame) {
    this->node = node;
    this->expression = expression;
}

void AssignNode::write(std::vector<const Instruction*>& instructions) const {
    expression->write(instructions);
    instructions.push_back(new StoreInstruction(node->get_address()));
}

BlockNode::BlockNode(const AbstractSyntaxTree* frame) : AbstractSyntaxTree(frame) {}

void BlockNode::write(std::vector<const Instruction*>& instructions) const {
    for (auto node : nodes) {
        node->write(instructions);
    }
}

void BlockNode::add(const AbstractSyntaxTree* node) {
    nodes.push_back(node);
}

BinaryOperationNode::BinaryOperationNode(
    const AbstractSyntaxTree* frame,
    const AbstractSyntaxTree* left,
    const AbstractSyntaxTree* right,
    uint8_t operation
) : AbstractSyntaxTree(frame) {
    this->left = left;
    this->right = right;
    this->operation = operation;
}

void BinaryOperationNode::write(std::vector<const Instruction*>& instructions) const {
    left->write(instructions);
    right->write(instructions);
    switch (operation) {
        case ADD:
            instructions.push_back(new AddInstruction());
            break;
        case SUB:
            instructions.push_back(new SubInstruction());
            break;
        case MUL:
            instructions.push_back(new MulInstruction());
            break;
        case DIV:
            instructions.push_back(new DivInstruction());
            break;
        case MOD:
            instructions.push_back(new ModInstruction());
            break;
        case AND:
            instructions.push_back(new AndInstruction());
            break;
        case OR:
            instructions.push_back(new OrInstruction());
            break;
        case XOR:
            instructions.push_back(new XorInstruction());
            break;
        case LT:
            instructions.push_back(new LtInstruction());
            break;
        case LTE:
            instructions.push_back(new LteInstruction());
            break;
        case GT:
            instructions.push_back(new GtInstruction());
            break;
        case GTE:
            instructions.push_back(new GteInstruction());
            break;
        case EQ:
            instructions.push_back(new EqInstruction());
            break;
        case NOT_EQ:
            instructions.push_back(new NotEqInstruction());
            break;
        default:
            std::cout << "Unrecognized operation: " << (int) operation << std::endl;
            exit(1);
    }
}

IfNode::IfNode(
    AbstractSyntaxTree* frame,
    const AbstractSyntaxTree* condition,
    const AbstractSyntaxTree* if_block,
    const AbstractSyntaxTree* else_block
) : AbstractSyntaxTree(frame) {
    this->condition = condition;
    this->if_block = if_block;
    this->else_block = else_block;
}

void IfNode::write(std::vector<const Instruction*>& instructions) const {
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction(0);
    instructions.push_back(jump);
    if_block->write(instructions);
    jump->set_address(ast::count_bytes(instructions));
    if (else_block != nullptr) {
        else_block->write(instructions);
    }
}

WhileNode::WhileNode(
    const AbstractSyntaxTree* frame,
    const AbstractSyntaxTree* condition,
    const AbstractSyntaxTree* body
) : AbstractSyntaxTree(frame) {
    this->condition = condition;
    this->body = body;
}

void WhileNode::write(std::vector<const Instruction*>& instructions) const {
    uint64_t while_address = ast::count_bytes(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction(0);
    instructions.push_back(jump);
    body->write(instructions);
    instructions.push_back(new JumpInstruction(while_address));
    jump->set_address(ast::count_bytes(instructions));
}

ForNode::ForNode(
    const AbstractSyntaxTree* frame,
    const AbstractSyntaxTree* init,
    const AbstractSyntaxTree* condition,
    const AbstractSyntaxTree* increment,
    const AbstractSyntaxTree* body
) : AbstractSyntaxTree(frame) {
    this->init = init;
    this->condition = condition;
    this->increment = increment;
    this->body = body;
}

void ForNode::write(std::vector<const Instruction*>& instructions) const {
    init->write(instructions);
    uint64_t if_address = ast::count_bytes(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction(0);
    instructions.push_back(jump);
    body->write(instructions);
    increment->write(instructions);
    instructions.push_back(new JumpInstruction(if_address));
    jump->set_address(ast::count_bytes(instructions));
}

PrintNode::PrintNode(const AbstractSyntaxTree* frame, const AbstractSyntaxTree* expression) : AbstractSyntaxTree(frame) {
    this->expression = expression;
}

void PrintNode::write(std::vector<const Instruction*>& instructions) const {
    expression->write(instructions);
    instructions.push_back(new PrintInstruction());
}

PrintStringNode::PrintStringNode(const AbstractSyntaxTree* frame, const std::string& str) : AbstractSyntaxTree(frame) {
    this->str = str;
}

void PrintStringNode::write(std::vector<const Instruction*>& instructions) const {
    for (auto it = str.rbegin(); it < str.rend(); it++) {
        instructions.push_back(new PushInstruction(*it));
    }
    for (int i = 0; i < str.size(); i++) {
        instructions.push_back(new PrintCharInstruction());
    }
}

HaltNode::HaltNode(const AbstractSyntaxTree* frame) : AbstractSyntaxTree(frame) {}

void HaltNode::write(std::vector<const Instruction*>& instructions) const {
    instructions.push_back(new HaltInstruction());
}

uint64_t ast::count_bytes(const std::vector<const Instruction*>& instructions) {
    uint64_t size = 0;
    for (const Instruction* instruction : instructions) {
        size += instruction->size();
    }
    return size;
}

std::vector<uint8_t> ast::to_bytes(const AbstractSyntaxTree* node) {
    std::vector<const Instruction*> instructions;
    node->write(instructions);

    std::vector<uint8_t> bytes;
    for (const Instruction* instruction : instructions) {
        instruction->write(bytes);
    }

    for (const Instruction* instruction : instructions) {
        delete instruction;
    }

    return bytes;
}

std::vector<std::pair<uint64_t, std::string>> ast::to_asm(const AbstractSyntaxTree* node) {
    std::vector<const Instruction*> instructions;
    node->write(instructions);

    std::vector<std::pair<uint64_t, std::string>> strings;
    uint64_t index = 0;
    for (const Instruction* instruction : instructions) {
        strings.push_back(std::make_pair(index, instruction->to_string()));
        index += instruction->size();
    }

    for (const Instruction* instruction : instructions) {
        delete instruction;
    }

    return strings;
}