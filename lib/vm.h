#if !defined(VM)
#define VM
#define HEAP_MEMORY (2 << 16)

#include <map>
#include <stack>
#include <memory>
#include <vector>
#include <stdint.h>
#include "c_interface.h"
#include "c_functions.h"
#include "var.h"

class Vm {
    public:
    ~Vm();
    Vm(const std::vector<uint8_t>& program, const std::vector<std::string>& shared_libraries = std::vector<std::string>());

    void execute();
    void push_frame();
    void pop_frame();

    Var* heap;
    std::vector<Var>* stack;
    std::vector<uint8_t> program;
    std::stack<uint64_t> call_stack;
    std::stack<std::vector<Var>> stacks;
    std::stack<Var*> heaps;
    uint64_t ip;
    bool running;
    CFunctions c_functions;
};

#endif // VM
