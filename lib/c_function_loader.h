#if !defined(C_FUNCTION_LOADER)
#define C_FUNCTION_LOADER

#include "var.h"
#include "c_function.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class CFunctionLoader {
    public:
    CFunctionLoader();
    ~CFunctionLoader();

    void load(const std::vector<std::string>& shared_libraries);
    std::shared_ptr<CFunction> get_function(const std::string& name) const;
    std::shared_ptr<CFunction> get_function(const size_t& hash) const;

    static Var call(const std::shared_ptr<CFunction>& function, const std::vector<Var>& args);

    private:
    std::vector<void*> handles;
    std::vector<std::shared_ptr<CFunction>> functions;
    std::map<size_t, std::shared_ptr<CFunction>> functions_by_hash;

    static std::hash<std::string> hasher;
};


#endif // C_FUNCTION_LOADER
