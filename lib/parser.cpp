#include "parser.h"
#include "var.h"
#include <sstream>
#include <stack>
#include <map>
#include <set>

namespace parser {
const std::string MAIN = "main";

const std::map<TokenType, AstBinaryOperation> BIN_OP = {
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

const std::set<TokenType> KEYWORDS = {
    TOKEN_FUN, TOKEN_RETURN, TOKEN_IF, TOKEN_ELSE,
    TOKEN_FOR, TOKEN_WHILE, TOKEN_AND, TOKEN_OR,
    TOKEN_PRINT, TOKEN_CHAR, TOKEN_SHORT, TOKEN_INT,
    TOKEN_LONG, TOKEN_BOOL
};

const std::map<TokenType, AstVarType> TOKEN_TO_TYPE = {
    {TOKEN_BOOL, AST_TYPE_BOOL},
    {TOKEN_CHAR, AST_TYPE_CHAR},
    {TOKEN_SHORT, AST_TYPE_SHORT},
    {TOKEN_INT, AST_TYPE_INT},
    {TOKEN_LONG, AST_TYPE_LONG},
};

const std::map<AstVarType, TokenType> TYPE_TO_TOKEN = {
    {AST_TYPE_BOOL, TOKEN_BOOL},
    {AST_TYPE_CHAR, TOKEN_CHAR},
    {AST_TYPE_SHORT, TOKEN_SHORT},
    {AST_TYPE_INT, TOKEN_INT},
    {AST_TYPE_LONG, TOKEN_LONG},
};

const std::set<TokenType> TYPES = {
    TOKEN_BOOL, TOKEN_CHAR, TOKEN_SHORT, TOKEN_INT, TOKEN_LONG
};

const std::map<TokenType, std::string> TYPE_NAME = {
    {TOKEN_BOOL, "BOOL"},
    {TOKEN_CHAR, "CHAR"},
    {TOKEN_SHORT, "SHORT"},
    {TOKEN_INT, "INT"},
    {TOKEN_LONG, "LONG"},
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
    std::map<std::string, std::shared_ptr<FunctionNode>> functions;
} Parser;

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> statement(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> expression_statement(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> assign_statement(Parser& parser, const bool& expect_semicolon = true);
std::shared_ptr<AbstractSyntaxTree> assign_expression(Parser& parser);
std::shared_ptr<AbstractSyntaxTree> call_statement(Parser& parser, const bool& expect_semicolon = true);
bool match_assign(Parser& parser);

void print_error(const Parser& parser, const std::string& message);
std::shared_ptr<BlockNode> block(Parser& parser);

std::shared_ptr<LiteralNode> literal(const std::string& value, const TokenType& type) {
    switch (type) {
        case TOKEN_BOOL:
            return std::shared_ptr<LiteralNode>(new LiteralNode(var::create_bool(stoi(value))));
        case TOKEN_CHAR:
            return std::shared_ptr<LiteralNode>(new LiteralNode(var::create_char(stoi(value))));
        case TOKEN_SHORT:
            return std::shared_ptr<LiteralNode>(new LiteralNode(var::create_short(stoi(value))));
        case TOKEN_INT:
            return std::shared_ptr<LiteralNode>(new LiteralNode(var::create_int(stoi(value))));
        case TOKEN_LONG:
        default:
            return std::shared_ptr<LiteralNode>(new LiteralNode(var::create_long(stol(value))));
    }
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

std::shared_ptr<VariableNode> new_variable(Parser& parser, const std::string& name, const TokenType& type) {
    std::shared_ptr<AbstractSyntaxTree> frame = current_frame(parser);
    std::shared_ptr<AbstractSyntaxTree> scope = current_scope(parser);
    std::shared_ptr<VariableNode> variable(new VariableNode(frame, TOKEN_TO_TYPE.at(type)));
    auto& mapping = parser.frames[frame].identifiers[scope];
    if (mapping.find(name) != mapping.end()) {
        print_error(parser, "Identifier '" + name + "' already declared in the scope.");
        exit(1);
    }
    mapping[name] = variable;
    return variable;
}

void register_function(Parser& parser, const std::shared_ptr<FunctionNode>& fun, const std::string& name) {
    if (parser.functions.find(name) != parser.functions.end()) {
        print_error(parser, "Function named '" + name + "' was already declared.");
        exit(1);
    }
    parser.functions[name] = fun;
}

std::shared_ptr<FunctionNode> get_function(const Parser& parser, const std::string& name) {
    if (parser.functions.find(name) == parser.functions.end()) {
        print_error(parser, "Function '" + name + "' not found.");
        exit(1);
    }
    return parser.functions.at(name);
}

std::shared_ptr<AbstractSyntaxTree> primary_expression(Parser& parser) {
    if (match(parser, {TOKEN_NUMBER, TOKEN_STRING})) {
        Token token = previous(parser);
        return literal(token.value, TOKEN_LONG);
    }
    if (match(parser, {TOKEN_IDENTIFIER})) {
        if (match(parser, {TOKEN_LEFT_PAREN})) {
            return call_statement(parser, /* expect_semicolon */ false);
        }
        Token token = previous(parser);
        std::string name = token.value;
        return get_variable_by_name(parser, name);
    }
    if (match(parser, {TOKEN_LEFT_PAREN})) {
        std::shared_ptr<AbstractSyntaxTree> exp = expression(parser);
        consume(parser, TOKEN_RIGHT_PAREN, "Could not find closing ).");
        return exp;
    }
    std::cout << "Reached end of primary expression without any matches." << std::endl;
    std::cout << "Token: '" << peek(parser).value << "'" << std::endl;
    exit(1);
}

std::shared_ptr<AbstractSyntaxTree> unary_expression(Parser& parser) {
    if (match(parser, {TOKEN_MINUS})) {
        return std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(literal("0", TOKEN_LONG), unary_expression(parser), AST_SUB));
    }
    if (match(parser, {TOKEN_BANG})) {
        return std::shared_ptr<BooleanNotNode>(new BooleanNotNode(unary_expression(parser)));
    }
    if (match(parser, {TOKEN_TILDE})) {
        return std::shared_ptr<BinaryNotNode>(new BinaryNotNode(unary_expression(parser)));
    }
    return primary_expression(parser);
}

std::shared_ptr<AbstractSyntaxTree> binary_expression(
    Parser& parser,
    const std::vector<std::vector<TokenType>>& types,
    const int& index = 0
) {
    if (index == types.size()) {
        return unary_expression(parser);
    }
    std::shared_ptr<AbstractSyntaxTree> left = binary_expression(parser, types, index + 1);
    while (match(parser, types.at(index))) {
        AstBinaryOperation op = BIN_OP.at(previous(parser).type);
        std::shared_ptr<AbstractSyntaxTree> right = binary_expression(parser, types, index + 1);
        left = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(left, right, op));
    }
    return left;
}

std::shared_ptr<AbstractSyntaxTree> expression(Parser& parser) {
    return binary_expression(
        parser,
        {
            // binary operators to be parsed in order of priority
            {TOKEN_AND, TOKEN_OR},
            {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL},
            {TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL},
            {TOKEN_PLUS, TOKEN_MINUS},
            {TOKEN_STAR, TOKEN_SLASH, TOKEN_MOD, TOKEN_XOR, TOKEN_AMPERSAND, TOKEN_PIPE}
        }
    );
}

std::shared_ptr<AbstractSyntaxTree> print_statement(Parser& parser) {
    return std::shared_ptr<PrintNode>(new PrintNode(expression_statement(parser)));
}

std::shared_ptr<AbstractSyntaxTree> var_statement(Parser& parser) {
    Token p = peek(parser);
    if (KEYWORDS.find(p.type) != KEYWORDS.end()) {
        print_error(parser, "'" + p.value + "' is a reserved identifier.");
        exit(1);
    }
    Token id = consume(parser, TOKEN_IDENTIFIER, "Expected identifier.");
    std::shared_ptr<VariableNode> variable = new_variable(parser, id.value, TOKEN_LONG);
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
    if (match(parser, {TOKEN_ELSE})) {
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

std::vector<std::shared_ptr<VariableNode>> fun_parameters(Parser& parser) {
    consume(parser, TOKEN_LEFT_PAREN, "Expected '(' before 'fun' parameters.");
    std::vector<std::shared_ptr<VariableNode>> parameters;
    TokenType previous_token = TOKEN_LEFT_PAREN;
    for (;;) {
        Token p = peek(parser);
        if (p.type == TOKEN_IDENTIFIER) {
            if (previous_token != TOKEN_LEFT_PAREN && previous_token != TOKEN_COMMA) {
                print_error(parser, "Unexpected token '" + p.value + "'.");
                exit(1);
            }
            parameters.push_back(new_variable(parser, p.value, TOKEN_LONG));
            advance(parser);
        } else if (p.type == TOKEN_COMMA) {
            if (previous_token != TOKEN_IDENTIFIER) {
                print_error(parser, "Unexpected token '" + p.value + "'.");
                exit(1);
            }
            advance(parser);
        } else if (p.type == TOKEN_RIGHT_PAREN) {
            if (previous_token != TOKEN_LEFT_PAREN && previous_token != TOKEN_IDENTIFIER) {
                print_error(parser, "Unexpected token '" + p.value + "'.");
                exit(1);
            }
            advance(parser);
            break;
        } else {
            print_error(parser, "Unexpected token '" + p.value + "'.");
            exit(1);
        }
        previous_token = p.type;
    }
    return parameters;
}

std::shared_ptr<AbstractSyntaxTree> fun_statement(Parser& parser) {
    consume(parser, TOKEN_IDENTIFIER, "Expected identifier after 'fun'.");
    Token fun_id = previous(parser);
    std::shared_ptr<FunctionNode> fun_node(new FunctionNode());
    register_function(parser, fun_node, fun_id.value);
    push_frame(parser, fun_node);
    push_scope(parser, fun_node);
    fun_node->set_parameters(fun_parameters(parser));
    fun_node->set_body(block(parser));
    pop_scope(parser);
    pop_frame(parser);
    return fun_node;
}

std::shared_ptr<AbstractSyntaxTree> return_statement(Parser& parser) {
    if (match(parser, {TOKEN_SEMICOLON})) {
        return std::shared_ptr<ReturnNode>(new ReturnNode());
    }
    std::shared_ptr<AbstractSyntaxTree> exp = expression_statement(parser);
    return std::shared_ptr<ReturnNode>(new ReturnNode({exp}));
}

std::shared_ptr<AbstractSyntaxTree> call_statement(Parser& parser, const bool& expect_semicolon) {
    Token fun_id = previous(parser, 2);
    std::shared_ptr<FunctionNode> fun_node = get_function(parser, fun_id.value);
    std::vector<std::shared_ptr<AbstractSyntaxTree>> values;
    for (int i=0; i<fun_node->get_parameters_count(); i++) {
        values.push_back(expression(parser));
        if (i != fun_node->get_parameters_count() - 1) {
            consume(parser, TOKEN_COMMA, "Expected ',' after function parameter.");
        }
    }
    consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after function parameters.");
    if (expect_semicolon) {
        consume(parser, TOKEN_SEMICOLON, "Expected ';' after function call.");
    }
    return std::shared_ptr<CallNode>(new CallNode(fun_node, values));
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
    std::shared_ptr<VariableNode> variable = get_variable_by_name(parser, id.value);
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
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, literal("1", TOKEN_LONG), AST_ADD));
            break;
        case TOKEN_MINUS_MINUS:
            exp = std::shared_ptr<BinaryOperationNode>(new BinaryOperationNode(variable, literal("1", TOKEN_LONG), AST_SUB));
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
    if (match(parser, {TOKEN_LONG})) {
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
    if (match(parser, {TOKEN_FUN})) {
        return fun_statement(parser);
    }
    if (match(parser, {TOKEN_RETURN})) {
        return return_statement(parser);
    }
    if (match_sequence(parser, {TOKEN_IDENTIFIER, TOKEN_LEFT_PAREN})) {
        return call_statement(parser);
    }
    return expression_statement(parser);
}

std::shared_ptr<BlockNode> block(Parser& parser) {
    consume(parser, TOKEN_LEFT_BRACE, "Missing '{' before block.");
    std::shared_ptr<BlockNode> block(new BlockNode());
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

void jump_to_main(Parser& parser, const std::shared_ptr<AbstractSyntaxTree>& root) {
    if (parser.functions.find(MAIN) == parser.functions.end()) {
        // find beginning of script;
        return;
    }
    root->set_main(parser.functions.at(MAIN).get());
}

std::shared_ptr<AbstractSyntaxTree> program(Parser& parser) {
    std::shared_ptr<BlockNode> root(new BlockNode());
    push_frame(parser, root);
    push_scope(parser, root);
    while (!eof(parser)) {
        root->add(statement(parser));
    }
    pop_scope(parser);
    pop_frame(parser);
    jump_to_main(parser, root);
    return root;
}
}

std::shared_ptr<AbstractSyntaxTree> parser::parse(const std::vector<Token>& tokens) {
    Parser parser;
    parser.current = 0;
    parser.tokens = tokens;
    return program(parser);
}