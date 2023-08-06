#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "vm.h"

std::vector<uint8_t> read_file(const std::string& filename) {
    std::ifstream is(filename.c_str(), std::ios::binary);
    if (!is.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::vector<uint8_t> contents((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    is.close();
    return contents;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Syntax is: " << argv[0] << " [file.obj]" << std::endl;
        exit(1);
    }
    std::vector<uint8_t> program = read_file(argv[1]);
    Vm vm(program);
    // vm.debug = true;
    vm.execute();
    return 0;
}