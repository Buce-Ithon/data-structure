#ifndef STRING_CPP_1_H
#define STRING_CPP_1_H

#include <cstddef>
#include <iostream>
#include <string_view>

// A unified String class based on SSO (Small String Optimization).
// This solution integrates static storage with dynamic allocation: 
// When the string length is less than 15 bytes, the data is directly stored in the stack buffer (extremely high performance); 
// when it exceeds 15 bytes, it automatically switches to heap memory. Users do not need to perceive the difference in physical storage.

namespace ModernString1 {

    class String {
    public:
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);
        static constexpr std::size_t SSO_CAPACITY = 15;

        // 1. Constructors & Destructor (RAII)
        String() noexcept;
        String(const char* str);
        String(std::string_view sv);
        String(const String& other);
        String(String&& other) noexcept;
        ~String();

        // 2. Assignment Operators
        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;
        String& operator=(std::string_view sv);

        // 3. Capacity & Element Access
        [[nodiscard]] std::size_t length() const noexcept;
        [[nodiscard]] std::size_t capacity() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] const char* data() const noexcept;

        char& operator[](std::size_t index) noexcept;
        const char& operator[](std::size_t index) const noexcept;

        // 4. Basic String Operations
        [[nodiscard]] String substr(std::size_t pos = 0, std::size_t len = npos) const;
        [[nodiscard]] std::size_t find(std::string_view target, std::size_t pos = 0) const noexcept;

        String& operator+=(std::string_view sv);
        friend String operator+(const String& lhs, std::string_view rhs);

        // 5. Comparisons (C++20 Three-way comparison)
        auto operator<=>(const String& other) const noexcept {
            return std::string_view(*this) <=> std::string_view(other);
        }
        bool operator==(const String& other) const noexcept {
            return std::string_view(*this) == std::string_view(other);
        }

        operator std::string_view() const noexcept {
            return std::string_view(data(), length());
        }

    private:
        bool is_sso() const noexcept { return m_capacity <= SSO_CAPACITY; }

        struct LongBuffer {
            char* ptr;
            std::size_t cap;
        };

        union Storage {
            char sso_buf[SSO_CAPACITY + 1];
            LongBuffer heap;
        } m_storage;

        std::size_t m_length;
        std::size_t m_capacity; // 若 <= SSO_CAPACITY 则使用 sso_buf
    };

    std::ostream& operator<<(std::ostream& os, const String& str);
}

#endif // STRING_CPP_1_H