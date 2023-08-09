#if !defined(FILEUTILS)
#define FILEUTILS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <iterator>

namespace fileutils {
std::vector<uint8_t> read_bytes(const std::string& filename);
std::vector<std::string> read_lines(const std::string& filename);
std::string read_string(const std::string& filename);
void write_bytes(const std::vector<uint8_t>& bytes, const std::string& filename);
void write_lines(const std::vector<std::string>& lines, const std::string& filename);
}

#endif // FILEUTILS
