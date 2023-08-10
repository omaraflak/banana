#include "parser.h"
#include <sstream>
#include <stack>
#include <map>

namespace parser {
std::map<TokenType, AstBinaryOperation> OP_CONVERTER = {
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
    {TOKEN_AND, AST_AND},
    {TOKEN_OR, AST_OR}
};

typedef struct {
    std::vector<Token> tokens;
    int current;
    std::map<std::shared_ptr<AbstractSyntaxTree>, std::map<std::string, std::shared_ptr<VariableNode>>> identifiers;
    std::stack<std::shared_ptr<AbstractSyntaxTree>> scope_stack;
} Parser;

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser);
void print_error(const Parser& parser, const std::string& message);

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

Token previous(const Parser& parser) {
    return parser.tokens[parser.current - 1];
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

void print_error(const Parser& parser, const std::string& message) {
    std::cout << "Line " << peek(parser).line << ": " << message << std::endl;
}

std::shared_ptr<AbstractSyntaxTree> current_scope(const Parser& parser) {
    if (parser.scope_stack.size() == 0) {
        print_error(parser, "Reading variable without a scope!");
        exit(1);
    }
    return parser.scope_stack.top();
}

void push_scope(Parser& parser, const std::shared_ptr<AbstractSyntaxTree>& scope) {
    parser.scope_stack.push(scope);
    parser.identifiers[scope] = std::map<std::string, std::shared_ptr<VariableNode>>();
}

void pop_scope(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> scope = current_scope(parser);
    parser.scope_stack.pop();
    parser.identifiers.erase(scope);
}

std::shared_ptr<VariableNode> get_variable_by_name(const Parser& parser, const std::string& name) {
    std::shared_ptr<AbstractSyntaxTree> scope = current_scope(parser);
    const std::map<std::string, std::shared_ptr<VariableNode>>& mapping = parser.identifiers.at(scope);
    if (mapping.find(name) == mapping.end()) {
        print_error(parser, "Could not find '" + name + "' in current scope.");
        exit(1);
    }
    return mapping.at(name);
}

std::shared_ptr<VariableNode> new_variable(Parser& parser, const std::string& name) {
    std::shared_ptr<AbstractSyntaxTree> scope = current_scope(parser);
    std::shared_ptr<VariableNode> variable = std::shared_ptr<VariableNode>(new VariableNode(scope));
    std::map<std::string, std::shared_ptr<VariableNode>>& mapping = parser.identifiers[scope];
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
        AstBinaryOperation op = OP_CONVERTER[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> exp = unary(parser);
        std::shared_ptr<LiteralNode> zero = std::shared_ptr<LiteralNode>(new LiteralNode(0));
        return std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(zero, exp, op));
    }
    return primary(parser);
}

std::shared_ptr<AbstractSyntaxTree> factor(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = unary(parser);
    while (match(parser, {TOKEN_STAR, TOKEN_SLASH})) {
        AstBinaryOperation op = OP_CONVERTER[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = unary(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> term(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = factor(parser);
    while (match(parser, {TOKEN_PLUS, TOKEN_MINUS})) {
        AstBinaryOperation op = OP_CONVERTER[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = factor(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> comparison(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = term(parser);
    while (match(parser, {TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL})) {
        AstBinaryOperation op = OP_CONVERTER[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = term(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> equality(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> left = comparison(parser);
    while (match(parser, {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL})) {
        AstBinaryOperation op = OP_CONVERTER[previous(parser).type];
        std::shared_ptr<AbstractSyntaxTree> right = comparison(parser);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser) {
    return equality(parser);
}

std::shared_ptr<AbstractSyntaxTree> print_statement(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after print value.");
    return std::shared_ptr<PrintNode>(new PrintNode(exp));
}

std::shared_ptr<AbstractSyntaxTree> var_statement(Parser& parser) {
    Token id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier.");
    std::shared_ptr<VariableNode> variable = new_variable(parser, token_as_string(id));
    consume(parser, TOKEN_EQUAL, "Expected '=' sign after identifier.");
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression.");
    return std::shared_ptr<AssignNode>(new AssignNode(variable, exp));
}

std::shared_ptr<AbstractSyntaxTree> expression_statement(Parser& parser) {
    std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression.");
    return exp;
}

std::shared_ptr<AbstractSyntaxTree> statement(Parser& parser) {
    if (match(parser, {TOKEN_PRINT})) {
        return print_statement(parser);
    }
    if (match(parser, {TOKEN_VAR})) {
        return var_statement(parser);
    }
    return expression_statement(parser);
}

std::shared_ptr<AbstractSyntaxTree> program(Parser& parser) {
    std::shared_ptr<BlockNode> root = std::shared_ptr<BlockNode>(new BlockNode());
    push_scope(parser, root);
    while (!eof(parser)) {
        root->add(statement(parser));
    }
    pop_scope(parser);
    return root;
}
}

std::shared_ptr<AbstractSyntaxTree> parser::parse(const std::vector<Token>& tokens) {
    Parser parser;
    parser.current = 0;
    parser.tokens = tokens;
    return program(parser);
}