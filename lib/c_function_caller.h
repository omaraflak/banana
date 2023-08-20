#if !defined(C_FUNCTION_CALLER)
#define C_FUNCTION_CALLER

#include "c_function.h"
#include "var.h"
#include <ffi.h>
#include <memory>
#include <vector>

namespace cfunctioncaller {
Var call(const std::shared_ptr<CFunction>& function, const std::vector<Var>& args);
}

#endif // C_FUNCTION_CALLER
