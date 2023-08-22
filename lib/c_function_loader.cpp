#include "c_function_loader.h"
#include <iostream>
#include <functional>
#include <dlfcn.h>
#include <ffi.h>

CFunctionLoader::CFunctionLoader() {}

CFunctionLoader::~CFunctionLoader() {
    functions.clear();
    functions_by_hash.clear();
    for (auto& handle : handles) {
        dlclose(handle);
    }
}

void CFunctionLoader::load(const std::vector<std::string>& shared_libraries) {
    for (auto path : shared_libraries) {
        void* handle = dlopen(path.c_str(), RTLD_NOW);
        if (handle == nullptr) {
            std::cout << dlerror() << std::endl;
            exit(1);
        }
        void* ptr = dlsym(handle, "get_classes");
        if (ptr == nullptr) {
            std::cout << dlerror() << std::endl;
            exit(1);
        }
        std::function<std::vector<CFunction*>()> get_classes = reinterpret_cast<std::vector<CFunction*>(*)()>(ptr);
        for (auto c_function_ptr : get_classes()) {
            functions.push_back(std::shared_ptr<CFunction>(c_function_ptr));
            functions_by_hash[CFunctionLoader::hasher(functions.back()->get_name())] = functions.back();
        }
    }
}

std::shared_ptr<CFunction> CFunctionLoader::get_function(const std::string& name) const {
    return get_function(CFunctionLoader::hasher(name));
}

std::shared_ptr<CFunction> CFunctionLoader::get_function(const size_t& hash) const {
    if (functions_by_hash.find(hash) == functions_by_hash.end()) {
        std::cout << "Could not find function " << hash << std::endl;
        exit(1);
    }
    return functions_by_hash.at(hash);
}


namespace cfunctionloader {
std::map<var::DataType, ffi_type*> DATA_TYPE_TO_FFI_TYPE = {
    {var::BOOL, &ffi_type_schar},
    {var::CHAR, &ffi_type_schar},
    {var::INT, &ffi_type_sint},
    {var::LONG, &ffi_type_slong},
};

const std::map<cfunction::ArgType, var::DataType> C_TYPE_TO_DATA_TYPE {
    {cfunction::BOOL, var::BOOL},
    {cfunction::CHAR, var::CHAR},
    {cfunction::INT, var::INT},
    {cfunction::LONG, var::LONG},
};
}

Var CFunctionLoader::call(const std::shared_ptr<CFunction>& function, const std::vector<Var>& args) {
    const int args_num = args.size();

    // argument types
    ffi_type *ffi_args[args_num];
    for (int i = 0; i < args_num; i++) {
        ffi_args[i] = cfunctionloader::DATA_TYPE_TO_FFI_TYPE.at(args.at(i).type);
    }
    
    // argument values
    void *values[args_num];
    for (int i = 0; i < args_num; i++) {
        values[i] = var::to_ptr(args.at(i));
    }

    // function return type
    var::DataType return_type = cfunctionloader::C_TYPE_TO_DATA_TYPE.at(function->get_return_type());
    auto ffi_return_type = cfunctionloader::DATA_TYPE_TO_FFI_TYPE.at(return_type);

    // function to call
    void (*f)() = reinterpret_cast<void (*)()>(function->get_function());
    
    ffi_cif cif;
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, args_num, ffi_return_type, ffi_args) == FFI_OK) {
        switch (return_type) {
            case var::BOOL: {
                char result;
                ffi_call(&cif, f, &result, values);
                return var::create_bool(result);
            }
            case var::CHAR: {
                char result;
                ffi_call(&cif, f, &result, values);
                return var::create_char(result);
            }
            case var::INT: {
                int result;
                ffi_call(&cif, f, &result, values);
                return var::create_int(result);
            }
            case var::LONG: {
                long result;
                ffi_call(&cif, f, &result, values);
                return var::create_long(result);
            }
        }
        std::cout << "Could not find type" << return_type << std::endl;
        exit(1);
    }

    std::cout << "Could not call function" << std::endl;
    exit(1);
}