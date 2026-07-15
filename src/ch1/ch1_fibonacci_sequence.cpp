#include <iostream>
#include <chrono>

class Fibonacci {
public:
    // 1. Recursive Approach
    static long long recursive(unsigned int n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        return recursive(n-1) + recursive(n-2);
    }    
    
    // 2. Non-recursive / Iteractive Approach
    static long long iteractive(unsigned int n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        
        long long a = 0; // F(n-2)
        long long b = 1; // F(n-1)
        long long c = 0; // F(n)
        
        for (long long i=2; i<=n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        return c;
    }
};

int main() {
    std::cout << "Hello, data structures!" << "\n";
    
    unsigned int n{40};
    // Test Recursive Approach
    auto start = std::chrono::high_resolution_clock::now();
    long long res_recur = Fibonacci::recursive(n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_recur = end - start;
    std::cout << "[Recursive Approach] F(" << n << ")=" << res_recur
              << ", duration: " << elapsed_recur << "\n";
     
    // Test Non-recursive / Iteractive Approach
    start = std::chrono::high_resolution_clock::now();
    long long res_iter = Fibonacci::iteractive(n);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_iter = end - start;
    std::cout << "[Non-recursive / Iteractive Approach] F(" << n << ")=" << res_iter
              << ", duration: " << elapsed_iter << "\n";

    return 0;
}