#if !defined(C_FUNCTION_LOADER)
#define C_FUNCTION_LOADER

#include "var.h"
#include "c_interface.h"
#include <map>
#include <string>
#include <vector>
#include <memory>

class CFunctions {
    public:
    ~CFunctions();

    void load(const std::vector<std::string>& shared_libraries);
    std::shared_ptr<CInterface> get_function(const std::string& name) const;
    std::shared_ptr<CInterface> get_function(const size_t& hash) const;

    static Var call(const std::shared_ptr<CInterface>& function, const std::vector<Var>& args);

    private:
    std::vector<void*> handles;
    std::map<size_t, std::shared_ptr<CInterface>> functions_by_hash;

    static std::hash<std::string> hasher;
};


#endif // C_FUNCTION_LOADER
