#include "scanner.h"
#include <iostream>

namespace scanner {
struct Scanner {
    std::string code;
    size_t start;
    size_t current;
    size_t line;
};

Token create_token(const TokenType& type, const Scanner& scanner) {
    Token token;
    token.type = type;
    token.line = scanner.line;
    token.value = scanner.code.substr(scanner.start, scanner.current - scanner.start);
    return token;
}

bool is_digit(const char& c) {
    return '0' <= c && c <= '9';
}

bool is_character(const char& c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool match_string(const Scanner& scanner, const std::string& str, const bool& keyword = false) {
    for (size_t i = 0; i < str.size(); i++) {
        const char p = scanner.code[scanner.current + i];
        if (p == '\0' || p != str[i]) {
            return false;
        }
    }
    if (!keyword) {
        return true;
    }
    return !is_character(scanner.code[scanner.current + str.size()]);
}

int match_quote(const Scanner& scanner) {
    size_t p = scanner.current + 1;
    while (p < scanner.code.size() && scanner.code[p] != '\n') {
        if (scanner.code[p] == '\\') {
            p += 2;
            continue;
        }
        if (scanner.code[p] == '"') {
            return p + 1;
        }
        p++;
    }
    return -1;
}

int match_number(const Scanner& scanner) {
    int p = scanner.current;
    while (is_digit(scanner.code[p])) {
         p++;
    }
    return p;
}

int match_identifier(const Scanner& scanner) {
    int p = scanner.current;
    while (is_character(scanner.code[p]) || is_digit(scanner.code[p])) {
        p++;
    }
    return p;
}

int next_line(const Scanner& scanner) {
    size_t p = scanner.current;
    while (p < scanner.code.size() && scanner.code[p] != '\n') {
        p++;
    }
    return p < scanner.code.size() ? p + 1 : p;
}

void error(const Scanner& scanner) {
    std::cout << "Unrecognized token on line " << scanner.line << "." << std::endl;
    int i = 0;
    while (scanner.current + i < scanner.code.size() && scanner.code[scanner.current + i] != '\n') {
        std::cout << scanner.code[scanner.current + i];
        i++;
    }
    std::cout << std::endl;
    exit(1);
}
}

std::vector<Token> scanner::scan(const std::string& code) {
    std::vector<Token> tokens;
    Scanner scanner;
    scanner.code = code;
    scanner.start = 0;
    scanner.current = 0;
    scanner.line = 1;

    while (scanner.code[scanner.current] != '\0') {
        switch (scanner.code[scanner.current]) {
            case ' ':
            case '\r':
            case '\t':
                scanner.start++;
                scanner.current++;
                break;
            case '\n':
                scanner.start++;
                scanner.current++;
                scanner.line++;
                break;
            case '(':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_LEFT_PAREN, scanner));
                break;
            case ')':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_RIGHT_PAREN, scanner));
                break;
            case '{':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_LEFT_BRACE, scanner));
                break;
            case '}':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_RIGHT_BRACE, scanner));
                break;
            case ',':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_COMMA, scanner));
                break;
            case '.':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_DOT, scanner));
                break;
            case ';':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_SEMICOLON, scanner));
                break;
            case '~':
                scanner.current++;
                tokens.push_back(create_token(TOKEN_TILDE, scanner));
                break;
            case '&':
                if (match_string(scanner, "&=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_AMPERSAND_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_AMPERSAND, scanner));
                }
                break;
            case '|':
                if (match_string(scanner, "|=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_PIPE_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_PIPE, scanner));
                }
                break;
            case '+':
                if (match_string(scanner, "++")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_PLUS_PLUS, scanner));
                } else if (match_string(scanner, "+=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_PLUS_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_PLUS, scanner));
                }
                break;
            case '-':
                if (match_string(scanner, "--")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_MINUS_MINUS, scanner));
                } else if (match_string(scanner, "-=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_MINUS_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_MINUS, scanner));
                }
                break;
            case '/':
                if (match_string(scanner, "//")) {
                    scanner.current = next_line(scanner);
                    scanner.line++;
                } else if (match_string(scanner, "/=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_SLASH_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_SLASH, scanner));
                }
                break;
            case '*':
                if (match_string(scanner, "*=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_STAR_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_STAR, scanner));
                }
                break;
            case '%':
                if (match_string(scanner, "%=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_MOD_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_MOD, scanner));
                }
                break;
            case '^':
                if (match_string(scanner, "^=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_XOR_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_XOR, scanner));
                }
                break;
            case '!':
                if (match_string(scanner, "!=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_BANG_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_BANG, scanner));
                }
                break;
            case '=':
                if (match_string(scanner, "==")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_EQUAL_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_EQUAL, scanner));
                }
                break;
            case '>':
                if (match_string(scanner, ">=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_GREATER_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_GREATER, scanner));
                }
                break;
            case '<':
                if (match_string(scanner, "<=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_LESS_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(TOKEN_LESS, scanner));
                }
                break;
            case 'f':
                if (match_string(scanner, "for", /* keyword */ true)) {
                    scanner.current += 3;
                    tokens.push_back(create_token(TOKEN_FOR, scanner));
                } else if (match_string(scanner, "false", /* keyword */ true)) {
                    scanner.current += 5;
                    tokens.push_back(create_token(TOKEN_FALSE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                scanner.start = scanner.current;
                break;
            case 'r':
                if (match_string(scanner, "return", /* keyword */ true)) {
                    scanner.current += 6;
                    tokens.push_back(create_token(TOKEN_RETURN, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'i':
                if (match_string(scanner, "if", /* keyword */ true)) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_IF, scanner));
                } else if (match_string(scanner, "int", /* keyword */ true)) {
                    scanner.current += 3;
                    tokens.push_back(create_token(TOKEN_INT, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'e':
                if (match_string(scanner, "else", /* keyword */ true)) {
                    scanner.current += 4;
                    tokens.push_back(create_token(TOKEN_ELSE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'w':
                if (match_string(scanner, "while", /* keyword */ true)) {
                    scanner.current += 5;
                    tokens.push_back(create_token(TOKEN_WHILE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'a':
                if (match_string(scanner, "and", /* keyword */ true)) {
                    scanner.current += 3;
                    tokens.push_back(create_token(TOKEN_AND, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'o':
                if (match_string(scanner, "or", /* keyword */ true)) {
                    scanner.current += 2;
                    tokens.push_back(create_token(TOKEN_OR, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'p':
                if (match_string(scanner, "print", /* keyword */ true)) {
                    scanner.current += 5;
                    tokens.push_back(create_token(TOKEN_PRINT, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'c':
                if (match_string(scanner, "char", /* keyword */ true)) {
                    scanner.current += 4;
                    tokens.push_back(create_token(TOKEN_CHAR, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'l':
                if (match_string(scanner, "long", /* keyword */ true)) {
                    scanner.current += 4;
                    tokens.push_back(create_token(TOKEN_LONG, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'b':
                if (match_string(scanner, "bool", /* keyword */ true)) {
                    scanner.current += 4;
                    tokens.push_back(create_token(TOKEN_BOOL, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 't':
                if (match_string(scanner, "true", /* keyword */ true)) {
                    scanner.current += 4;
                    tokens.push_back(create_token(TOKEN_TRUE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                }
                break;
            case '"': {
                int quote = match_quote(scanner);
                if (quote == -1) {
                    std::cout << "Closing string quote is missing on line " << scanner.line << std::endl;
                }
                scanner.start++;
                scanner.current = quote - 1;
                tokens.push_back(create_token(TOKEN_STRING, scanner));
                scanner.current = quote;
                break;
            }
            case '@':
                if (match_string(scanner, "@native", /* keyword */ true)) {
                    scanner.current += 7;
                    tokens.push_back(create_token(TOKEN_AT_NATIVE, scanner));
                } else {
                    std::cout << "Line: " << scanner.line << ". Identifier cannot begin with '@'." << std::endl;
                    exit(1);
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                scanner.current = match_number(scanner);
                tokens.push_back(create_token(TOKEN_NUMBER, scanner));
                break;
            case '_':
            case 'd':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'm':
            case 'n':
            case 's':
            case 'q':
            case 'u':
            case 'x':
            case 'y':
            case 'z':
                scanner.current = match_identifier(scanner);
                tokens.push_back(create_token(TOKEN_IDENTIFIER, scanner));
                break;
            default:
                error(scanner);
                break;
        }
        scanner.start = scanner.current;
    }

    return tokens;
}

