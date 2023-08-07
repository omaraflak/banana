#if !defined(VM)
#define VM
#define MAX_MEMORY (2 << 16)

#include <vector>
#include <forward_list>
#include <stdint.h>

class Vm {
    public:
    Vm(std::vector<uint8_t> program);
    ~Vm();

    void execute();
    void new_stack();
    void pop_stack();

    std::vector<uint8_t>* stack;
    std::vector<uint64_t> call_stack;
    std::vector<uint8_t> program;
    std::forward_list<std::vector<uint8_t>> stacks;
    uint8_t* memory;
    uint64_t ip;
    bool running;
    bool debug;
};

#endif // VM
