#if !defined(SCANNER)
#define SCANNER

#include <vector>

enum TokenType {
        // Single-character tokens.
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,
        // One or two character tokens.
        TOKEN_BANG, TOKEN_BANG_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL,
        TOKEN_MINUS, TOKEN_MINUS_MINUS, TOKEN_MINUS_EQUAL,
        TOKEN_PLUS, TOKEN_PLUS_PLUS, TOKEN_PLUS_EQUAL,
        TOKEN_STAR, TOKEN_STAR_EQUAL,
        TOKEN_SLASH, TOKEN_SLASH_EQUAL,
        TOKEN_MOD, TOKEN_MOD_EQUAL,
        TOKEN_XOR, TOKEN_XOR_EQUAL,
        TOKEN_AMPERSAND, TOKEN_AMPERSAND_EQUAL,
        TOKEN_PIPE, TOKEN_PIPE_EQUAL,
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
