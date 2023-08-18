#include "var.h"
#include "../lib/byteutils.h"
#include <iostream>
#include <sstream>

#define BINARY_OPERATION(left, right, op) \
    switch (left.type) { \
        case CHAR: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._char op (char) right.data._bool); \
                case CHAR: \
                    return create_char(left.data._char op right.data._char); \
                case INT: \
                    return create_int(left.data._char op right.data._int); \
                case LONG: \
                    return create_long(left.data._char op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case INT: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._int op (int) right.data._bool); \
                case CHAR: \
                    return create_int(left.data._int op right.data._char); \
                case INT: \
                    return create_int(left.data._int op right.data._int); \
                case LONG: \
                    return create_long(left.data._int op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case LONG: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._long op (long) right.data._bool); \
                case CHAR: \
                    return create_long(left.data._long op right.data._char); \
                case INT: \
                    return create_long(left.data._long op right.data._int); \
                case LONG: \
                    return create_long(left.data._long op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case BOOL: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._bool op right.data._bool); \
                case CHAR: \
                    return create_long((char) left.data._bool op right.data._char); \
                case INT: \
                    return create_long((short) left.data._bool op right.data._int); \
                case LONG: \
                    return create_long((long) left.data._bool op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        default: \
            var::type_not_found(left.type); \
            exit(1); \
    } \

#define COMPARE_OPERATION(left, right, op) \
    switch (left.type) { \
        case CHAR: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._char op (char) right.data._bool); \
                case CHAR: \
                    return create_bool(left.data._char op right.data._char); \
                case INT: \
                    return create_bool(left.data._char op right.data._int); \
                case LONG: \
                    return create_bool(left.data._char op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case INT: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._int op (int) right.data._bool); \
                case CHAR: \
                    return create_bool(left.data._int op right.data._char); \
                case INT: \
                    return create_bool(left.data._int op right.data._int); \
                case LONG: \
                    return create_bool(left.data._int op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case LONG: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._long op (long) right.data._bool); \
                case CHAR: \
                    return create_bool(left.data._long op right.data._char); \
                case INT: \
                    return create_bool(left.data._long op right.data._int); \
                case LONG: \
                    return create_bool(left.data._long op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        case BOOL: \
            switch (right.type) { \
                case BOOL: \
                    return create_bool(left.data._bool op right.data._bool); \
                case CHAR: \
                    return create_bool((char) left.data._bool op right.data._char); \
                case INT: \
                    return create_bool((short) left.data._bool op right.data._int); \
                case LONG: \
                    return create_bool((long) left.data._bool op right.data._long); \
                default: \
                    var::type_not_found(right.type); \
                    exit(1); \
            } \
        default: \
            var::type_not_found(left.type); \
            exit(1); \
    } \

#define UNARY_OPERATION(value, op) \
    switch (value.type) { \
        case BOOL: \
            return create_bool(op value.data._bool); \
        case CHAR: \
            return create_char(op value.data._char); \
        case INT: \
            return create_int(op value.data._int); \
        case LONG: \
            return create_long(op value.data._long); \
        default: \
            var::type_not_found(value.type); \
            exit(1); \
    } \

namespace var {
void type_not_found(const DataType& type) {
    std::cout << "Type not found: " << type << std::endl;
}
}

void var::push(const Var &var, std::vector<uint8_t>& bytes) {
    bytes.push_back(var.type);
    switch (var.type) {
        case BOOL:
            bytes.push_back(var.data._bool);
            break;
        case CHAR:
            bytes.push_back(var.data._char);
            break;
        case INT:
            byteutils::push_int(bytes, var.data._int);
            break;
        case LONG:
            byteutils::push_long(bytes, var.data._long);
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
}

Var var::read(const std::vector<uint8_t>& bytes, uint64_t* index) {
    Var var;
    var.type = (DataType) bytes[*index];
    *index += SIZE_OF_BYTE;
    switch (var.type) {
        case BOOL:
            var.data._bool = bytes[*index];
            *index += SIZE_OF_BYTE;
            break;
        case CHAR:
            var.data._char = bytes[*index];
            *index += SIZE_OF_BYTE;
            break;
        case INT:
            var.data._int = byteutils::read_int(bytes, *index);
            *index += SIZE_OF_INT;
            break;
        case LONG:
            var.data._long = byteutils::read_long(bytes, *index);
            *index += SIZE_OF_LONG;
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
    return var;
}

std::string var::to_string(const Var& var) {
    std::stringstream ss;
    ss << TYPE_NAME.at(var.type) << " ";
    switch (var.type) {
        case BOOL:
            ss << var.data._bool;
            break;
        case CHAR:
            ss << (int) var.data._char;
            break;
        case INT:
            ss << var.data._int;
            break;
        case LONG:
            ss << var.data._long;
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
    return ss.str();
}

Var var::from_string(const std::vector<std::string>& strings) {
    Var var;
    var.type = TYPE_NAME_REVERSED.at(strings[0]);
    switch (var.type) {
        case BOOL:
            var.data._bool = stol(strings[1]);
            break;
        case CHAR:
            var.data._char = stol(strings[1]);
            break;
        case INT:
            var.data._int = stoi(strings[1]);
            break;
        case LONG:
            var.data._long = stol(strings[1]);
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
    return var;
}

uint8_t var::size(const Var& var) {
    uint8_t size = SIZE_OF_BYTE;
    switch (var.type) {
        case BOOL:
            size += SIZE_OF_BYTE;
            break;
        case CHAR:
            size += SIZE_OF_BYTE;
            break;
        case INT:
            size += SIZE_OF_INT;
            break;
        case LONG:
            size += SIZE_OF_LONG;
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
    return size;
}

Var var::convert(const Var& var, const DataType& type) {
    switch (var.type) {
        case CHAR:
            switch (type) {
                case BOOL:
                    return create_bool(var.data._char);
                case CHAR:
                    return var;
                case INT:
                    return create_int(var.data._char);
                case LONG:
                    return create_long(var.data._char);
                default:
                    var::type_not_found(var.type);
                    exit(1);
            }
        case INT:
            switch (type) {
                case BOOL:
                    return create_bool(var.data._int);
                case CHAR:
                    return create_char(var.data._int);
                case INT:
                    return var;
                case LONG:
                    return create_long(var.data._int);
                default:
                    var::type_not_found(var.type);
                    exit(1);
            }
        case LONG:
            switch (type) {
                case BOOL:
                    return create_bool(var.data._long);
                case CHAR:
                    return create_char(var.data._long);
                case INT:
                    return create_int(var.data._long);
                case LONG:
                    return var;
                default:
                    var::type_not_found(var.type);
                    exit(1);
            }
        case BOOL:
            switch (type) {
                case BOOL:
                    return var;
                case CHAR:
                    return create_char(var.data._bool);
                case INT:
                    return create_int(var.data._bool);
                case LONG:
                    return create_long(var.data._bool);
                default:
                    var::type_not_found(var.type);
                    exit(1);
            }
        default:
            var::type_not_found(var.type);
            exit(1);
    }
}

Var var::create_char(const char& value) {
    Var var;
    var.type = CHAR;
    var.data._char = value;
    return var;
}

Var var::create_int(const int& value) {
    Var var;
    var.type = INT;
    var.data._int = value;
    return var;
}

Var var::create_long(const long& value) {
    Var var;
    var.type = LONG;
    var.data._long = value;
    return var;
}

Var var::create_bool(const bool& value) {
    Var var;
    var.type = BOOL;
    var.data._bool = value;
    return var;
}

Var var::add(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, +);
}

Var var::sub(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, -);
}

Var var::mul(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, *);
}

Var var::div(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, /);
}

Var var::mod(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, %);
}

Var var::binary_xor(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, ^);
}

Var var::binary_and(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, &);
}

Var var::binary_or(const Var& left, const Var& right) {
    BINARY_OPERATION(left, right, |);
}

Var var::lt(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, <);
}

Var var::lte(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, <=);
}

Var var::gt(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, >);
}

Var var::gte(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, >=);
}

Var var::eq(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, ==);
}

Var var::neq(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, !=);
}

Var var::boolean_and(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, &&);
}

Var var::boolean_or(const Var& left, const Var& right) {
    COMPARE_OPERATION(left, right, ||);
}

Var var::binary_not(const Var& var) {
    UNARY_OPERATION(var, ~);
}

Var var::boolean_not(const Var& var) {
    UNARY_OPERATION(var, !);
}

void var::print(const Var& var) {
    switch (var.type) {
        case BOOL:
            std::cout << (var.data._bool ? "true" : "false");
            break;
        case CHAR:
            std::cout << var.data._char;
            break;
        case INT:
            std::cout << var.data._int;
            break;
        case LONG:
            std::cout << var.data._long;
            break;
        default:
            var::type_not_found(var.type);
            exit(1);
    }
}
