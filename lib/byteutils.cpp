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

namespace byteutils {
uint16_t read_short(const uint8_t* stack, const uint64_t& index) {
    return ((uint16_t) stack[index + 1] << 8) | stack[index];
}

uint32_t read_int(const uint8_t* stack, const uint64_t& index) {
    return ((uint32_t) read_short(stack, index + 2) << 16) | read_short(stack, index);
}

uint64_t read_long(const uint8_t* stack, const uint64_t& index) {
    return ((uint64_t) read_int(stack, index + 4) << 32) | read_int(stack, index);
}
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
