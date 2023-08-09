#include "scanner.h"
#include <iostream>
#include <string>

namespace scanner {
typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

scanner::Token create_token(const scanner::TokenType& type, const Scanner& scanner) {
    scanner::Token token;
    token.start = scanner.start;
    token.length = scanner.current - scanner.start;
    token.line = scanner.line;
    token.type = type;
    return token;
}

bool match_string(const Scanner& scanner, const std::string& str) {
    for (int i = 0; i < str.size(); i++) {
        const char* p = scanner.current + i;
        if (*p == '\0' || *p != str[i]) {
            return false;
        }
    }
    return true;
}

const char* match_quote(const Scanner& scanner) {
    const char* c = scanner.current;
    while (*c != '\0') {
        if (*c == '\\') {
            c += 2;
            continue;
        }
        if (*c == '"') {
            return c + 1;
        }
        c++;
    }
    return nullptr;
}

const char* match_number(const Scanner& scanner) {
    const char* c = scanner.current;
    while ('0' <= *c && *c <= '9') {
         c++;
    }
    return c;
}

bool is_digit(const char& c) {
    return '0' <= c && c <= '9';
}

bool is_character(const char& c) {
    return ('a' <= c && c <= 'z') or ('A' <= c && c <= 'Z');
}

const char* match_identifier(const Scanner& scanner) {
    const char* c = scanner.current;
    while (is_character(*c) || is_digit(*c)) {
        c++;
    }
    return c;
}

void error(const Scanner& scanner) {
    std::cout << "Unrecognized token on line " << scanner.line + 1 << "." << std::endl;
    exit(1);
}
}

std::vector<scanner::Token> scanner::scan(const char* code) {
    std::vector<scanner::Token> tokens;
    Scanner scanner;
    scanner.start = code;
    scanner.current = code;
    scanner.line = 0;

    while (*scanner.current != '\0') {
        switch (*scanner.current) {
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
                tokens.push_back(create_token(scanner::TOKEN_LEFT_PAREN, scanner));
                break;
            case ')':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_RIGHT_PAREN, scanner));
                break;
            case '{':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_LEFT_BRACE, scanner));
                break;
            case '}':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_RIGHT_BRACE, scanner));
                break;
            case ',':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_COMMA, scanner));
                break;
            case '.':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_DOT, scanner));
                break;
            case ';':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_SEMICOLON, scanner));
                break;
            case '+':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_PLUS, scanner));
                break;
            case '-':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_MINUS, scanner));
                break;
            case '/':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_SLASH, scanner));
                break;
            case '*':
                scanner.current++;
                tokens.push_back(create_token(scanner::TOKEN_STAR, scanner));
                break;
            case '!':
                if (match_string(scanner, "!=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_BANG_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(scanner::TOKEN_BANG, scanner));
                }
                break;
            case '=':
                if (match_string(scanner, "==")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_EQUAL_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(scanner::TOKEN_EQUAL, scanner));
                }
                break;
            case '>':
                if (match_string(scanner, ">=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_GREATER_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(scanner::TOKEN_GREATER, scanner));
                }
                break;
            case '<':
                if (match_string(scanner, "<=")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_LESS_EQUAL, scanner));
                } else {
                    scanner.current++;
                    tokens.push_back(create_token(scanner::TOKEN_LESS, scanner));
                }
                break;
            case 'f':
                if (match_string(scanner, "for")) {
                    scanner.current += 3;
                    tokens.push_back(create_token(scanner::TOKEN_FOR, scanner));
                } else if (match_string(scanner, "false")) {
                    scanner.current += 5;
                    tokens.push_back(create_token(scanner::TOKEN_FALSE, scanner));
                } else if (match_string(scanner, "fun")) {
                    scanner.current += 3;
                    tokens.push_back(create_token(scanner::TOKEN_FUN, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                scanner.start = scanner.current;
                break;
            case 'r':
                if (match_string(scanner, "return")) {
                    scanner.current += 6;
                    tokens.push_back(create_token(scanner::TOKEN_RETURN, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 't':
                if (match_string(scanner, "true")) {
                    scanner.current += 4;
                    tokens.push_back(create_token(scanner::TOKEN_TRUE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'i':
                if (match_string(scanner, "if")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_IF, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'e':
                if (match_string(scanner, "else")) {
                    scanner.current += 4;
                    tokens.push_back(create_token(scanner::TOKEN_ELSE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'w':
                if (match_string(scanner, "while")) {
                    scanner.current += 5;
                    tokens.push_back(create_token(scanner::TOKEN_WHILE, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'a':
                if (match_string(scanner, "and")) {
                    scanner.current += 3;
                    tokens.push_back(create_token(scanner::TOKEN_AND, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'o':
                if (match_string(scanner, "or")) {
                    scanner.current += 2;
                    tokens.push_back(create_token(scanner::TOKEN_OR, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case 'p':
                if (match_string(scanner, "print")) {
                    scanner.current += 5;
                    tokens.push_back(create_token(scanner::TOKEN_PRINT, scanner));
                } else {
                    scanner.current = match_identifier(scanner);
                    tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                }
                break;
            case '"': {
                const char* quote = match_quote(scanner);
                if (quote == nullptr) {
                    std::cout << "Closing string quote is missing on line " << scanner.line << std::endl;
                }
                scanner.current = quote;
                tokens.push_back(create_token(scanner::TOKEN_STRING, scanner));
                break;
            }
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
                tokens.push_back(create_token(scanner::TOKEN_NUMBER, scanner));
                break;
            case '_':
            case 'b':
            case 'c':
            case 'd':
            case 'g':
            case 'h':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'q':
            case 's':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
                scanner.current = match_identifier(scanner);
                tokens.push_back(create_token(scanner::TOKEN_IDENTIFIER, scanner));
                break;
            default:
                error(scanner);
                break;
        }
        scanner.start = scanner.current;
    }

    return tokens;
}

