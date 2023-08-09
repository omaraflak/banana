#include "ast.h"

AbstractSyntaxTree::AbstractSyntaxTree() {
    written = false;
}

void AbstractSyntaxTree::write(std::vector<const Instruction*>& instructions) {
    program_address = ast::count_bytes(instructions);
    written = true;
}

uint64_t AbstractSyntaxTree::get_program_address() const {
    return program_address;
}

bool AbstractSyntaxTree::is_written() const {
    return written;
}

LiteralNode::LiteralNode(const uint64_t& value) : AbstractSyntaxTree() {
    this->value = value;
}

void LiteralNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new PushInstruction(value));
}

VariableNode::VariableNode(const AbstractSyntaxTree* frame) {
    this->frame = frame;
    if (frame == nullptr) {
        std::cout << "Trying to get address without a frame" << std::endl;
        exit(1);
    }
    if (latest_address.find(frame) == latest_address.end()) {
        latest_address[frame] = 0;
    }
    address = latest_address[frame];
    latest_address[frame] += SIZE_OF_LONG;
}

void VariableNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new LoadInstruction(address));
}

uint64_t VariableNode::get_address() const {
    return address;
}

AssignNode::AssignNode(
    VariableNode* node,
    AbstractSyntaxTree* expression
) : AbstractSyntaxTree() {
    this->node = node;
    this->expression = expression;
}

void AssignNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    expression->write(instructions);
    instructions.push_back(new StoreInstruction(node->get_address()));
}

BlockNode::BlockNode() : AbstractSyntaxTree() {}

void BlockNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    for (auto node : nodes) {
        node->write(instructions);
    }
}

void BlockNode::add(AbstractSyntaxTree* node) {
    nodes.push_back(node);
}

BinaryOperationNode::BinaryOperationNode(
    AbstractSyntaxTree* left,
    AbstractSyntaxTree* right,
    const AstBinaryOperation& operation
) : AbstractSyntaxTree() {
    this->left = left;
    this->right = right;
    this->operation = operation;
}

void BinaryOperationNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    left->write(instructions);
    right->write(instructions);
    switch (operation) {
        case AST_ADD:
            instructions.push_back(new AddInstruction());
            break;
        case AST_SUB:
            instructions.push_back(new SubInstruction());
            break;
        case AST_MUL:
            instructions.push_back(new MulInstruction());
            break;
        case AST_DIV:
            instructions.push_back(new DivInstruction());
            break;
        case AST_MOD:
            instructions.push_back(new ModInstruction());
            break;
        case AST_AND:
            instructions.push_back(new AndInstruction());
            break;
        case AST_OR:
            instructions.push_back(new OrInstruction());
            break;
        case AST_XOR:
            instructions.push_back(new XorInstruction());
            break;
        case AST_LT:
            instructions.push_back(new LtInstruction());
            break;
        case AST_LTE:
            instructions.push_back(new LteInstruction());
            break;
        case AST_GT:
            instructions.push_back(new GtInstruction());
            break;
        case AST_GTE:
            instructions.push_back(new GteInstruction());
            break;
        case AST_EQ:
            instructions.push_back(new EqInstruction());
            break;
        case AST_NOT_EQ:
            instructions.push_back(new NotEqInstruction());
            break;
        default:
            std::cout << "Unrecognized operation: " << (int) operation << std::endl;
            exit(1);
    }
}

IfNode::IfNode(
    AbstractSyntaxTree* condition,
    AbstractSyntaxTree* if_block,
    AbstractSyntaxTree* else_block
) : AbstractSyntaxTree() {
    this->condition = condition;
    this->if_block = if_block;
    this->else_block = else_block;
}

void IfNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
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
    AbstractSyntaxTree* condition,
    AbstractSyntaxTree* body
) : AbstractSyntaxTree() {
    this->condition = condition;
    this->body = body;
}

void WhileNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    uint64_t while_address = ast::count_bytes(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction(0);
    instructions.push_back(jump);
    body->write(instructions);
    instructions.push_back(new JumpInstruction(while_address));
    jump->set_address(ast::count_bytes(instructions));
}

ForNode::ForNode(
    AbstractSyntaxTree* init,
    AbstractSyntaxTree* condition,
    AbstractSyntaxTree* increment,
    AbstractSyntaxTree* body
) : AbstractSyntaxTree() {
    this->init = init;
    this->condition = condition;
    this->increment = increment;
    this->body = body;
}

void ForNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
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

PrintNode::PrintNode(AbstractSyntaxTree* expression, const std::string& end) : AbstractSyntaxTree() {
    this->expression = expression;
    this->end = end;
}

void PrintNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    expression->write(instructions);
    instructions.push_back(new PrintInstruction());
    PrintStringNode(end).write(instructions);
}

PrintStringNode::PrintStringNode(const std::string& str) : AbstractSyntaxTree() {
    this->str = str;
}

void PrintStringNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    for (auto it = str.rbegin(); it < str.rend(); it++) {
        instructions.push_back(new PushInstruction(*it));
    }
    for (int i = 0; i < str.size(); i++) {
        instructions.push_back(new PrintCharInstruction());
    }
}

FunctionNode::FunctionNode(
    AbstractSyntaxTree* body,
    const std::vector<VariableNode*>& parameters
) : AbstractSyntaxTree() {
    this->body = body;
    this->parameters.insert(this->parameters.end(), parameters.begin(), parameters.end());
}

void FunctionNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    for (auto parameter : parameters) {
        instructions.push_back(new StoreInstruction(parameter->get_address()));
    }
    body->write(instructions);
}

uint64_t FunctionNode::get_parameters_count() const {
    return parameters.size();
}

CallNode::CallNode(
    FunctionNode* function,
    const std::vector<AbstractSyntaxTree*>& values
) : AbstractSyntaxTree() {
    this->function = function;
    this->values.insert(this->values.end(), values.begin(), values.end());
}

void CallNode::write(std::vector<const Instruction*>& instructions) {
    if (!function->is_written()) {
        std::cout << "Trying to call a function not yet written (declared)." << std::endl;
        exit(1);
    }
    if (values.size() != function->get_parameters_count()) {
        std::cout << "Function accepts " << function->get_parameters_count() << " parameters, but " << values.size() << " were passed." << std::endl;
        exit(1);
    }
    AbstractSyntaxTree::write(instructions);
    for (auto it = values.rbegin(); it < values.rend(); it++) {
        (*it)->write(instructions);
    }
    instructions.push_back(new CallInstruction(function->get_program_address(), function->get_parameters_count()));
}

ReturnNode::ReturnNode(const std::vector<AbstractSyntaxTree*>& values) : AbstractSyntaxTree() {
    this->values = values;
}

void ReturnNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    for (auto value : values) {
        value->write(instructions);
    }
    instructions.push_back(new RetInstruction(values.size()));
}

HaltNode::HaltNode() : AbstractSyntaxTree() {}

void HaltNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new HaltInstruction());
}

uint64_t ast::count_bytes(const std::vector<const Instruction*>& instructions) {
    uint64_t size = 0;
    for (const Instruction* instruction : instructions) {
        size += instruction->size();
    }
    return size;
}

std::vector<std::unique_ptr<const Instruction>> ast::to_instructions(AbstractSyntaxTree* root, AbstractSyntaxTree* main) {
    std::vector<const Instruction*> instructions;
    if (main == nullptr) {
        root->write(instructions);    
    } else {
        JumpInstruction* jump = new JumpInstruction((uint64_t) 0);
        instructions.push_back(jump);
        root->write(instructions);
        jump->set_address(main->get_program_address());
    }
    instructions.push_back(new HaltInstruction());

    std::vector<std::unique_ptr<const Instruction>> instructions_ptr;
    for (auto ptr : instructions) {
        instructions_ptr.push_back(std::unique_ptr<const Instruction>(ptr));
    }
    return instructions_ptr;
}

std::vector<uint8_t> ast::to_bytes(const std::vector<std::unique_ptr<const Instruction>>& instructions) {
    std::vector<uint8_t> bytes;
    for (const auto& instruction : instructions) {
        instruction->write(bytes);
    }
    return bytes;
}

std::vector<std::pair<uint64_t, std::string>> ast::to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions) {
    std::vector<std::pair<uint64_t, std::string>> strings;
    uint64_t index = 0;
    for (const auto& instruction : instructions) {
        strings.push_back(std::make_pair(index, instruction->to_string()));
        index += instruction->size();
    }
    return strings;
}