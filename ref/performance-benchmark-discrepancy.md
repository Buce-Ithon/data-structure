# Performance Benchmark Discrepancy: `std::memset` vs. `std::fill_n` in String Matching

A drastic performance anomaly occurred during benchmark testing: Brute Force (BF) execution time dropped from **~17.7s** (`std::memset`) to **~133ms** (`std::fill_n`), while KMP execution time remained largely unchanged (~1000ms vs ~896ms). 

## Root Cause 1: Missing Null-Terminator & Validation Guard Failure
* **Buffer Overflow via `strlen`**: `std::fill_n(long_s, 248, 'a')` writes raw bytes without appending a `\0` null-terminator. Passing an un-terminated string to `StrAssign` causes `std::strlen` to read past the buffer, returning a length greater than `MAXLEN` (255).
* **Failed String Assignment**: The guard condition `if (len > MAXLEN) return false;` in `StrAssign` triggers, leaving `S.length` as `0` or uninitialized.
* **Bypassed Comparison Loop**: Inside `PatternMatchBF`, `while (i < S.length)` immediately evaluates to `false`. Running 1,000,000 iterations of no-op function calls takes ~133ms, creating the illusion of a massive speedup.

## Root Cause 2: Compiler Optimization and Transparency
* **Opaque C Library (`memset`)**: `memset` is treated as an external library call. The compiler cannot easily infer memory invariant properties, forcing full execution of the worst-case $O(N \times M)$ comparison loops (~15 billion character checks).
* **Transparent Template (`std::fill_n`)**: `std::fill_n` is an inlineable C++ template. If the compiler deduces static buffer contents, modern toolchains (GCC/Clang) can apply SIMD auto-vectorization, loop unrolling, or constant folding to the pattern matching logic.

## Secondary Finding: KMP Heap Allocation Bottleneck
* **Repeated Allocations**: `PatternMatchKMP` instantiates `std::vector<int> next(T.length)` inside the function body on every call.
* **Dominant Memory Overhead**: Performing 1,000,000 heap allocation/deallocation (`malloc`/`free`) cycles consumes ~900ms. This fixed runtime penalty completely masks the algorithmic efficiency of KMP.

## Optimization Checklist
* **Explicit Termination**: Always explicitly assign `buffer[length] = '\0'` after raw memory population.
* **Avoid Allocations in Hot Paths**: Replace `std::vector` inside `PatternMatchKMP` with stack-allocated arrays (e.g., `int next[MAXLEN]`) to measure raw search performance accurately.