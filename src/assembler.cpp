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

std::string trim(const std::string& str, const std::string& whitespace = " ") {
    int begin = str.find_first_not_of(whitespace);
    if (begin == std::string::npos) {
        return "";
    }
    int end = str.find_last_not_of(whitespace);
    int range = end - begin + 1;
    return str.substr(begin, range);
}

std::vector<std::string> parse_file(const std::vector<std::string>& lines, const std::string& comment = ";") {
    std::vector<std::string> result;
    for (std::string line : lines) {
        int index = line.find(comment);
        if (index != -1) {
            line = line.substr(0, index);
        }
        line = trim(line);
        if (line != "") {
            result.push_back(line);
        }
    }
    return result;
}

uint64_t param_value(const std::string& param, std::vector<std::pair<std::string, uint64_t>>& labels, uint64_t label_index) {
    if (param[0] == '.') {
        labels.push_back(std::make_pair(param, label_index));
        return 0;
    }
    return stol(param);
}

std::vector<std::string> split_string(const std::string& str, const char& separator = ' ') {
    std::stringstream ss(str);
    std::vector<std::string> result;
    std::string s;
    while (getline(ss, s, separator)) {
        result.push_back(s);
    }
    return result;
}

std::vector<uint8_t> to_binary(const std::vector<std::string>& lines) {
    std::vector<std::string> parsed = parse_file(lines);
    std::vector<uint8_t> result;
    std::map<std::string, uint64_t> labels;
    std::vector<std::pair<std::string, uint64_t>> missed_labels_indices;

    for (int i = 0; i < parsed.size(); i++) {
        std::vector<std::string> line = split_string(parsed[i]);
        std::string op = line[0];

        if (op[0] == '.') {
            labels[op] = result.size();
        }
        else if (op == "add") { result.push_back(OP_ADD); }
        else if (op == "sub") { result.push_back(OP_SUB); }
        else if (op == "mul") { result.push_back(OP_MUL); }
        else if (op == "div") { result.push_back(OP_DIV); }
        else if (op == "mod") { result.push_back(OP_MOD); }
        else if (op == "and") { result.push_back(OP_AND); }
        else if (op == "or") { result.push_back(OP_OR); }
        else if (op == "xor") { result.push_back(OP_XOR); }
        else if (op == "not") { result.push_back(OP_NOT); }
        else if (op == "push") {
            result.push_back(OP_PUSH);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "jump") {
            result.push_back(OP_JUMP);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "jump_if") {
            result.push_back(OP_JUMP_IF);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "jump_if_false") {
            result.push_back(OP_JUMP_IF_FALSE);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "call") {
            result.push_back(OP_CALL);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "ret") { result.push_back(OP_RET); }
        else if (op == "dup") { result.push_back(OP_DUP); }
        else if (op == "swap") { result.push_back(OP_SWAP); }
        else if (op == "lt") { result.push_back(OP_LT); }
        else if (op == "lte") { result.push_back(OP_LTE); }
        else if (op == "gt") { result.push_back(OP_GT); }
        else if (op == "gte") { result.push_back(OP_GTE); }
        else if (op == "eq") { result.push_back(OP_EQ); }
        else if (op == "noteq") { result.push_back(OP_NOT_EQ); }
        else if (op == "print") { result.push_back(OP_PRINT); }
        else if (op == "printc") { result.push_back(OP_PRINT_C); }
        else if (op == "prints") {
            std::string rest = parsed[i].substr(parsed[i].find(" ") + 1);
            for (const char& c : rest) {
                result.push_back(OP_PUSH);
                bytes::push_long(result, c);
                result.push_back(OP_PRINT_C);
            }
        }
        else if (op == "store") { 
            result.push_back(OP_STORE);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "load") { 
            result.push_back(OP_LOAD);
            bytes::push_long(result, param_value(line[1], missed_labels_indices, result.size()));
        }
        else if (op == "halt") {
            result.push_back(OP_HALT);
        }
        else {
            std::cout << "Operation not recognized: " << op << std::endl;
            exit(1);
        }
    }

    for (auto entry : missed_labels_indices) {
        std::string label = entry.first;
        uint64_t index = entry.second;
        uint64_t value = labels[label];
        result[index] = BYTE_0(value);
        result[index + 1] = BYTE_1(value);
        result[index + 2] = BYTE_2(value);
        result[index + 3] = BYTE_3(value);
        result[index + 4] = BYTE_4(value);
        result[index + 5] = BYTE_5(value);
        result[index + 6] = BYTE_6(value);
        result[index + 7] = BYTE_7(value);
    }

    return result;
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
    std::vector<std::string> program = read_file(argv[1]);
    std::vector<uint8_t> binary = to_binary(program);
    write_file(binary, argv[2]);
    return 0;
}