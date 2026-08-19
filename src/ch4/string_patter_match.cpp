#include <string_c.h>
#include <iostream>

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

int main() {
    StringCStyle::SString S, T;

    // Helper lambda to print test results
    auto run_test = [](int test_num, const StringCStyle::SString &s, const StringCStyle::SString &t, int expected) {
        int result = PatternMatchBF(s, t);
        bool passed = (result == expected);
        std::cout << "[Test " << test_num << "] ";
        if (passed) {
            std::cout << "PASSED";
        } else {
            std::cout << "FAILED (Expected: " << expected << ", Got: " << result << ")";
        }
        std::cout << "\n  Main string (S)   : \"";
        for (int i = 0; i < s.length; ++i) std::cout << s.ch[i];
        std::cout << "\"\n  Pattern string (T): \"";
        for (int i = 0; i < t.length; ++i) std::cout << t.ch[i];
        std::cout << "\"\n  Matched Position  : " << result << "\n\n";
    };

    std::cout << "========================================\n";
    std::cout << " Brute Force Pattern Matching Tests\n";
    std::cout << "========================================\n\n";

    // Test 1: Standard match with backtracking
    StringCStyle::StrAssign(S, "ababcabcacbab");
    StringCStyle::StrAssign(T, "abcac");
    run_test(1, S, T, 6);

    // Test 2: Pattern found at the very beginning
    StringCStyle::StrAssign(S, "hello world");
    StringCStyle::StrAssign(T, "hello");
    run_test(2, S, T, 1);

    // Test 3: Pattern found at the very end
    StringCStyle::StrAssign(S, "hello world");
    StringCStyle::StrAssign(T, "world");
    run_test(3, S, T, 7);

    // Test 4: Pattern not found
    StringCStyle::StrAssign(S, "abcdefg");
    StringCStyle::StrAssign(T, "xyz");
    run_test(4, S, T, 0);

    // Test 5: Single character pattern match
    StringCStyle::StrAssign(S, "banana");
    StringCStyle::StrAssign(T, "a");
    run_test(5, S, T, 2);

    // Test 6: Pattern length equals main string length (exact match)
    StringCStyle::StrAssign(S, "pattern");
    StringCStyle::StrAssign(T, "pattern");
    run_test(6, S, T, 1);

    // Test 7: Pattern length exceeds main string length
    StringCStyle::StrAssign(S, "short");
    StringCStyle::StrAssign(T, "longer_pattern");
    run_test(7, S, T, 0);
    return 0;
}