#include "c_function_loader.h"
#include <iostream>
#include <functional>
#include <dlfcn.h>

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