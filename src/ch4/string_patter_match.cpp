#include <cstddef>
#include <string_c.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>

/**
 * @brief Simple pattern matching algorithm: Brute Force Matching (BF)
 * @param S Main string
 * @param T Pattern string
 * @return int The index of T in S (1-based)
 */
int PatternMatchBF(const StringCStyle::SString &S, const StringCStyle::SString &T) {
    int i = 0, j = 0;
    while (i < S.length && j < T.length) {
        if (S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            i = i - j + 1; // i - j + 1, i go to next position (0-based) and reloop
            j = 0;
        }
    }
    if (j == T.length) {
        return i - j + 1; // matching successfully: i - j + 1, the position index of T in S (1-based)
    }
    return 0; // matching failed: return 0
}

/**
 * @brief Get next[] array of KMP
 * @param T Pattern string
 * @param next Store the next index of j
 */
void GetNext(const StringCStyle::SString &T, int next[]) {
    int j = 0;
    int k = -1;
    next[0] = -1;
    
    while (j < T.length - 1) {
        if (k == -1 || T.ch[j] == T.ch[k]) {
            j++; k++;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

/**
 * @brief Better pattern matching algorithm: KMP Algorithm
 * @param S Main string
 * @param T Pattern string
 * @return int Return index (1-based) if matching successfully, else return 0
 */
int PatternMatchKMP(const StringCStyle::SString &S, const StringCStyle::SString &T) {
    if (T.length == 0) return 1;
    if (S.length < T.length) return 0;
    
    std::vector<int> next(static_cast<std::size_t>(T.length));
    GetNext(T, next.data());
    
    int i = 0, j = 0;
    
    while (i < S.length && j < T.length) {
        if (j == -1 || S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            j = next[static_cast<std::size_t>(j)];
        }
    }
    
    if (j == T.length) {
        return i - j + 1;
    }
    
    return 0;
}

int main() {
    StringCStyle::SString S, T;

    // 1. Helper lambda to test correctness for both BF and KMP
    auto run_test = [](int test_num, const StringCStyle::SString &s, const StringCStyle::SString &t, int expected) {
        int res_bf = PatternMatchBF(s, t);
        int res_kmp = PatternMatchKMP(s, t);
        bool passed = (res_bf == expected) && (res_kmp == expected);

        std::cout << "[Test " << test_num << "] " << (passed ? "PASSED" : "FAILED") << "\n";
        std::cout << "  Main string (S)   : \"";
        for (int i = 0; i < s.length; ++i) std::cout << s.ch[i];
        std::cout << "\"\n  Pattern string (T): \"";
        for (int i = 0; i < t.length; ++i) std::cout << t.ch[i];
        std::cout << "\"\n  BF Pos: " << res_bf << " | KMP Pos: " << res_kmp << " (Expected: " << expected << ")\n\n";
    };

    std::cout << "========================================\n";
    std::cout << " 1. Correctness Tests (BF & KMP)\n";
    std::cout << "========================================\n\n";

    // Test cases
    StringCStyle::StrAssign(S, "ababcabcacbab"); StringCStyle::StrAssign(T, "abcac");
    run_test(1, S, T, 6);

    StringCStyle::StrAssign(S, "hello world"); StringCStyle::StrAssign(T, "hello");
    run_test(2, S, T, 1);

    StringCStyle::StrAssign(S, "hello world"); StringCStyle::StrAssign(T, "world");
    run_test(3, S, T, 7);

    StringCStyle::StrAssign(S, "abcdefg"); StringCStyle::StrAssign(T, "xyz");
    run_test(4, S, T, 0);

    StringCStyle::StrAssign(S, "banana"); StringCStyle::StrAssign(T, "a");
    run_test(5, S, T, 2);

    StringCStyle::StrAssign(S, "pattern"); StringCStyle::StrAssign(T, "pattern");
    run_test(6, S, T, 1);

    StringCStyle::StrAssign(S, "short"); StringCStyle::StrAssign(T, "longer_pattern");
    run_test(7, S, T, 0);

    // 2. Performance Benchmark (Worst-Case Scenario)
    std::cout << "========================================\n";
    std::cout << " 2. Performance Benchmark (Worst Case)\n";
    std::cout << "========================================\n\n";

    // Constructing worst-case input: S = "aaaa...ab", T = "aaa...ab"
    char long_s[250];
    char long_t[100];
    std::memset(long_s, 'a', 248);
    long_s[248] = 'b';
    long_s[249] = '\0';

    std::memset(long_t, 'a', 98);
    long_t[98] = 'b';
    long_t[99] = '\0';

    StringCStyle::StrAssign(S, long_s);
    StringCStyle::StrAssign(T, long_t);

    const int iterations = 1000000;

    // Benchmark BF Algorithm
    auto start_bf = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int pos = PatternMatchBF(S, T);
        (void)pos;
    }
    auto end_bf = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_bf = end_bf - start_bf;

    // Benchmark KMP Algorithm
    auto start_kmp = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int pos = PatternMatchKMP(S, T);
        (void)pos;
    }
    auto end_kmp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_kmp = end_kmp - start_kmp;

    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "  - Brute Force (BF) Time : " << duration_bf.count() << " ms\n";
    std::cout << "  - KMP Algorithm Time    : " << duration_kmp.count() << " ms\n";
    std::cout << "  - Speedup Ratio (BF/KMP): " << (duration_bf.count() / duration_kmp.count()) << "x\n";
    
    return 0;
}