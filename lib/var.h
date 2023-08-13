#if !defined(VAR)
#define VAR

#include <vector>
#include <string>
#include <stdint.h>

union Data {
    char _byte;
    short _short;
    int _int;
    long _long;
    bool _bool;
};

enum DataType {
    BYTE, SHORT, INT, LONG,
    BOOL
};

struct Var {
    Data data;
    DataType type;
};

namespace var {
    void push(const Var &var, std::vector<uint8_t>& bytes);
    Var pop(std::vector<uint8_t>& bytes);

    Var create_byte(const char& value);
    Var create_short(const short& value);
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
    Var boolean_and(const Var& left, const Var& right);
    Var boolean_or(const Var& left, const Var& right);
    
    Var lt(const Var& left, const Var& right);
    Var lte(const Var& left, const Var& right);
    Var gt(const Var& left, const Var& right);
    Var gte(const Var& left, const Var& right);
    Var eq(const Var& left, const Var& right);
    Var neq(const Var& left, const Var& right);

    Var binary_not(const Var& var);
    Var boolean_not(const Var& var);

    void print(const Var& var);
    void print_char(const Var& var);
}

#endif // VAR