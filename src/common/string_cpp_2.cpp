#include "string_cpp_2.h"

namespace ModernString2 {

    DynamicString::DynamicString(std::string_view sv) : m_data(nullptr), m_length(0) {
        assign(sv);
    }

    DynamicString::DynamicString(const DynamicString& other) : DynamicString(std::string_view(other)) {}

    DynamicString::DynamicString(DynamicString&& other) noexcept
        : m_data(other.m_data), m_length(other.m_length) {
        other.m_data = nullptr;
        other.m_length = 0;
    }

    DynamicString::~DynamicString() {
        delete[] m_data;
    }

    DynamicString& DynamicString::operator=(const DynamicString& other) {
        if (this != &other) {
            assign(std::string_view(other));
        }
        return *this;
    }

    DynamicString& DynamicString::operator=(DynamicString&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_length = other.m_length;
            other.m_data = nullptr;
            other.m_length = 0;
        }
        return *this;
    }

    bool DynamicString::assign(std::string_view sv) {
        delete[] m_data;
        m_length = sv.length();
        if (m_length == 0) {
            m_data = nullptr;
            return true;
        }
        m_data = new char[m_length + 1];
        std::memcpy(m_data, sv.data(), m_length);
        m_data[m_length] = '\0';
        return true;
    }
}