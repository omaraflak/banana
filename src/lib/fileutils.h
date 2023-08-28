#if !defined(FILE_UTILS)
#define FILE_UTILS

#include <vector>
#include <string>
#include <stdint.h>

namespace fileutils {
std::vector<uint8_t> read_bytes(const std::string& filename);
std::vector<std::string> read_lines(const std::string& filename);
std::string read_string(const std::string& filename);
void write_bytes(const std::vector<uint8_t>& bytes, const std::string& filename);
void write_lines(const std::vector<std::string>& lines, const std::string& filename);
std::vector<std::string> list_files(const std::string& directory, const std::string& extension);
}

#endif // FILE_UTILS
