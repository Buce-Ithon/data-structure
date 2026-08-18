#include "string_cpp_1.h"
#include <cstring>
#include <algorithm>

namespace ModernString1 {

    String::String() noexcept : m_length(0), m_capacity(SSO_CAPACITY) {
        m_storage.sso_buf[0] = '\0';
    }

    String::String(const char* str) : String(str ? std::string_view(str) : std::string_view()) {}

    String::String(std::string_view sv) {
        m_length = sv.length();
        if (m_length <= SSO_CAPACITY) {
            m_capacity = SSO_CAPACITY;
            std::memcpy(m_storage.sso_buf, sv.data(), m_length);
            m_storage.sso_buf[m_length] = '\0';
        } else {
            m_capacity = m_length;
            m_storage.heap.ptr = new char[m_capacity + 1];
            m_storage.heap.cap = m_capacity;
            std::memcpy(m_storage.heap.ptr, sv.data(), m_length);
            m_storage.heap.ptr[m_length] = '\0';
        }
    }

    String::String(const String& other) : String(std::string_view(other)) {}

    String::String(String&& other) noexcept {
        m_length = other.m_length;
        m_capacity = other.m_capacity;
        if (other.is_sso()) {
            std::memcpy(m_storage.sso_buf, other.m_storage.sso_buf, m_length + 1);
        } else {
            m_storage.heap = other.m_storage.heap;
            other.m_capacity = SSO_CAPACITY;
            other.m_storage.sso_buf[0] = '\0';
        }
        other.m_length = 0;
    }

    String::~String() {
        if (!is_sso()) {
            delete[] m_storage.heap.ptr;
        }
    }

    String& String::operator=(const String& other) {
        if (this != &other) {
            *this = std::string_view(other);
        }
        return *this;
    }

    String& String::operator=(String&& other) noexcept {
        if (this != &other) {
            if (!is_sso()) {
                delete[] m_storage.heap.ptr;
            }
            m_length = other.m_length;
            m_capacity = other.m_capacity;
            if (other.is_sso()) {
                std::memcpy(m_storage.sso_buf, other.m_storage.sso_buf, m_length + 1);
            } else {
                m_storage.heap = other.m_storage.heap;
                other.m_capacity = SSO_CAPACITY;
                other.m_storage.sso_buf[0] = '\0';
            }
            other.m_length = 0;
        }
        return *this;
    }

    String& String::operator=(std::string_view sv) {
        if (sv.length() <= m_capacity) {
            m_length = sv.length();
            std::memcpy(const_cast<char*>(data()), sv.data(), m_length);
            const_cast<char*>(data())[m_length] = '\0';
        } else {
            if (!is_sso()) {
                delete[] m_storage.heap.ptr;
            }
            m_length = sv.length();
            m_capacity = m_length;
            m_storage.heap.ptr = new char[m_capacity + 1];
            m_storage.heap.cap = m_capacity;
            std::memcpy(m_storage.heap.ptr, sv.data(), m_length);
            m_storage.heap.ptr[m_length] = '\0';
        }
        return *this;
    }

    std::size_t String::length() const noexcept { return m_length; }
    std::size_t String::capacity() const noexcept { return m_capacity; }
    bool String::empty() const noexcept { return m_length == 0; }

    const char* String::data() const noexcept {
        return is_sso() ? m_storage.sso_buf : m_storage.heap.ptr;
    }

    char& String::operator[](std::size_t index) noexcept {
        return const_cast<char*>(data())[index];
    }

    const char& String::operator[](std::size_t index) const noexcept {
        return data()[index];
    }

    String String::substr(std::size_t pos, std::size_t len) const {
        if (pos >= m_length) return String();
        std::size_t actual_len = std::min(len, m_length - pos);
        return String(std::string_view(data() + pos, actual_len));
    }

    std::size_t String::find(std::string_view target, std::size_t pos) const noexcept {
        std::string_view current(data(), m_length);
        return current.find(target, pos);
    }

    String& String::operator+=(std::string_view sv) {
        std::size_t new_len = m_length + sv.length();
        if (new_len <= m_capacity) {
            std::memcpy(const_cast<char*>(data()) + m_length, sv.data(), sv.length());
            m_length = new_len;
            const_cast<char*>(data())[m_length] = '\0';
        } else {
            std::size_t new_cap = std::max(m_capacity * 2, new_len);
            char* new_ptr = new char[new_cap + 1];
            std::memcpy(new_ptr, data(), m_length);
            std::memcpy(new_ptr + m_length, sv.data(), sv.length());
            new_ptr[new_len] = '\0';

            if (!is_sso()) {
                delete[] m_storage.heap.ptr;
            }
            m_storage.heap.ptr = new_ptr;
            m_storage.heap.cap = new_cap;
            m_capacity = new_cap;
            m_length = new_len;
        }
        return *this;
    }

    String operator+(const String& lhs, std::string_view rhs) {
        String result = lhs;
        result += rhs;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const String& str) {
        return os << std::string_view(str);
    }
}