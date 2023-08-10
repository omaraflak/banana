#if !defined(PARSER)
#define PARSER

#include "ast.h"
#include "scanner.h"
#include <memory>

namespace parser {
void parse(const std::vector<Token>& tokens, AbstractSyntaxTree** root, AbstractSyntaxTree** main);
}

#endif // PARSER
