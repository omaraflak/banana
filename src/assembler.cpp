#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdint.h>
#include <iterator>
#include "instructions.h"
#include "byteutils.h"

std::string trim(const std::string& str, const std::string& character) {
    int begin = str.find_first_not_of(character);
    if (begin == std::string::npos) {
        return "";
    }
    int end = str.find_last_not_of(character);
    int range = end - begin + 1;
    return str.substr(begin, range);
}

std::vector<std::string> trim_lines(const std::vector<std::string>& lines, const std::string& comment = ";") {
    std::vector<std::string> result;
    for (std::string line : lines) {
        int index = line.find(comment);
        if (index != -1) {
            line = line.substr(0, index);
        }
        line = trim(line, " ");
        if (line != "") {
            result.push_back(line);
        }
    }
    return result;
}

// uint64_t param_value(const std::string& param, std::vector<std::pair<std::string, uint64_t>>& labels, uint64_t label_index) {
//     if (param[0] == '.') {
//         labels.push_back(std::make_pair(param, label_index));
//         return 0;
//     }
//     return stol(param);
// }

std::vector<uint8_t> to_binary(const std::vector<std::string>& lines) {
    std::vector<uint8_t> bytes;
    for (const std::string& line : lines) {
        Instruction* instruction = Instruction::from_string(line);
        instruction->write(bytes);
        delete instruction;
    }
    return bytes;
}

std::vector<std::string> read_file(const std::string& filename) {
    std::ifstream is(filename.c_str());
    if (!is.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(is, line)) {
        lines.push_back(line);
    }
    is.close();
    return lines;
}

void write_file(const std::vector<uint8_t>& data, const std::string& filename) {
    std::ofstream os(filename.c_str(), std::ios::binary);
    std::ostream_iterator<uint8_t> output_iterator(os);
    std::copy(data.begin(), data.end(), output_iterator);
    os.close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Syntax is: " << argv[0] << " [file.asm] [output.obj]" << std::endl;
        exit(1);
    }
    std::vector<uint8_t> binary = to_binary(trim_lines(read_file(argv[1])));
    write_file(binary, argv[2]);
    return 0;
}