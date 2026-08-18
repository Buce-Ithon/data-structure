#ifndef STRING_CPP_2_H
#define STRING_CPP_2_H

#include <cstddef>
#include <string_view>
#include <concepts>
#include <algorithm>
#include <cstring>

// Static/dynamic template generic methods that preserve physical isolation
// This scheme preserves the physical isolation between StaticString<N> (fixed-length stack allocation) and DynamicString (heap allocation). 
// By defining C++20 Concepts (StringConcept) and template generic algorithms, all operations (such as searching by index, substring extraction, and comparison by StrCompare) can be used uniformly for both string types.
namespace ModernString2 {

    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    // 1. C++20 Concept: uniform requirements for both string types
    template <typename T>
    concept StringConcept = requires(const T& s) {
        { s.length() } -> std::same_as<std::size_t>;
        { s.data() } -> std::same_as<const char*>;
        { s.empty() } -> std::same_as<bool>;
        { static_cast<std::string_view>(s) } -> std::same_as<std::string_view>;
    };

    // 2. Static sequential string class template (Stack Allocation)
    template <std::size_t N = 255>
    class StaticString {
    public:
        StaticString() noexcept : m_length(0) { m_data[0] = '\0'; }
        StaticString(std::string_view sv) { assign(sv); }

        bool assign(std::string_view sv) {
            m_length = std::min(sv.length(), N);
            std::memcpy(m_data, sv.data(), m_length);
            m_data[m_length] = '\0';
            return m_length == sv.length();
        }

        [[nodiscard]] std::size_t length() const noexcept { return m_length; }
        [[nodiscard]] std::size_t capacity() const noexcept { return N; }
        [[nodiscard]] bool empty() const noexcept { return m_length == 0; }
        [[nodiscard]] const char* data() const noexcept { return m_data; }

        char& operator[](std::size_t i) noexcept { return m_data[i]; }
        const char& operator[](std::size_t i) const noexcept { return m_data[i]; }

        operator std::string_view() const noexcept {
            return std::string_view(m_data, m_length);
        }

    private:
        char m_data[N + 1];
        std::size_t m_length;
    };

    // 3. Dynamic heap-memory string class (Heap Allocation)
    class DynamicString {
    public:
        DynamicString() noexcept : m_data(nullptr), m_length(0) {}
        DynamicString(std::string_view sv);
        DynamicString(const DynamicString& other);
        DynamicString(DynamicString&& other) noexcept;
        ~DynamicString();

        DynamicString& operator=(const DynamicString& other);
        DynamicString& operator=(DynamicString&& other) noexcept;

        bool assign(std::string_view sv);

        [[nodiscard]] std::size_t length() const noexcept { return m_length; }
        [[nodiscard]] bool empty() const noexcept { return m_length == 0; }
        [[nodiscard]] const char* data() const noexcept { return m_data ? m_data : ""; }

        char& operator[](std::size_t i) noexcept { return m_data[i]; }
        const char& operator[](std::size_t i) const noexcept { return m_data[i]; }

        operator std::string_view() const noexcept {
            return std::string_view(data(), m_length);
        }

    private:
        char* m_data;
        std::size_t m_length;
    };

    // 4. Generic template functions (support both StaticString and DynamicString)

    // Generic search (Index)
    template <StringConcept S1, StringConcept S2>
    std::size_t Index(const S1& str, const S2& target, std::size_t pos = 0) noexcept {
        return std::string_view(str).find(std::string_view(target), pos);
    }

    // Generic comparison (StrCompare)
    template <StringConcept S1, StringConcept S2>
    int StrCompare(const S1& s1, const S2& s2) noexcept {
        std::string_view sv1(s1);
        std::string_view sv2(s2);
        return sv1.compare(sv2);
    }

    // Generic substring extraction (SubString)
    template <typename ResultString = DynamicString, StringConcept S>
    ResultString SubString(const S& src, std::size_t pos, std::size_t len = npos) {
        std::string_view sv(src);
        if (pos >= sv.length()) return ResultString();
        return ResultString(sv.substr(pos, len));
    }

    // Generic concatenation (Concat)
    template <typename ResultString = DynamicString, StringConcept S1, StringConcept S2>
    ResultString Concat(const S1& s1, const S2& s2) {
        std::string_view sv1(s1);
        std::string_view sv2(s2);
        
        char* buf = new char[sv1.length() + sv2.length()];
        std::memcpy(buf, sv1.data(), sv1.length());
        std::memcpy(buf + sv1.length(), sv2.data(), sv2.length());
        
        ResultString res(std::string_view(buf, sv1.length() + sv2.length()));
        delete[] buf;
        return res;
    }
}

#endif // STRING_CPP_2_H