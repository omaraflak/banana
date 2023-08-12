#if !defined(VM)
#define VM
#define MAX_MEMORY (2 << 16)

#include <vector>
#include <forward_list>
#include <stdint.h>

class Vm {
    public:
    Vm(std::vector<uint8_t> program);

    void execute();
    void push_frame();
    void pop_frame();

    std::vector<uint8_t>* stack;
    std::vector<uint8_t>* heap;
    std::vector<uint64_t> call_stack;
    std::vector<uint8_t> program;
    std::forward_list<std::vector<uint8_t>> stacks;
    std::forward_list<std::vector<uint8_t>> heaps;
    uint64_t ip;
    bool running;
};

#endif // VM
