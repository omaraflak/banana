#include "parser.h"
#include <sstream>
#include <stack>
#include <map>
#include <set>

namespace parser {
std::map<TokenType, AstBinaryOperation> BIN_OP = {
    {TOKEN_EQUAL_EQUAL, AST_EQ},
    {TOKEN_BANG_EQUAL, AST_NOT_EQ},
    {TOKEN_LESS, AST_LT},
    {TOKEN_LESS_EQUAL, AST_LTE},
    {TOKEN_GREATER, AST_GT},
    {TOKEN_GREATER_EQUAL, AST_GTE},
    {TOKEN_PLUS, AST_ADD},
    {TOKEN_MINUS, AST_SUB},
    {TOKEN_STAR, AST_MUL},
    {TOKEN_SLASH, AST_DIV},
    {TOKEN_MOD, AST_MOD},
    {TOKEN_XOR, AST_XOR},
    {TOKEN_AMPERSAND, AST_BIN_AND},
    {TOKEN_PIPE, AST_BIN_OR},
    {TOKEN_AND, AST_BOOL_AND},
    {TOKEN_OR, AST_BOOL_OR},
};

std::set<TokenType> KEYWORDS = {
    TOKEN_FUN, TOKEN_RETURN, TOKEN_IF, TOKEN_ELSE,
    TOKEN_FOR, TOKEN_WHILE, TOKEN_AND, TOKEN_OR,
    TOKEN_PRINT, TOKEN_VAR
};

typedef std::map<std::string, std::shared_ptr<VariableNode>> Identifiers;

typedef struct {
    std::map<std::shared_ptr<AbstractSyntaxTree>, Identifiers> identifiers;
    std::vector<std::shared_ptr<AbstractSyntaxTree>> scope_stack;
} Frame;

typedef struct {
    std::vector<Token> tokens;
    int current;
    std::map<std::shared_ptr<AbstractSyntaxTree>, Frame> frames;
    std::stack<std::shared_ptr<AbstractSyntaxTree>> frame_stack;
} Parser;

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> statement(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> expression_statement(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> assign_statement(Parser& parser, const bool& expect_semicolon = true);
std::shared_ptr<AbstractSyntaxTree> assign_expression(Parser& parser);
bool match_assign(Parser& parser);

void print_error(const Parser& parser, const std::string& message);
std::shared_ptr<BlockNode> block(Parser& parser);

std::shared_ptr<LiteralNode> literal(const uint64_t& value) {
    return std::shared_ptr<LiteralNode>(new LiteralNode(value));
}

std::string token_as_string(const Token& token) {
    std::stringstream ss;
    for (int i = 0; i < token.length; i++) {
        ss << token.start[i];
    }
    return ss.str();
}

uint64_t token_as_long(const Token& token) {
    return stol(token_as_string(token));
}

bool eof(const Parser& parser) {
    return parser.current == parser.tokens.size();
}

bool check(const Parser& parser, TokenType type) {
    if (eof(parser)) {
        return false;
    }
    return parser.tokens[parser.current].type == type;
}

Token peek(const Parser& parser) {
    return parser.tokens[parser.current];
}

Token previous(const Parser& parser, const int& offset = 1) {
    return parser.tokens[parser.current - offset];
}

Token advance(Parser& parser) {
    if (!eof(parser)) {
        parser.current++;
    }
    return previous(parser);
}

Token consume(Parser& parser, const TokenType& type, const std::string& message) {
    if (check(parser, type)) {
        return advance(parser);
    }
    print_error(parser, message);
    exit(1);
}

bool match(Parser& parser, const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (check(parser, type)) {
            advance(parser);
            return true;
        }
    }
    return false;
}

bool match_sequence(Parser& parser, const std::vector<TokenType>& types) {
    if (parser.current + types.size() >= parser.tokens.size()) {
        return false;
    }
    for (int i = 0; i < types.size(); i++) {
        if (parser.tokens[parser.current + i].type != types[i]) {
            return false;
        }
    }
    parser.current += types.size();
    return true;
}

void print_error(const Parser& parser, const std::string& message) {
    std::cout << "Line " << peek(parser).line << ": " << message << std::endl;
}

std::shared_ptr<AbstractSyntaxTree> current_frame(const Parser& parser) {
    if (parser.frame_stack.empty()) {
        print_error(parser, "Could not find the current frame!");
        exit(1);
    }
    return parser.frame_stack.top();
}

std::shared_ptr<AbstractSyntaxTree> current_scope(const Parser& parser) {
    const Frame& frame = parser.frames.at(current_frame(parser));
    if (frame.scope_stack.empty()) {
        print_error(parser, "Could not find the current scope!");
        exit(1);
    }
    return frame.scope_stack.back();
}

void push_frame(Parser& parser, const std::shared_ptr<AbstractSyntaxTree>& frame) {
    parser.frame_stack.push(frame);
    parser.frames[frame] = Frame();
}

void pop_frame(Parser& parser) {
    parser.frames.erase(parser.frame_stack.top());
    parser.frame_stack.pop();
}

void push_scope(Parser& parser, const std::shared_ptr<AbstractSyntaxTree>& scope) {
    Frame& frame = parser.frames[current_frame(parser)];
    frame.scope_stack.push_back(scope);
    frame.identifiers[scope] = Identifiers();
}

void pop_scope(Parser& parser) {
    Frame& frame = parser.frames[current_frame(parser)];
    frame.identifiers.erase(current_scope(parser));
    frame.scope_stack.pop_back();
}

std::shared_ptr<VariableNode> get_variable_by_name(const Parser& parser, const std::string& name) {
    const Frame& frame = parser.frames.at(current_frame(parser));
    for (const auto& scope : frame.scope_stack) {
        const auto& mapping = frame.identifiers.at(scope);
        if (mapping.find(name) != mapping.end()) {
            return mapping.at(name);
        }
    }
    print_error(parser, "Could not find '" + name + "' in current scope.");
    exit(1);
}

std::shared_ptr<VariableNode> new_variable(Parser& parser, const std::string& name) {
    std::shared_ptr<AbstractSyntaxTree> frame = current_frame(parser);
    std::shared_ptr<AbstractSyntaxTree> scope = current_scope(parser);
    std::shared_ptr<VariableNode> variable = std::shared_ptr<VariableNode>(new VariableNode(frame));
    auto& mapping = parser.frames[frame].identifiers[scope];
    if (mapping.find(name) != mapping.end()) {
        print_error(parser, "Identifier '" + name + "' already declared in the scope.");
        exit(1);
    }
    mapping[name] = variable;
    return variable;
}

std::shared_ptr<AbstractSyntaxTree> primary(Parser& parser) {
    if (match(parser, {TOKEN_NUMBER, TOKEN_STRING})) {
        Token token = previous(parser);
        return std::shared_ptr<LiteralNode>(new LiteralNode(token_as_long(token)));
    }
    if (match(parser, {TOKEN_IDENTIFIER})) {
        Token token = previous(parser);
        std::string name = token_as_string(token);
        return get_variable_by_name(parser, name);
    }
    if (match(parser, {TOKEN_LEFT_PAREN})) {
        std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
        consume(parser, TOKEN_RIGHT_PAREN, "Could not find closing ).");
        return exp;
    }
    std::cout << "Reached end of primary expression without any matches." << std::endl;
    exit(1);
}

std::shared_ptr<AbstractSyntaxTree> unary(Parser& parser) {
    if (match(parser, {TOKEN_MINUS, TOKEN_BANG})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> exp = unary(parser);
        std::shared_ptr<LiteralNode> zero = std::shared_ptr<LiteralNode>(new LiteralNode(0));
        return std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(zero, exp, op));
    }
    return primary(parser);
}

std::shared_ptr<AbstractSyntaxTree> factor(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = unary(parser);
    while (match(parser, {TOKEN_STAR, TOKEN_SLASH, TOKEN_MOD, TOKEN_XOR, TOKEN_AMPERSAND, TOKEN_PIPE})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = unary(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> term(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = factor(parser);
    while (match(parser, {TOKEN_PLUS, TOKEN_MINUS})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = factor(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> comparison(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = term(parser);
    while (match(parser, {TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = term(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> equality(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = comparison(parser);
    while (match(parser, {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = comparison(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> bool_logic(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = equality(parser);
    while (match(parser, {TOKEN_AND, TOKEN_OR})) {
        AstBinaryOperation op = BIN_OP[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = equality(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser) {
    return bool_logic(parser);
}

std::shared_ptr<AbstractSyntaxTree> print_statement(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after print value.");
    return std::shared_ptr<PrintNode>(new PrintNode(exp));
}

std::shared_ptr<AbstractSyntaxTree> var_statement(Parser& parser) {
    Token p = peek(parser);
    if (KEYWORDS.find(p.type) != KEYWORDS.end()) {
        print_error(parser, "'" + token_as_string(p) + "' is a reserved identifier.");
        exit(1);
    }
    Token id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier.");
    std::shared_ptr<VariableNode> variable = new_variable(parser, token_as_string(id));
    consume(parser, TOKEN_EQUAL, "Expected '=' sign after identifier.");
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression.");
    return std::shared_ptr<AssignNode>(new AssignNode(variable, exp));
}

std::shared_ptr<AbstractSyntaxTree> if_statement(Parser& parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Missing '(' after 'if'.");
    std::shared_ptr<AbstractSyntaxTree> condition = expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Missing ')' after 'if' condition.");
    std::shared_ptr<BlockNode> if_block = block(parser);
    std::shared_ptr<BlockNode> else_block = nullptr;
    if (check(parser, TOKEN_ELSE)) {
        advance(parser);
        else_block = block(parser);
    }
    return std::shared_ptr<IfNode>(new IfNode(condition, if_block, else_block));
}

std::shared_ptr<AbstractSyntaxTree> while_statement(Parser& parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Missing '(' after 'while'.");
    std::shared_ptr<AbstractSyntaxTree> condition = expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Missing ')' after 'while' condition.");
    std::shared_ptr<BlockNode> while_block = block(parser);
    return std::shared_ptr<WhileNode>(new WhileNode(condition, while_block));
}

std::shared_ptr<AbstractSyntaxTree> for_statement(Parser& parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'for'.");
    std::shared_ptr<AbstractSyntaxTree> init = statement(parser);
    std::shared_ptr<AbstractSyntaxTree> condition = expression_statement(parser);
    std::shared_ptr<AbstractSyntaxTree> increment = assign_expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after 'for' increment.");
    std::shared_ptr<BlockNode> for_block = block(parser);
    return std::shared_ptr<ForNode>(new ForNode(init, condition, increment, for_block));
}

std::shared_ptr<AbstractSyntaxTree> assign_expression(Parser& parser) {
    if (match_assign(parser)) {
        return assign_statement(parser, /* expect_semicolon */ false);
    }
    print_error(parser, "Expected assign expression.");
    exit(1);
}

std::shared_ptr<AbstractSyntaxTree> assign_statement(Parser& parser, const bool& expect_semicolon) {
    Token assign = previous(parser);
    Token id = previous(parser, 2);
    std::shared_ptr<VariableNode> variable = get_variable_by_name(parser, token_as_string(id));
    std::shared_ptr<AbstractSyntaxTree> exp;
    switch (assign.type) {
        case TOKEN_EQUAL:
            exp = expression(parser);
            break;
        case TOKEN_PLUS_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_ADD));
            break;
        case TOKEN_MINUS_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_SUB));
            break;
        case TOKEN_STAR_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_MUL));
            break;
        case TOKEN_SLASH_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_DIV));
            break;
        case TOKEN_MOD_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_MOD));
            break;
        case TOKEN_XOR_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_XOR));
            break;
        case TOKEN_AMPERSAND_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_BIN_AND));
            break;
        case TOKEN_PIPE_EQUAL:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, expression(parser), AST_BIN_OR));
            break;
        case TOKEN_PLUS_PLUS:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, literal(1), AST_ADD));
            break;
        case TOKEN_MINUS_MINUS:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, literal(1), AST_SUB));
            break;
        default:
            print_error(parser, "Could not recognize assignment type!");
            exit(1);
    }
    if (expect_semicolon) {
        consume(parser, TOKEN_SEMICOLON, "Expected ';' after statement.");
    }
    return std::shared_ptr<AssignNode>(new AssignNode(variable, exp));
}

std::shared_ptr<AbstractSyntaxTree> expression_statement(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression.");
    return exp;
}

bool match_assign(Parser& parser) {
    return match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_PLUS_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_MINUS_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_STAR_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_SLASH_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_MOD_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_AMPERSAND_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_PIPE_EQUAL}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_PLUS_PLUS}) ||
        match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_MINUS_MINUS});
}

std::shared_ptr<AbstractSyntaxTree> statement(Parser& parser) {
    if (match(parser, {TOKEN_PRINT})) {
        return print_statement(parser);
    }
    if (match(parser, {TOKEN_VAR})) {
        return var_statement(parser);
    }
    if (match(parser, {TOKEN_IF})) {
        return if_statement(parser);
    }
    if (match(parser, {TOKEN_WHILE})) {
        return while_statement(parser);
    }
    if (match(parser, {TOKEN_FOR})) {
        return for_statement(parser);
    }
    if (match_assign(parser)) {
        return assign_statement(parser);
    }
    return expression_statement(parser);
}

std::shared_ptr<BlockNode> block(Parser& parser) {
    consume(parser, TOKEN_LEFT_BRACE, "Missing '{' before block.");
    std::shared_ptr<BlockNode> block = std::shared_ptr<BlockNode>(new BlockNode());
    push_scope(parser, block);
    const Frame& frame = parser.frames.at(current_frame(parser));
    const int nest_level = frame.scope_stack.size();
    while (
        !eof(parser) && (
            frame.scope_stack.size() != nest_level ||
            (frame.scope_stack.size() == nest_level && !check(parser, TOKEN_RIGHT_BRACE))
        )
    ) {
        block->add(statement(parser));
    }
    if (eof(parser)) {
        print_error(parser, "Missing '} after block.");
    }
    consume(parser, TOKEN_RIGHT_BRACE, "Missing '}' after block.");
    pop_scope(parser);
    return block;
}

std::shared_ptr<AbstractSyntaxTree> program(Parser& parser) {
    std::shared_ptr<BlockNode> root = std::shared_ptr<BlockNode>(new BlockNode());
    push_frame(parser, root);
    push_scope(parser, root);
    while (!eof(parser)) {
        root->add(statement(parser));
    }
    pop_scope(parser);
    pop_frame(parser);
    return root;
}
}

std::shared_ptr<AbstractSyntaxTree> parser::parse(const std::vector<Token>& tokens) {
    Parser parser;
    parser.current = 0;
    parser.tokens = tokens;
    return program(parser);
}