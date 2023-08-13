#include "byteutils.h"

void byteutils::push_short(std::vector<uint8_t>& stack, const uint16_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
}

void byteutils::push_int(std::vector<uint8_t>& stack, const uint32_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
}

void byteutils::push_long(std::vector<uint8_t>& stack, const uint64_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
    stack.push_back(BYTE_4(value));
    stack.push_back(BYTE_5(value));
    stack.push_back(BYTE_6(value));
    stack.push_back(BYTE_7(value));
}

uint8_t byteutils::pop_byte(std::vector<uint8_t>& stack) {
    uint8_t b = stack.back();
    stack.pop_back();
    return b;
}

uint16_t byteutils::pop_short(std::vector<uint8_t>& stack) {
    return ((uint16_t) pop_byte(stack) << 8) | pop_byte(stack);
}

uint32_t byteutils::pop_int(std::vector<uint8_t>& stack) {
    return ((uint32_t) pop_short(stack) << 16) | pop_short(stack);
}

uint64_t byteutils::pop_long(std::vector<uint8_t>& stack) {
    return ((uint64_t) pop_int(stack) << 32) | pop_int(stack);
}

uint16_t byteutils::read_short(const uint8_t* stack, const uint64_t& index) {
    return ((uint16_t) stack[index + 1] << 8) | stack[index];
}

uint32_t byteutils::read_int(const uint8_t* stack, const uint64_t& index) {
    return ((uint32_t) read_short(stack, index + 2) << 16) | read_short(stack, index);
}

uint64_t byteutils::read_long(const uint8_t* stack, const uint64_t& index) {
    return ((uint64_t) read_int(stack, index + 4) << 32) | read_int(stack, index);
}

uint16_t byteutils::read_short(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return byteutils::read_short(stack.data(), index);
}

uint32_t byteutils::read_int(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return byteutils::read_int(stack.data(), index);
}

uint64_t byteutils::read_long(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return byteutils::read_long(stack.data(), index);
}

void byteutils::write_short(uint8_t* stack, const uint64_t& index, const uint16_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);    
}

void byteutils::write_int(uint8_t* stack, const uint64_t& index, const uint32_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
}

void byteutils::write_long(uint8_t* stack, const uint64_t& index, const uint64_t& value) {
    stack[index] = BYTE_0(value);
    stack[index + 1] = BYTE_1(value);
    stack[index + 2] = BYTE_2(value);
    stack[index + 3] = BYTE_3(value);
    stack[index + 4] = BYTE_4(value);
    stack[index + 5] = BYTE_5(value);
    stack[index + 6] = BYTE_6(value);
    stack[index + 7] = BYTE_7(value);
}

void byteutils::write_short(std::vector<uint8_t>& stack, const uint64_t& index, const uint16_t& value) {
    byteutils::write_short(stack.data(), index, value);
}

void byteutils::write_int(std::vector<uint8_t>& stack, const uint64_t& index, const uint32_t& value) {
    byteutils::write_int(stack.data(), index, value);
}

void byteutils::write_long(std::vector<uint8_t>& stack, const uint64_t& index, const uint64_t& value) {
    byteutils::write_long(stack.data(), index, value);
}