#include "ast.h"
#include "var.h"
#include <map>

namespace ast {
std::map<AstVarType, var::DataType> AST_TO_VAR = {
    {ast::BOOL, var::BOOL},
    {ast::CHAR, var::CHAR},
    {ast::INT, var::INT},
    {ast::LONG, var::LONG},
};
}

AbstractSyntaxTree::AbstractSyntaxTree() {
    written = false;
}

void AbstractSyntaxTree::write(std::vector<const Instruction*>& instructions) {
    program_address = ast::count_bytes(instructions);
    written = true;
}

Address AbstractSyntaxTree::get_program_address() const {
    return program_address;
}

bool AbstractSyntaxTree::is_written() const {
    return written;
}

LiteralNode::LiteralNode(const Var& value) : AbstractSyntaxTree() {
    this->value = value;
}

void LiteralNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new PushInstruction(value));
}

VariableNode::VariableNode(const std::shared_ptr<const AbstractSyntaxTree>& frame, const ast::AstVarType& type) {
    this->frame = frame;
    this->type = type;
    if (frame == nullptr) {
        std::cout << "Trying to get address without a frame" << std::endl;
        exit(1);
    }
    if (latest_address.find(frame) == latest_address.end()) {
        latest_address[frame] = 0;
    }
    address = latest_address[frame];
    latest_address[frame]++;
}

void VariableNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new LoadInstruction(address));
}

Address VariableNode::get_address() const {
    return address;
}

ast::AstVarType VariableNode::get_type() const {
    return type;
}

AssignNode::AssignNode(
    const std::shared_ptr<VariableNode>& node,
    const std::shared_ptr<AbstractSyntaxTree>& expression
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

void BlockNode::add(const std::shared_ptr<AbstractSyntaxTree>& node) {
    nodes.push_back(node);
}

BinaryOperationNode::BinaryOperationNode(
    const std::shared_ptr<AbstractSyntaxTree>& left,
    const std::shared_ptr<AbstractSyntaxTree>& right,
    const ast::AstBinaryOperation& operation
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
        case ast::ADD:
            instructions.push_back(new AddInstruction());
            break;
        case ast::SUB:
            instructions.push_back(new SubInstruction());
            break;
        case ast::MUL:
            instructions.push_back(new MulInstruction());
            break;
        case ast::DIV:
            instructions.push_back(new DivInstruction());
            break;
        case ast::MOD:
            instructions.push_back(new ModInstruction());
            break;
        case ast::XOR:
            instructions.push_back(new XorInstruction());
            break;
        case ast::BIN_AND:
            instructions.push_back(new BinaryAndInstruction());
            break;
        case ast::BIN_OR:
            instructions.push_back(new BinaryOrInstruction());
            break;
        case ast::LT:
            instructions.push_back(new LtInstruction());
            break;
        case ast::LTE:
            instructions.push_back(new LteInstruction());
            break;
        case ast::GT:
            instructions.push_back(new GtInstruction());
            break;
        case ast::GTE:
            instructions.push_back(new GteInstruction());
            break;
        case ast::EQ:
            instructions.push_back(new EqInstruction());
            break;
        case ast::NOT_EQ:
            instructions.push_back(new NotEqInstruction());
            break;
        case ast::BOOL_AND:
            instructions.push_back(new BooleanAndInstruction());
            break;
        case ast::BOOL_OR:
            instructions.push_back(new BooleanOrInstruction());
            break;
        default:
            std::cout << "Unrecognized binary operation: " << (int) operation << std::endl;
            exit(1);
    }
}

BooleanNotNode::BooleanNotNode(const std::shared_ptr<AbstractSyntaxTree>& expression) : AbstractSyntaxTree() {
    this->expression = expression;
}

void BooleanNotNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    expression->write(instructions);
    instructions.push_back(new BooleanNotInstruction());
}

BinaryNotNode::BinaryNotNode(const std::shared_ptr<AbstractSyntaxTree>& expression) : AbstractSyntaxTree() {
    this->expression = expression;
}

void BinaryNotNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    expression->write(instructions);
    instructions.push_back(new BinaryNotInstruction());
}

IfNode::IfNode(
    const std::shared_ptr<AbstractSyntaxTree>& condition,
    const std::shared_ptr<AbstractSyntaxTree>& if_block,
    const std::shared_ptr<AbstractSyntaxTree>& else_block
) : AbstractSyntaxTree() {
    this->condition = condition;
    this->if_block = if_block;
    this->else_block = else_block;
}

void IfNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction();
    instructions.push_back(jump);
    if_block->write(instructions);
    if (else_block != nullptr) {
        JumpInstruction* jump_to_end = new JumpInstruction();
        instructions.push_back(jump_to_end);
        jump->set_address(ast::count_bytes(instructions));
        else_block->write(instructions);
        jump_to_end->set_address(ast::count_bytes(instructions));
    } else {
        jump->set_address(ast::count_bytes(instructions));
    }
}

WhileNode::WhileNode(
    const std::shared_ptr<AbstractSyntaxTree>& condition,
    const std::shared_ptr<AbstractSyntaxTree>& body
) : AbstractSyntaxTree() {
    this->condition = condition;
    this->body = body;
}

void WhileNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    Address while_address = ast::count_bytes(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction();
    instructions.push_back(jump);
    body->write(instructions);
    instructions.push_back(new JumpInstruction(while_address));
    jump->set_address(ast::count_bytes(instructions));
}

ForNode::ForNode(
    const std::shared_ptr<AbstractSyntaxTree>& init,
    const std::shared_ptr<AbstractSyntaxTree>& condition,
    const std::shared_ptr<AbstractSyntaxTree>& increment,
    const std::shared_ptr<AbstractSyntaxTree>& body
) : AbstractSyntaxTree() {
    this->init = init;
    this->condition = condition;
    this->increment = increment;
    this->body = body;
}

void ForNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    init->write(instructions);
    Address if_address = ast::count_bytes(instructions);
    condition->write(instructions);
    JumpIfFalseInstruction* jump = new JumpIfFalseInstruction();
    instructions.push_back(jump);
    body->write(instructions);
    increment->write(instructions);
    instructions.push_back(new JumpInstruction(if_address));
    jump->set_address(ast::count_bytes(instructions));
}

PrintNode::PrintNode(const std::shared_ptr<AbstractSyntaxTree>& expression, const std::string& end) : AbstractSyntaxTree() {
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
        instructions.push_back(new PushInstruction(var::create_char(*it)));
        instructions.push_back(new PrintInstruction());
    }
}

FunctionNode::FunctionNode(const bool& is_main) : AbstractSyntaxTree() {
    this->is_main = is_main;
}

void FunctionNode::write(std::vector<const Instruction*>& instructions) {
    JumpInstruction* jump = nullptr;
    if (!is_main) {
        jump = new JumpInstruction();
        instructions.push_back(jump);
    }

    AbstractSyntaxTree::write(instructions);
    for (auto parameter : parameters) {
        instructions.push_back(new StoreInstruction(parameter->get_address()));
    }
    body->write(instructions);

    if (!is_main) {
        jump->set_address(ast::count_bytes(instructions));
    }
}

std::vector<std::shared_ptr<const VariableNode>> FunctionNode::get_parameters() const {
    return parameters;
}

uint8_t FunctionNode::get_parameters_count() const {
    return parameters.size();
}

ast::AstVarType FunctionNode::get_return_type() const {
    return return_type;
}

void FunctionNode::set_body(const std::shared_ptr<AbstractSyntaxTree>& body) {
    this->body = body;
}

void FunctionNode::set_parameters(const std::vector<std::shared_ptr<VariableNode>>& parameters) {
    this->parameters.clear();
    this->parameters.insert(this->parameters.end(), parameters.begin(), parameters.end());
}

void FunctionNode::set_return_type(const ast::AstVarType& return_type) {
    this->return_type = return_type;
}

CallNode::CallNode(
    const std::shared_ptr<FunctionNode>& function,
    const std::vector<std::shared_ptr<AbstractSyntaxTree>>& values
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

ReturnNode::ReturnNode(const std::vector<std::shared_ptr<AbstractSyntaxTree>>& values) : AbstractSyntaxTree() {
    this->values = values;
}

void ReturnNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    for (auto value : values) {
        value->write(instructions);
    }
    instructions.push_back(new RetInstruction(values.size()));
}

ConvertNode::ConvertNode(
    const std::shared_ptr<AbstractSyntaxTree>& expression,
    const ast::AstVarType& type
) : AbstractSyntaxTree() {
    this->expression = expression;
    this->type = type;
}

void ConvertNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    expression->write(instructions);
    instructions.push_back(new ConvertInstruction(ast::AST_TO_VAR.at(type)));
}

NativeNode::NativeNode(
    const std::string& function_name,
    const std::vector<std::shared_ptr<VariableNode>>& values
) : AbstractSyntaxTree() {
    this->function_name = function_name;
    this->values.insert(this->values.begin(), values.begin(), values.end());
}

void NativeNode::write(std::vector<const Instruction*>& instructions) {
    for (const auto& value : values) {
        value->write(instructions);
    }
    instructions.push_back(new NativeInstruction(function_name));
}

HaltNode::HaltNode() : AbstractSyntaxTree() {}

void HaltNode::write(std::vector<const Instruction*>& instructions) {
    AbstractSyntaxTree::write(instructions);
    instructions.push_back(new HaltInstruction());
}

Address ast::count_bytes(const std::vector<const Instruction*>& instructions) {
    Address size = 0;
    for (const Instruction* instruction : instructions) {
        size += instruction->size();
    }
    return size;
}

std::vector<std::unique_ptr<const Instruction>> ast::to_instructions(const std::shared_ptr<AbstractSyntaxTree>& root) {
    std::vector<const Instruction*> instructions;
    root->write(instructions);
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

std::vector<std::pair<Address, std::string>> ast::to_asm(const std::vector<std::unique_ptr<const Instruction>>& instructions) {
    std::vector<std::pair<Address, std::string>> strings;
    Address index = 0;
    for (const auto& instruction : instructions) {
        strings.push_back(std::make_pair(index, instruction->to_string()));
        index += instruction->size();
    }
    return strings;
}
