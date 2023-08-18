#if !defined(VAR)
#define VAR

#include <map>
#include <vector>
#include <string>
#include <stdint.h>

union Data {
    bool _bool;
    char _char;
    int _int;
    long _long;
};

enum DataType {
    BOOL, CHAR, INT, LONG
};

struct Var {
    Data data;
    DataType type;
};

namespace var {
const std::map<DataType, std::string> TYPE_NAME = {
    {BOOL, "bool"},
    {CHAR, "char"},
    {INT, "int"},
    {LONG, "long"},
};

template <class K, class V>
std::map<V, K> reverse_map(const std::map<K, V>& m) {
    std::map<V, K> result;
    for (const auto& pair : m) {
        result[pair.second] = pair.first;
    }
    return result;
}

const std::map<std::string, DataType> TYPE_NAME_REVERSED = reverse_map(TYPE_NAME);

void push(const Var &var, std::vector<uint8_t>& bytes);
Var read(const std::vector<uint8_t>& bytes, uint64_t* index);

std::string to_string(const Var& var);
Var from_string(const std::vector<std::string>& strings);

uint8_t size(const Var& var);
Var convert(const Var& var, const DataType& type);

Var create_char(const char& value);
Var create_int(const int& value);
Var create_long(const long& value);
Var create_bool(const bool& value);

Var add(const Var& left, const Var& right);
Var sub(const Var& left, const Var& right);
Var mul(const Var& left, const Var& right);
Var div(const Var& left, const Var& right);
Var mod(const Var& left, const Var& right);
Var binary_xor(const Var& left, const Var& right);
Var binary_and(const Var& left, const Var& right);
Var binary_or(const Var& left, const Var& right);

Var lt(const Var& left, const Var& right);
Var lte(const Var& left, const Var& right);
Var gt(const Var& left, const Var& right);
Var gte(const Var& left, const Var& right);
Var eq(const Var& left, const Var& right);
Var neq(const Var& left, const Var& right);
Var boolean_and(const Var& left, const Var& right);
Var boolean_or(const Var& left, const Var& right);

Var binary_not(const Var& var);
Var boolean_not(const Var& var);

void print(const Var& var);
}

#endif // VAR