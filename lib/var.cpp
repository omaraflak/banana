#include "var.h"
#include "../lib/byteutils.h"
#include <iostream>
#include <cassert>

namespace var {
void type_not_found(const Var& var) {
    std::cout << "Type not found: " << var.type << std::endl;
}
}

void var::push(const Var &var, std::vector<uint8_t>& bytes) {
    switch (var.type) {
        case BYTE:
            bytes.push_back(var.data._byte);
            break;
        case SHORT:
            byteutils::push_short(bytes, var.data._short);
            break;
        case INT:
            byteutils::push_short(bytes, var.data._int);
            break;
        case LONG:
            byteutils::push_short(bytes, var.data._long);
            break;
        case BOOL:
            bytes.push_back(var.data._bool);
            break;
        default:
            var::type_not_found(var);
            exit(1);
    }
    bytes.push_back(var.type);
}

Var var::pop(std::vector<uint8_t>& bytes) {
    Var var;
    var.type = (DataType) byteutils::pop_byte(bytes);
    switch (var.type) {
        case BYTE:
            var.data._byte = byteutils::pop_byte(bytes);
            break;
        case SHORT:
            var.data._short = byteutils::pop_short(bytes);
            break;
        case INT:
            var.data._int = byteutils::pop_int(bytes);
            break;
        case LONG:
            var.data._long = byteutils::pop_long(bytes);
            break;
        case BOOL:
            var.data._bool = byteutils::pop_byte(bytes);
            break;
        default:
            var::type_not_found(var);
            exit(1);
    }
    return var;
}

Var var::create_byte(const char& value) {
    Var var;
    var.type = BYTE;
    var.data._byte = value;
    return var;
}

Var var::create_short(const short& value) {
    Var var;
    var.type = SHORT;
    var.data._short = value;
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
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte + right.data._byte);
        case SHORT:
            return create_short(left.data._short + right.data._short);
        case INT:
            return create_int(left.data._int + right.data._int);
        case LONG:
            return create_long(left.data._long + right.data._long);
        case BOOL:
            return create_bool(left.data._bool + right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::sub(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte - right.data._byte);
        case SHORT:
            return create_short(left.data._short - right.data._short);
        case INT:
            return create_int(left.data._int - right.data._int);
        case LONG:
            return create_long(left.data._long - right.data._long);
        case BOOL:
            return create_bool(left.data._bool - right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::mul(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte * right.data._byte);
        case SHORT:
            return create_short(left.data._short * right.data._short);
        case INT:
            return create_int(left.data._int * right.data._int);
        case LONG:
            return create_long(left.data._long * right.data._long);
        case BOOL:
            return create_bool(left.data._bool * right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::div(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte / right.data._byte);
        case SHORT:
            return create_short(left.data._short / right.data._short);
        case INT:
            return create_int(left.data._int / right.data._int);
        case LONG:
            return create_long(left.data._long / right.data._long);
        case BOOL:
            return create_bool(left.data._bool / right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::mod(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte % right.data._byte);
        case SHORT:
            return create_short(left.data._short % right.data._short);
        case INT:
            return create_int(left.data._int % right.data._int);
        case LONG:
            return create_long(left.data._long % right.data._long);
        case BOOL:
            return create_bool(left.data._bool % right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::binary_xor(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte ^ right.data._byte);
        case SHORT:
            return create_short(left.data._short ^ right.data._short);
        case INT:
            return create_int(left.data._int ^ right.data._int);
        case LONG:
            return create_long(left.data._long ^ right.data._long);
        case BOOL:
            return create_bool(left.data._bool ^ right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::binary_and(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte & right.data._byte);
        case SHORT:
            return create_short(left.data._short & right.data._short);
        case INT:
            return create_int(left.data._int & right.data._int);
        case LONG:
            return create_long(left.data._long & right.data._long);
        case BOOL:
            return create_bool(left.data._bool & right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::binary_or(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte | right.data._byte);
        case SHORT:
            return create_short(left.data._short | right.data._short);
        case INT:
            return create_int(left.data._int | right.data._int);
        case LONG:
            return create_long(left.data._long | right.data._long);
        case BOOL:
            return create_bool(left.data._bool | right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::boolean_and(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte && right.data._byte);
        case SHORT:
            return create_short(left.data._short && right.data._short);
        case INT:
            return create_int(left.data._int && right.data._int);
        case LONG:
            return create_long(left.data._long && right.data._long);
        case BOOL:
            return create_bool(left.data._bool && right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::boolean_or(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte || right.data._byte);
        case SHORT:
            return create_short(left.data._short || right.data._short);
        case INT:
            return create_int(left.data._int || right.data._int);
        case LONG:
            return create_long(left.data._long || right.data._long);
        case BOOL:
            return create_bool(left.data._bool || right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}


Var var::lt(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte < right.data._byte);
        case SHORT:
            return create_short(left.data._short < right.data._short);
        case INT:
            return create_int(left.data._int < right.data._int);
        case LONG:
            return create_long(left.data._long < right.data._long);
        case BOOL:
            return create_bool(left.data._bool < right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::lte(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte <= right.data._byte);
        case SHORT:
            return create_short(left.data._short <= right.data._short);
        case INT:
            return create_int(left.data._int <= right.data._int);
        case LONG:
            return create_long(left.data._long <= right.data._long);
        case BOOL:
            return create_bool(left.data._bool <= right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::gt(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte > right.data._byte);
        case SHORT:
            return create_short(left.data._short > right.data._short);
        case INT:
            return create_int(left.data._int > right.data._int);
        case LONG:
            return create_long(left.data._long > right.data._long);
        case BOOL:
            return create_bool(left.data._bool > right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::gte(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte >= right.data._byte);
        case SHORT:
            return create_short(left.data._short >= right.data._short);
        case INT:
            return create_int(left.data._int >= right.data._int);
        case LONG:
            return create_long(left.data._long >= right.data._long);
        case BOOL:
            return create_bool(left.data._bool >= right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::eq(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte == right.data._byte);
        case SHORT:
            return create_short(left.data._short == right.data._short);
        case INT:
            return create_int(left.data._int == right.data._int);
        case LONG:
            return create_long(left.data._long == right.data._long);
        case BOOL:
            return create_bool(left.data._bool == right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}

Var var::neq(const Var& left, const Var& right) {
    assert(left.type == right.type);
    switch (left.type) {
        case BYTE:
            return create_byte(left.data._byte != right.data._byte);
        case SHORT:
            return create_short(left.data._short != right.data._short);
        case INT:
            return create_int(left.data._int != right.data._int);
        case LONG:
            return create_long(left.data._long != right.data._long);
        case BOOL:
            return create_bool(left.data._bool != right.data._bool);
        default:
            var::type_not_found(left);
            exit(1);
    }
}


Var var::binary_not(const Var& var) {
    switch (var.type) {
        case BYTE:
            return create_byte(~var.data._byte);
        case SHORT:
            return create_short(~var.data._short);
        case INT:
            return create_int(~var.data._int);
        case LONG:
            return create_long(~var.data._long);
        case BOOL:
            return create_bool(~var.data._bool);
        default:
            var::type_not_found(var);
            exit(1);
    }
}

Var var::boolean_not(const Var& var) {
    switch (var.type) {
        case BYTE:
            return create_byte(!var.data._byte);
        case SHORT:
            return create_short(!var.data._short);
        case INT:
            return create_int(!var.data._int);
        case LONG:
            return create_long(!var.data._long);
        case BOOL:
            return create_bool(!var.data._bool);
        default:
            var::type_not_found(var);
            exit(1);
    }
}

void var::print(const Var& var) {
    switch (var.type) {
        case BYTE:
            std::cout << var.data._byte;
            break;
        case SHORT:
            std::cout << var.data._short;
            break;
        case INT:
            std::cout << var.data._int;
            break;
        case LONG:
            std::cout << var.data._long;
            break;
        case BOOL:
            std::cout << var.data._bool;
            break;
        default:
            var::type_not_found(var);
            exit(1);
    }
}

void var::print_char(const Var& var) {
    switch (var.type) {
        case BYTE:
            std::cout << (char) var.data._byte;
            break;
        case SHORT:
            std::cout << (char) var.data._short;
            break;
        case INT:
            std::cout << (char) var.data._int;
            break;
        case LONG:
            std::cout << (char) var.data._long;
            break;
        case BOOL:
            std::cout << (char) var.data._bool;
            break;
        default:
            var::type_not_found(var);
            exit(1);
    }
}
