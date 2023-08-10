#if !defined(PARSER)
#define PARSER

#include "ast.h"
#include "scanner.h"
#include <memory>

namespace parser {
std::unique_ptr<AbstractSyntaxTree> parse(const std::vector<Token>& tokens);
}

#endif // PARSER
