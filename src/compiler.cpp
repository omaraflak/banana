#include <iostream>
#include <set>
#include "../lib/ast.h"
#include "../lib/scanner.h"
#include "../lib/parser.h"
#include "../lib/fileutils.h"

std::vector<std::unique_ptr<const Instruction>> get_instructions(const std::string& filename) {
    std::string content = fileutils::read_string(filename);
    std::vector<Token> tokens = scanner::scan(content.c_str());
    std::shared_ptr<AbstractSyntaxTree> root = parser::parse(tokens);
    return ast::to_instructions(root);
}

void compile(const std::string& filename, const std::string& output) {
    auto bytes = ast::to_bytes(get_instructions(filename));
    fileutils::write_bytes(bytes, output);
}

void compile_and_execute(const std::string& filename) {
    Vm(ast::to_bytes(get_instructions(filename))).execute();
}

void execute(const std::string& filename) {
    Vm(fileutils::read_bytes(filename)).execute();
}

void print_assembly(const std::string& filename) {
    for (auto pair : ast::to_asm(get_instructions(filename))) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

std::set<std::string> parse_flags(int argc, char** argv) {
    std::set<std::string> flags;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            flags.insert(argv[i]);
        }
    }
    return flags;
}

bool has_flag(const std::set<std::string>& flags, const std::string& flag) {
    return flags.find(flag) != flags.end();
}

std::string file_name(const std::string& filename) {
    int index = filename.find_last_of(".");
    if (index == -1) {
        return filename;
    }
    return filename.substr(0, index);
}

std::string replace_extension(const std::string& filename, const std::string& extension) {
    return file_name(filename) + "." + extension;
}

int main(int argc, char** argv) {
    std::string filename = argv[argc - 1];
    std::set<std::string> flags = parse_flags(argc, argv);

    if (has_flag(flags, "-c")) {
        compile(filename, replace_extension(filename, "obj"));
        return 0;
    }
    if (has_flag(flags, "-a")) {
        print_assembly(filename);
        return 0;
    }
    if (has_flag(flags, "-i")) {
        compile_and_execute(filename);
        return 0;
    }
    if (has_flag(flags, "-h")) {
        std::cout << "Syntax is: " << argv[0] << " [-c] [-a] [-i] [-h]" << " [source]" << std::endl;
        return 0;
    }
    if (argc == 2) {
        execute(filename);
    }
}