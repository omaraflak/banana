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

namespace byteutils {
void push_short(std::vector<uint8_t>& stack, const int16_t& value);
void push_int(std::vector<uint8_t>& stack, const int32_t& value);
void push_long(std::vector<uint8_t>& stack, const int64_t& value);
void push_ulong(std::vector<uint8_t>& stack, const uint64_t& value);

int16_t read_short(const std::vector<uint8_t>& stack, const uint64_t& index);
int32_t read_int(const std::vector<uint8_t>& stack, const uint64_t& index);
int64_t read_long(const std::vector<uint8_t>& stack, const uint64_t& index);
uint64_t read_ulong(const std::vector<uint8_t>& stack, const uint64_t& index);
}

#endif // BYTE_UTILS