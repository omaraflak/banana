#include <benchmark/benchmark.h>
#include "../src/lib/ast.h"
#include "../src/lib/vm.h"
#include "../src/lib/instructions.h"
#include "../src/lib/fileutils.h"
#include "../src/lib/scanner.h"
#include "../src/lib/parser.h"

#define PATH(name) "benchmarks/" #name ".na"

#define NA_BENCHMARK(name) \
static void bm_##name(benchmark::State &state) { \
    auto content = fileutils::read_string(PATH(name)); \
    auto tokens = scanner::scan(content.c_str()); \
    auto tree = parser::parse(tokens); \
    auto instructions = ast::to_instructions(tree); \
    auto bytes = Instruction::to_bytes(instructions); \
    for (auto _ : state) { \
        Vm(bytes).execute(); \
    } \
} \
BENCHMARK(bm_##name) \

NA_BENCHMARK(fib);
NA_BENCHMARK(primes);

// Run the benchmark
BENCHMARK_MAIN();