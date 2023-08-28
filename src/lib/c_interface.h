#if !defined(C_FUNCTION)
#define C_FUNCTION

#include <vector>
#include <string>

namespace cinterface {
enum ArgType {
    BOOL, CHAR, INT, LONG
};
}

class CInterface {
    public:
    virtual ~CInterface() = default;

    virtual cinterface::ArgType get_return_type() const = 0;
    virtual std::vector<cinterface::ArgType> get_arg_types() const = 0;
    virtual std::string get_name() const = 0;
    virtual void* get_function() const = 0;
};

extern "C" std::vector<CInterface*> get_classes();

#endif // C_FUNCTION
