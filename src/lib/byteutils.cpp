#include "byteutils.h"

void byteutils::push_short(std::vector<uint8_t>& stack, const int16_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
}

void byteutils::push_int(std::vector<uint8_t>& stack, const int32_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
}

void byteutils::push_long(std::vector<uint8_t>& stack, const int64_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
    stack.push_back(BYTE_4(value));
    stack.push_back(BYTE_5(value));
    stack.push_back(BYTE_6(value));
    stack.push_back(BYTE_7(value));
}

void byteutils::push_ulong(std::vector<uint8_t>& stack, const uint64_t& value) {
    stack.push_back(BYTE_0(value));
    stack.push_back(BYTE_1(value));
    stack.push_back(BYTE_2(value));
    stack.push_back(BYTE_3(value));
    stack.push_back(BYTE_4(value));
    stack.push_back(BYTE_5(value));
    stack.push_back(BYTE_6(value));
    stack.push_back(BYTE_7(value));
}

int16_t byteutils::read_short(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return ((int16_t) stack[index + 1] << 8) | stack[index + 0];
}

int32_t byteutils::read_int(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return
        ((int32_t) stack[index + 3] << 24) |
        ((int32_t) stack[index + 2] << 16) |
        ((int32_t) stack[index + 1] << 8) |
        stack[index + 0];
}

int64_t byteutils::read_long(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return
        ((int64_t) stack[index + 7] << 56) |
        ((int64_t) stack[index + 6] << 48) |
        ((int64_t) stack[index + 5] << 40) |
        ((int64_t) stack[index + 4] << 32) |
        ((int64_t) stack[index + 3] << 24) |
        ((int64_t) stack[index + 2] << 16) |
        ((int64_t) stack[index + 1] << 8) |
        stack[index + 0];
}

uint64_t byteutils::read_ulong(const std::vector<uint8_t>& stack, const uint64_t& index) {
    return
        ((uint64_t) stack[index + 7] << 56) |
        ((uint64_t) stack[index + 6] << 48) |
        ((uint64_t) stack[index + 5] << 40) |
        ((uint64_t) stack[index + 4] << 32) |
        ((uint64_t) stack[index + 3] << 24) |
        ((uint64_t) stack[index + 2] << 16) |
        ((uint64_t) stack[index + 1] << 8) |
        stack[index + 0];
}