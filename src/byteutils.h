#if !defined(BYTE_UTILS)
#define BYTE_UTILS

#include <vector>
#include <stdint.h>

#define MASK (int64_t) 0xff
#define BYTE_0(x) (x & MASK)
#define BYTE_1(x) ((x & (MASK << 8)) >> 8)
#define BYTE_2(x) ((x & (MASK << 16)) >> 16)
#define BYTE_3(x) ((x & (MASK << 24)) >> 24)
#define BYTE_4(x) ((x & (MASK << 32)) >> 32)
#define BYTE_5(x) ((x & (MASK << 40)) >> 40)
#define BYTE_6(x) ((x & (MASK << 48)) >> 48)
#define BYTE_7(x) ((x & (MASK << 56)) >> 56)

#define SIZE_OF_BYTE 1
#define SIZE_OF_SHORT 2
#define SIZE_OF_INT 4
#define SIZE_OF_LONG 8

namespace bytes {

uint64_t add_long(const uint64_t& a, const uint64_t& b);
uint64_t sub_long(const uint64_t& a, const uint64_t& b);
uint64_t mul_long(const uint64_t& a, const uint64_t& b);
uint64_t div_long(const uint64_t& a, const uint64_t& b);
uint64_t mod_long(const uint64_t& a, const uint64_t& b);
uint64_t and_long(const uint64_t& a, const uint64_t& b);
uint64_t or_long(const uint64_t& a, const uint64_t& b);
uint64_t xor_long(const uint64_t& a, const uint64_t& b);
uint64_t not_long(const uint64_t& x);
uint8_t lt_long(const uint64_t& a, const uint64_t& b);
uint8_t lte_long(const uint64_t& a, const uint64_t& b);
uint8_t gt_long(const uint64_t& a, const uint64_t& b);
uint8_t gte_long(const uint64_t& a, const uint64_t& b);
uint8_t eq_long(const uint64_t& a, const uint64_t& b);
uint8_t not_eq_long(const uint64_t& a, const uint64_t& b);

uint8_t pop_byte(std::vector<uint8_t>& stack);
uint16_t pop_short(std::vector<uint8_t>& stack);
uint32_t pop_int(std::vector<uint8_t>& stack);
uint64_t pop_long(std::vector<uint8_t>& stack);

void push_byte(std::vector<uint8_t>& stack, const uint8_t& value);
void push_short(std::vector<uint8_t>& stack, const uint16_t& value);
void push_int(std::vector<uint8_t>& stack, const uint32_t& value);
void push_long(std::vector<uint8_t>& stack, const uint64_t& value);

uint8_t read_byte(const uint8_t* stack, uint64_t index);
uint16_t read_short(const uint8_t* stack, uint64_t index);
uint32_t read_int(const uint8_t* stack, uint64_t index);
uint64_t read_long(const uint8_t* stack, uint64_t index);

uint8_t read_byte(const std::vector<uint8_t>& stack, uint64_t index);
uint16_t read_short(const std::vector<uint8_t>& stack, uint64_t index);
uint32_t read_int(const std::vector<uint8_t>& stack, uint64_t index);
uint64_t read_long(const std::vector<uint8_t>& stack, uint64_t index);

void write_byte(uint8_t* stack, uint64_t index, uint8_t value);
void write_short(uint8_t* stack, uint64_t index, uint16_t value);
void write_int(uint8_t* stack, uint64_t index, uint32_t value);
void write_long(uint8_t* stack, uint64_t index, uint64_t value);

void write_byte(std::vector<uint8_t>& stack, uint64_t index, uint8_t value);
void write_short(std::vector<uint8_t>& stack, uint64_t index, uint16_t value);
void write_int(std::vector<uint8_t>& stack, uint64_t index, uint32_t value);
void write_long(std::vector<uint8_t>& stack, uint64_t index, uint64_t value);

}

#endif // BYTE_UTILS