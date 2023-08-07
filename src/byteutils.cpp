#include "byteutils.h"

namespace bytes {

uint64_t add_long(const uint64_t& a, const uint64_t& b) { return a + b; }
uint64_t sub_long(const uint64_t& a, const uint64_t& b) { return a - b; }
uint64_t mul_long(const uint64_t& a, const uint64_t& b) { return a * b; }
uint64_t div_long(const uint64_t& a, const uint64_t& b) { return a / b; }
uint64_t mod_long(const uint64_t& a, const uint64_t& b) { return a % b; }
uint64_t and_long(const uint64_t& a, const uint64_t& b) { return a & b; }
uint64_t or_long(const uint64_t& a, const uint64_t& b) { return a | b; }
uint64_t xor_long(const uint64_t& a, const uint64_t& b) { return a ^ b; }
uint64_t not_long(const uint64_t& x) { return ~x; }
uint8_t lt_long(const uint64_t& a, const uint64_t& b) { return a < b; }
uint8_t lte_long(const uint64_t& a, const uint64_t& b) { return a <= b; }
uint8_t gt_long(const uint64_t& a, const uint64_t& b) { return a > b; }
uint8_t gte_long(const uint64_t& a, const uint64_t& b) { return a >= b; }
uint8_t eq_long(const uint64_t& a, const uint64_t& b) { return a == b; }
uint8_t not_eq_long(const uint64_t& a, const uint64_t& b) { return a != b; }

void push_byte(std::vector<uint8_t>& stack, const uint8_t& value) {
    stack.push_back(value);
}

void push_short(std::vector<uint8_t>& stack, const uint16_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
}

void push_int(std::vector<uint8_t>& stack, const uint32_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
}

void push_long(std::vector<uint8_t>& stack, const uint64_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
    stack.push_back(BYTE_4(value));
    stack.push_back(BYTE_5(value));
    stack.push_back(BYTE_6(value));
    stack.push_back(BYTE_7(value));
}

uint8_t pop_byte(std::vector<uint8_t>& stack) {
    uint8_t b = stack.back();
    stack.pop_back();
    return b;
}

uint16_t pop_short(std::vector<uint8_t>& stack) {
    return ((uint16_t) pop_byte(stack) << 8) | pop_byte(stack);
}

uint32_t pop_int(std::vector<uint8_t>& stack) {
    return ((uint32_t) pop_short(stack) << 16) | pop_short(stack);
}

uint64_t pop_long(std::vector<uint8_t>& stack) {
    return ((uint64_t) pop_int(stack) << 32) | pop_int(stack);
}

uint8_t read_byte(const uint8_t* stack, uint64_t index) {
    return stack[index];
}

uint16_t read_short(const uint8_t* stack, uint64_t index) {
    return ((uint16_t) read_byte(stack, index + 1) << 8) | read_byte(stack, index);
}

uint32_t read_int(const uint8_t* stack, uint64_t index) {
    return ((uint32_t) read_short(stack, index + 2) << 16) | read_short(stack, index);
}

uint64_t read_long(const uint8_t* stack, uint64_t index) {
    return ((uint64_t) read_int(stack, index + 4) << 32) | read_int(stack, index);
}

uint8_t read_byte(const std::vector<uint8_t>& stack, uint64_t index) {
    return bytes::read_byte(stack.data(), index);
}

uint16_t read_short(const std::vector<uint8_t>& stack, uint64_t index) {
    return bytes::read_short(stack.data(), index);
}

uint32_t read_int(const std::vector<uint8_t>& stack, uint64_t index) {
    return bytes::read_int(stack.data(), index);
}

uint64_t read_long(const std::vector<uint8_t>& stack, uint64_t index) {
    return bytes::read_long(stack.data(), index);
}

void write_byte(uint8_t* stack, uint64_t index, uint8_t value) {
    stack[index] = value;
}

void write_short(uint8_t* stack, uint64_t index, uint16_t value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);    
}

void write_int(uint8_t* stack, uint64_t index, uint32_t value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
}

void write_long(uint8_t* stack, uint64_t index, uint64_t value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
    stack[index + 4] = BYTE_4(value);
    stack[index + 5] = BYTE_5(value);
    stack[index + 6] = BYTE_6(value);
    stack[index + 7] = BYTE_7(value);
}

void write_byte(std::vector<uint8_t>& stack, uint64_t index, uint8_t value) {
    bytes::write_byte(stack.data(), index, value);
}

void write_short(std::vector<uint8_t>& stack, uint64_t index, uint16_t value) {
    bytes::write_short(stack.data(), index, value);
}

void write_int(std::vector<uint8_t>& stack, uint64_t index, uint32_t value) {
    bytes::write_int(stack.data(), index, value);
}

void write_long(std::vector<uint8_t>& stack, uint64_t index, uint64_t value) {
    bytes::write_long(stack.data(), index, value);
}

}