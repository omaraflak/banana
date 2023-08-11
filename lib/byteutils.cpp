#include "byteutils.h"

uint64_t bytes::add_long(const uint64_t& a, const uint64_t& b) { return a + b; }
uint64_t bytes::sub_long(const uint64_t& a, const uint64_t& b) { return a - b; }
uint64_t bytes::mul_long(const uint64_t& a, const uint64_t& b) { return a * b; }
uint64_t bytes::div_long(const uint64_t& a, const uint64_t& b) { return a / b; }
uint64_t bytes::mod_long(const uint64_t& a, const uint64_t& b) { return a % b; }
uint64_t bytes::and_long(const uint64_t& a, const uint64_t& b) { return a & b; }
uint64_t bytes::or_long(const uint64_t& a, const uint64_t& b) { return a | b; }
uint64_t bytes::xor_long(const uint64_t& a, const uint64_t& b) { return a ^ b; }
uint64_t bytes::not_long(const uint64_t& x) { return ~x; }

uint8_t bytes::lt_long(const uint64_t& a, const uint64_t& b) { return a < b; }
uint8_t bytes::lte_long(const uint64_t& a, const uint64_t& b) { return a <= b; }
uint8_t bytes::gt_long(const uint64_t& a, const uint64_t& b) { return a > b; }
uint8_t bytes::gte_long(const uint64_t& a, const uint64_t& b) { return a >= b; }
uint8_t bytes::eq_long(const uint64_t& a, const uint64_t& b) { return a == b; }
uint8_t bytes::not_eq_long(const uint64_t& a, const uint64_t& b) { return a != b; }
uint8_t bytes::bool_and(const bool& a, const bool& b) { return a && b; }
uint8_t bytes::bool_or(const bool& a, const bool& b) { return a || b; }
uint8_t bytes::bool_not(const bool& x) { return !x; }

void bytes::push_byte(std::vector<uint8_t>& stack, const uint8_t& value) {
    stack.push_back(value);
}

void bytes::push_short(std::vector<uint8_t>& stack, const uint16_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
}

void bytes::push_int(std::vector<uint8_t>& stack, const uint32_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
}

void bytes::push_long(std::vector<uint8_t>& stack, const uint64_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
    stack.push_back(BYTE_4(value));
    stack.push_back(BYTE_5(value));
    stack.push_back(BYTE_6(value));
    stack.push_back(BYTE_7(value));
}

uint8_t bytes::pop_byte(std::vector<uint8_t>& stack) {
    uint8_t b = stack.back();
    stack.pop_back();
    return b;
}

uint16_t bytes::pop_short(std::vector<uint8_t>& stack) {
    return ((uint16_t) pop_byte(stack) << 8) | pop_byte(stack);
}

uint32_t bytes::pop_int(std::vector<uint8_t>& stack) {
    return ((uint32_t) pop_short(stack) << 16) | pop_short(stack);
}

uint64_t bytes::pop_long(std::vector<uint8_t>& stack) {
    return ((uint64_t) pop_int(stack) << 32) | pop_int(stack);
}

uint8_t bytes::read_byte(const uint8_t* stack, const uint64_t& index) {
    return stack[index];
}

uint16_t bytes::read_short(const uint8_t* stack, const uint64_t& index) {
    return ((uint16_t) read_byte(stack, index + 1) << 8) | read_byte(stack, index);
}

uint32_t bytes::read_int(const uint8_t* stack, const uint64_t& index) {
    return ((uint32_t) read_short(stack, index + 2) << 16) | read_short(stack, index);
}

uint64_t bytes::read_long(const uint8_t* stack, const uint64_t& index) {
    return ((uint64_t) read_int(stack, index + 4) << 32) | read_int(stack, index);
}

uint8_t bytes::read_byte(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return bytes::read_byte(stack.data(), index);
}

uint16_t bytes::read_short(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return bytes::read_short(stack.data(), index);
}

uint32_t bytes::read_int(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return bytes::read_int(stack.data(), index);
}

uint64_t bytes::read_long(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return bytes::read_long(stack.data(), index);
}

void bytes::write_byte(uint8_t* stack, const uint64_t& index, const uint8_t& value) {
    stack[index] = value;
}

void bytes::write_short(uint8_t* stack, const uint64_t& index, const uint16_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);    
}

void bytes::write_int(uint8_t* stack, const uint64_t& index, const uint32_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
}

void bytes::write_long(uint8_t* stack, const uint64_t& index, const uint64_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
    stack[index + 4] = BYTE_4(value);
    stack[index + 5] = BYTE_5(value);
    stack[index + 6] = BYTE_6(value);
    stack[index + 7] = BYTE_7(value);
}

void bytes::write_byte(std::vector<uint8_t>& stack, const uint64_t& index, const uint8_t& value) {
    bytes::write_byte(stack.data(), index, value);
}

void bytes::write_short(std::vector<uint8_t>& stack, const uint64_t& index, const uint16_t& value) {
    bytes::write_short(stack.data(), index, value);
}

void bytes::write_int(std::vector<uint8_t>& stack, const uint64_t& index, const uint32_t& value) {
    bytes::write_int(stack.data(), index, value);
}

void bytes::write_long(std::vector<uint8_t>& stack, const uint64_t& index, const uint64_t& value) {
    bytes::write_long(stack.data(), index, value);
}