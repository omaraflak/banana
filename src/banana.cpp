#include <iostream>
#include <vector>
#include <map>
#include "../lib/ast.h"
#include "../lib/scanner.h"
#include "../lib/parser.h"
#include "../lib/fileutils.h"

std::vector<std::unique_ptr<const Instruction>> get_instructions(
    const std::string& filename,
    const std::vector<std::string>& shared_libraries
) {
    std::string content = fileutils::read_string(filename);
    std::vector<Token> tokens = scanner::scan(content.c_str());
    std::shared_ptr<AbstractSyntaxTree> root = parser::parse(tokens, shared_libraries);
    return ast::to_instructions(root);
}

void compile(const std::string& filename, const std::string& output, const std::vector<std::string>& shared_libraries) {
    auto bytes = ast::to_bytes(get_instructions(filename, shared_libraries));
    fileutils::write_bytes(bytes, output);
}

void compile_and_execute(const std::string& filename, const std::vector<std::string>& shared_libraries) {
    Vm(ast::to_bytes(get_instructions(filename, shared_libraries)), shared_libraries).execute();
}

void execute(const std::string& filename, const std::vector<std::string>& shared_libraries) {
    Vm(fileutils::read_bytes(filename), shared_libraries).execute();
}

void print_assembly(const std::string& filename, const std::vector<std::string>& shared_libraries) {
    for (auto pair : ast::to_asm(get_instructions(filename, shared_libraries))) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

std::map<std::string, std::string> parse_flags(int argc, char** argv) {
    std::map<std::string, std::string> flags;
    int i = 1;
    while (i < argc) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                flags[argv[i]] = argv[i + 1];
                i += 2;
                continue;
            } else {
                flags[argv[i]] = "1";
            }
        } else {
            flags["--file"] = argv[i];
        }
        i++;
    }
    return flags;
}

bool has_flag(const std::map<std::string, std::string>& flags, const std::string& flag) {
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

void help(const std::string& exe) {
    std::cout << "Syntax is: " << exe << " [-c] [-a] [-i]" << " [bytecode|source]" << std::endl;
    std::cout << "  -c\t Compiles banana code to vm bytecode." << std::endl;
    std::cout << "  -a\t Translates banana code to vm instructions." << std::endl;
    std::cout << "  -i\t Execute banana code from source file." << std::endl;
}

int main(int argc, char** argv) {
    std::map<std::string, std::string> flags = parse_flags(argc, argv);
    std::string filename = flags["--file"];

    std::vector<std::string> shared_libraries;
    if (has_flag(flags, "--lib")) {
        shared_libraries = fileutils::list_files(flags["--lib"], ".so");
    }

    if (has_flag(flags, "-c")) {
        compile(filename, replace_extension(filename, "obj"), shared_libraries);
        return 0;
    }
    if (has_flag(flags, "-a")) {
        print_assembly(filename, shared_libraries);
        return 0;
    }
    if (has_flag(flags, "-i")) {
        compile_and_execute(filename, shared_libraries);
        return 0;
    }
    if (has_flag(flags, "-h")) {
        help(argv[0]);
    }
    execute(filename, shared_libraries);
    return 0;
}