#include <iostream>
#include "../lib/scanner.h"
#include "../lib/parser.h"
#include "../lib/ast.h"
#include "../lib/fileutils.h"

void print(const std::vector<Token>& tokens) {
    for (auto token : tokens) {
        std::cout << "token: ";
        for (int i = 0; i < token.length; i++) {
            std::cout << *(token.start + i);
        }
        std::cout << std::endl;
        std::cout << "type: " << token.type << std::endl;
        std::cout << "line: " << token.line << std::endl;
        std::cout << "-----" << std::endl;
    }
}

int main(int argc, char** argv) {    
    if (argc != 2) {
        std::cout << "Syntax is: " << argv[0] << " [source.txt]" << std::endl;
        exit(1);
    }
    std::string content = fileutils::read_string(argv[1]);
    std::vector<Token> tokens = scanner::scan(content.c_str());
    std::shared_ptr<AbstractSyntaxTree> root = parser::parse(tokens);
    std::vector<std::unique_ptr<const Instruction>> instructions = ast::to_instructions(root);

    if (getenv("DEBUG") != nullptr) {
        for (auto pair : ast::to_asm(instructions)) {
            std::cout << pair.first << "\t" << pair.second << std::endl;
        }
        std::cout << "-----" << std::endl;
    }

    Vm vm(ast::to_bytes(instructions));
    vm.execute();
}