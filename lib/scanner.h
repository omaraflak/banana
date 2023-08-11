#if !defined(SCANNER)
#define SCANNER

#include <vector>

enum TokenType {
        // Single-character tokens.
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
        TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
        // One or two character tokens.
        TOKEN_BANG, TOKEN_BANG_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_PLUS_PLUS,
        TOKEN_MINUS_MINUS, TOKEN_PLUS_EQUAL, TOKEN_MINUS_EQUAL,
        TOKEN_STAR_EQUAL, TOKEN_SLASH_EQUAL,
        // Literals.
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
        // Keywords.
        TOKEN_FUN, TOKEN_RETURN, TOKEN_IF, TOKEN_ELSE,
        TOKEN_FOR, TOKEN_WHILE, TOKEN_AND, TOKEN_OR,
        TOKEN_PRINT, TOKEN_VAR
    };

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

namespace scanner {
std::vector<Token> scan(const char* code);
}

#endif // SCANNER
