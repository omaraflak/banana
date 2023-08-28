#include "../src/lib/c_interface.h"

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
    while (n > 1) {
        if (is_prime(i)) {
            n--;
            l = i;
        }
        i+=2;
    }
    return l;
}

class PrimeFunction : public CInterface {
    cinterface::ArgType get_return_type() const {
        return cinterface::LONG;
    }

    std::vector<cinterface::ArgType> get_arg_types() const {
        return {cinterface::LONG};
    }

    std::string get_name() const {
        return "math::prime";
    }

    void* get_function() const {
        return (void*) nth_prime;
    }
};

std::vector<CInterface*> get_classes() {
    return {new PrimeFunction()};
}
