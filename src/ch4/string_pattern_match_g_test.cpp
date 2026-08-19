#include <gtest/gtest.h>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <vector>
#include "string_c.h"

// ============================================================================
// Algorithms Implementation
// ============================================================================

int PatternMatchBF(const StringCStyle::SString &S, const StringCStyle::SString &T) {
    int i = 0, j = 0;
    while (i < S.length && j < T.length) {
        if (S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == T.length) {
        return i - j + 1;
    }
    return 0;
}

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

// 1. KMP using Heap Allocation (std::vector)
int PatternMatchKMP_Heap(const StringCStyle::SString &S, const StringCStyle::SString &T) {
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
    if (j == T.length) return i - j + 1;
    return 0;
}

// 2. KMP using Stack Allocation (Fixed Array)
int PatternMatchKMP_Stack(const StringCStyle::SString &S, const StringCStyle::SString &T) {
    if (T.length == 0) return 1;
    if (S.length < T.length) return 0;

    int next[StringCStyle::MAXLEN]; // Fixed-size array allocated on stack
    GetNext(T, next);

    int i = 0, j = 0;
    while (i < S.length && j < T.length) {
        if (j == -1 || S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            j = next[j];
        }
    }
    if (j == T.length) return i - j + 1;
    return 0;
}

// ============================================================================
// GoogleTest Suite
// ============================================================================

// 1. Correctness Tests across all 3 matching implementations
TEST(PatternMatchingTest, FunctionalCorrectness) {
    StringCStyle::SString S, T;

    // Normal Match
    StringCStyle::StrAssign(S, "ababcabcacbab");
    StringCStyle::StrAssign(T, "abcac");
    EXPECT_EQ(PatternMatchBF(S, T), 6);
    EXPECT_EQ(PatternMatchKMP_Heap(S, T), 6);
    EXPECT_EQ(PatternMatchKMP_Stack(S, T), 6);

    // No Match
    StringCStyle::StrAssign(S, "abcdefg");
    StringCStyle::StrAssign(T, "xyz");
    EXPECT_EQ(PatternMatchBF(S, T), 0);
    EXPECT_EQ(PatternMatchKMP_Heap(S, T), 0);
    EXPECT_EQ(PatternMatchKMP_Stack(S, T), 0);

    // Edge Case: Match at end
    StringCStyle::StrAssign(S, "hello world");
    StringCStyle::StrAssign(T, "world");
    EXPECT_EQ(PatternMatchBF(S, T), 7);
    EXPECT_EQ(PatternMatchKMP_Heap(S, T), 7);
    EXPECT_EQ(PatternMatchKMP_Stack(S, T), 7);
}

// 2. Initialization Safety Tests (memset vs std::fill_n)
TEST(StringInitTest, MemsetInitialization) {
    char buf[250];
    ::memset(buf, 'a', 248);
    buf[248] = 'b';
    buf[249] = '\0'; // Ensure explicit null-termination

    StringCStyle::SString S;
    EXPECT_TRUE(StringCStyle::StrAssign(S, buf));
    EXPECT_EQ(S.length, 249);
}

TEST(StringInitTest, FillNInitialization) {
    char buf[250];
    std::fill_n(buf, 248, 'a');
    buf[248] = 'b';
    buf[249] = '\0'; // Fix: explicitly append null terminator to avoid strlen overflow

    StringCStyle::SString S;
    EXPECT_TRUE(StringCStyle::StrAssign(S, buf));
    EXPECT_EQ(S.length, 249);
}

// 3. Performance Benchmark Test
TEST(PatternMatchingBenchmark, WorstCasePerformanceComparison) {
    char long_s[250];
    char long_t[100];

    // Initialize using ::memset
    ::memset(long_s, 'a', 248);
    long_s[248] = 'b';
    long_s[249] = '\0';

    // Initialize using std::fill_n (with proper null terminator)
    std::fill_n(long_t, 98, 'a');
    long_t[98] = 'b';
    long_t[99] = '\0';

    StringCStyle::SString S, T;
    ASSERT_TRUE(StringCStyle::StrAssign(S, long_s));
    ASSERT_TRUE(StringCStyle::StrAssign(T, long_t));

    constexpr int iterations = 100000;

    // Benchmark BF
    auto start_bf = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int pos = PatternMatchBF(S, T);
        (void)pos;
    }
    double duration_bf = std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - start_bf).count();

    // Benchmark KMP (Heap / std::vector)
    auto start_kmp_heap = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int pos = PatternMatchKMP_Heap(S, T);
        (void)pos;
    }
    double duration_kmp_heap = std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - start_kmp_heap).count();

    // Benchmark KMP (Stack Array)
    auto start_kmp_stack = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile int pos = PatternMatchKMP_Stack(S, T);
        (void)pos;
    }
    double duration_kmp_stack = std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now() - start_kmp_stack).count();

    RecordProperty("BF_Time_ms", duration_bf);
    RecordProperty("KMP_Heap_Time_ms", duration_kmp_heap);
    RecordProperty("KMP_Stack_Time_ms", duration_kmp_stack);

    std::cout << "\n[ Benchmark Results (" << iterations << " iterations) ]\n"
              << "  - Brute Force (BF)        : " << duration_bf << " ms\n"
              << "  - KMP (Heap / std::vector): " << duration_kmp_heap << " ms\n"
              << "  - KMP (Stack Allocation)  : " << duration_kmp_stack << " ms\n"
              << "  - Stack vs Heap Speedup   : " << (duration_kmp_heap / duration_kmp_stack) << "x\n\n";

    // KMP (Stack) must be significantly faster than both BF and Heap-allocated KMP
    EXPECT_LT(duration_kmp_stack, duration_kmp_heap);
    EXPECT_LT(duration_kmp_stack, duration_bf);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}