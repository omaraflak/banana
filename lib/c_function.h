#if !defined(C_FUNCTION)
#define C_FUNCTION

#include <vector>
#include <string>

namespace cfunction {
enum ArgType {
    BOOL, CHAR, INT, LONG
};
}

class CFunction {
    public:
    virtual ~CFunction() = default;

    virtual cfunction::ArgType get_return_type() const = 0;
    virtual std::vector<cfunction::ArgType> get_arg_types() const = 0;
    virtual std::string get_name() const = 0;
    virtual void* get_function() const = 0;
};

extern "C" std::vector<CFunction*> get_classes();

#endif // C_FUNCTION
