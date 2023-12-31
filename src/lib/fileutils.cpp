#include "fileutils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <filesystem>

std::vector<uint8_t> fileutils::read_bytes(const std::string& filename) {
    std::ifstream is(filename.c_str(), std::ios::binary);
    if (!is.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    is.close();
    return bytes;
}

std::vector<std::string> fileutils::read_lines(const std::string& filename) {
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

std::string fileutils::read_string(const std::string& filename) {
    std::ifstream is(filename.c_str());
    if (!is.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << is.rdbuf();
    is.close();
    return buffer.str();
}

void fileutils::write_bytes(const std::vector<uint8_t>& bytes, const std::string& filename) {
    std::ofstream os(filename.c_str(), std::ios::binary);
    std::ostream_iterator<uint8_t> output_iterator(os);
    std::copy(bytes.begin(), bytes.end(), output_iterator);
    os.close();
}

void fileutils::write_lines(const std::vector<std::string>& lines, const std::string& filename) {
    std::ofstream os(filename.c_str());
    std::ostream_iterator<std::string> output_iterator(os);
    std::copy(lines.begin(), lines.end(), output_iterator);
    os.close();
}

std::vector<std::string> fileutils::list_files(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;
    for (const auto &p : std::filesystem::recursive_directory_iterator(directory)) {
        if (p.path().extension() == extension) {
            files.push_back(p.path().string());
        }
    }
    return files;
}
