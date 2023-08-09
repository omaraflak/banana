#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "vm.h"
#include "fileutils.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Syntax is: " << argv[0] << " [file.obj]" << std::endl;
        exit(1);
    }
    std::vector<uint8_t> program = fileutils::read_bytes(argv[1]);
    Vm vm(program);
    // vm.debug = true;
    vm.execute();
    return 0;
}