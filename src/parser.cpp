#include <iostream>
#include "../lib/scanner.h"
#include "../lib/fileutils.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Syntax is: " << argv[0] << " [source.txt]" << std::endl;
        exit(1);
    }
    std::string content = fileutils::read_string(argv[1]);
    std::vector<Token> tokens = scanner::scan(content.c_str());
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