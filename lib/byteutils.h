#if !defined(BYTE_UTILS)
#define BYTE_UTILS

#include <vector>
#include <stdint.h>

#define BYTE_0(x) (x & 0xff)
#define BYTE_1(x) ((x & 0xff00) >> 8)
#define BYTE_2(x) ((x & 0xff0000) >> 16)
#define BYTE_3(x) ((x & 0xff000000) >> 24)
#define BYTE_4(x) ((x & 0xff00000000) >> 32)
#define BYTE_5(x) ((x & 0xff0000000000) >> 40)
#define BYTE_6(x) ((x & 0xff000000000000) >> 48)
#define BYTE_7(x) ((x & 0xff00000000000000) >> 56)

#define SIZE_OF_BYTE 1
#define SIZE_OF_SHORT 2
#define SIZE_OF_INT 4
#define SIZE_OF_LONG 8

namespace bytes {
uint8_t pop_byte(std::vector<uint8_t>& stack);
uint16_t pop_short(std::vector<uint8_t>& stack);
uint32_t pop_int(std::vector<uint8_t>& stack);
uint64_t pop_long(std::vector<uint8_t>& stack);

void push_byte(std::vector<uint8_t>& stack, const uint8_t& value);
void push_short(std::vector<uint8_t>& stack, const uint16_t& value);
void push_int(std::vector<uint8_t>& stack, const uint32_t& value);
void push_long(std::vector<uint8_t>& stack, const uint64_t& value);

uint8_t read_byte(const uint8_t* stack, const uint64_t& index);
uint16_t read_short(const uint8_t* stack, const uint64_t& index);
uint32_t read_int(const uint8_t* stack, const uint64_t& index);
uint64_t read_long(const uint8_t* stack, const uint64_t& index);

uint8_t read_byte(const std::vector<uint8_t>& stack, const uint64_t& index);
uint16_t read_short(const std::vector<uint8_t>& stack, const uint64_t& index);
uint32_t read_int(const std::vector<uint8_t>& stack, const uint64_t& index);
uint64_t read_long(const std::vector<uint8_t>& stack, const uint64_t& index);

void write_byte(uint8_t* stack, const uint64_t& index, const uint8_t& value);
void write_short(uint8_t* stack, const uint64_t& index, const uint16_t& value);
void write_int(uint8_t* stack, const uint64_t& index, const uint32_t& value);
void write_long(uint8_t* stack, const uint64_t& index, const uint64_t& value);

void write_byte(std::vector<uint8_t>& stack, const uint64_t& index, const uint8_t& value);
void write_short(std::vector<uint8_t>& stack, const uint64_t& index, const uint16_t& value);
void write_int(std::vector<uint8_t>& stack, const uint64_t& index, const uint32_t& value);
void write_long(std::vector<uint8_t>& stack, const uint64_t& index, const uint64_t& value);
}

#endif // BYTE_UTILS