#if !defined(VM)
#define VM
#define MAX_MEMORY (2 << 16)

#include <vector>
#include <stdint.h>

class Vm {
    public:
    Vm(std::vector<uint8_t> program);
    ~Vm();

    void execute();
    
    std::vector<uint8_t> stack;
    std::vector<uint64_t> call_stack;
    std::vector<uint8_t> program;
    uint8_t* memory;
    uint64_t ip;
    bool running;
    bool debug;
};

#endif // VM
