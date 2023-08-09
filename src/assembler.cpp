#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stdint.h>
#include "instructions.h"
#include "byteutils.h"
#include "fileutils.h"

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

std::string join_strings(const std::vector<std::string> &strings, const std::string& separator = " ") {
    std::stringstream ss;
    std::copy(strings.begin(), strings.end(), std::ostream_iterator<std::string>(ss, separator.c_str()));
    return ss.str();
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

std::map<std::string, uint64_t> create_labels_map(const std::vector<std::string>& lines) {
    std::map<std::string, uint64_t> labels;
    uint64_t position = 0;
    for (const std::string& line : lines) {
        if (line[0] == '.') {
            labels[line] = position;
            continue;
        }
        std::vector<std::string> words = split_string(line);
        Instruction* instruction = Instruction::from_opstring(words[0]);
        position += instruction->size();
        delete instruction;
    }
    return labels;
}

std::vector<std::string> resolve_labels(const std::vector<std::string>& lines) {
    std::map<std::string, uint64_t> labels = create_labels_map(lines);
    std::vector<std::string> new_lines;
    for (const std::string& line : lines) {
        if (line[0] == '.') {
            continue;
        }
        std::vector<std::string> words = split_string(line);
        for (int i = 0; i < words.size(); i++) {
            std::string word = words[i];
            if (word[0] == '.') {
                words[i] = std::to_string(labels[word]);
            }
        }
        new_lines.push_back(join_strings(words));
    }
    return new_lines;
}

std::vector<std::string> parse_program(const std::vector<std::string>& lines) {
    return resolve_labels(trim_lines(lines));
}

std::vector<uint8_t> to_binary(const std::vector<std::string>& lines) {
    std::vector<uint8_t> bytes;
    for (const std::string& line : lines) {
        Instruction* instruction = Instruction::from_string(line);
        instruction->write(bytes);
        delete instruction;
    }
    return bytes;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Syntax is: " << argv[0] << " [file.asm] [output.obj]" << std::endl;
        exit(1);
    }
    std::vector<std::string> lines = fileutils::read_lines(argv[1]);
    std::vector<uint8_t> binary = to_binary(parse_program(lines));
    fileutils::write_bytes(binary, argv[2]);
    return 0;
}