#if !defined(PARSER)
#define PARSER

#include <memory>
#include <vector>
#include "ast.h"
#include "scanner.h"

namespace parser {
std::shared_ptr<AbstractSyntaxTree> parse(
    const std::vector<Token>& tokens,
    const std::vector<std::string>& shared_libraries = std::vector<std::string>()
);
}

#endif // PARSER
