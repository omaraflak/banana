#if !defined(PARSER)
#define PARSER

#include <memory>
#include "ast.h"
#include "scanner.h"

namespace parser {
std::shared_ptr<AbstractSyntaxTree> parse(const std::vector<Token>& tokens);
}

#endif // PARSER
