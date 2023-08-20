#include "../lib/c_function.h"

bool is_prime(const long& n) {
    for (long i=2; i<n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

long nth_prime(long n) {
    long i = 3;
    long l = i;
    while (n >= 0) {
        if (is_prime(i)) {
            n--;
            l = i;
        }
        i+=2;
    }
    return l;
}

class PrimeFunction : public CFunction {
    cfunction::ArgType get_return_type() const {
        return cfunction::LONG;
    }

    std::vector<cfunction::ArgType> get_arg_types() const {
        return {cfunction::LONG};
    }

    std::string get_module_name() const {
        return "math";
    }

    std::string get_function_name() const {
        return "prime";
    }

    void* get_function() const {
        return (void*) nth_prime;
    }
};

CFunction* get_factory() {
    return new PrimeFunction();
}
