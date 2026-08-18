#include "string_c.h"
#include <new>

namespace StringCStyle {

    // ==================== 2. Basic Operations for Sequential-storage String ====================

    // Assign chars to string
    bool StrAssign(SString &T, const char *chars) {
        if (!chars) return false;
        int len = 0;
        while (chars[len] != '\0') {
            len++;
        }
        if (len > MAXLEN) return false;

        for (int i = 0; i < len; ++i) {
            T.ch[i] = chars[i];
        }
        T.length = len;
        return true;
    }

    // Copy string to another
    bool StrCopy(SString &T, const SString &S) {
        for (int i = 0; i < S.length; ++i) {
            T.ch[i] = S.ch[i];
        }
        T.length = S.length;
        return true;
    }

    // Check if string is empty
    bool StrEmpty(const SString &S) {
        return S.length == 0;
    }

    // Compare 2 strings
    int StrCompare(const SString &S, const SString &T) {
        for (int i = 0; i < S.length && i < T.length; ++i) {
            if (S.ch[i] != T.ch[i]) {
                return S.ch[i] - T.ch[i];
            }
        }
        return S.length - T.length;
    }

    // Get string length
    int StrLength(const SString &S) {
        return S.length;
    }

    // Get substring
    bool SubString(SString &Sub, const SString &S, int pos, int len) {
        if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length) {
            return false;
        }
        for (int i = 0; i < len; ++i) {
            Sub.ch[i] = S.ch[pos - 1 + i];
        }
        Sub.length = len;
        return true;
    }

    // Concatenate 2 strings
    bool Concat(SString &T, const SString &S1, const SString &S2) {
        if (S1.length + S2.length > MAXLEN) {
            // Truncate S2 if total length exceeds MAXLEN
            int i = 0;
            for (; i < S1.length && i < MAXLEN; ++i) {
                T.ch[i] = S1.ch[i];
            }
            for (int j = 0; j < S2.length && i < MAXLEN; ++j, ++i) {
                T.ch[i] = S2.ch[j];
            }
            T.length = i;
            return false; // Return false indicating truncation occurred
        } else {
            for (int i = 0; i < S1.length; ++i) {
                T.ch[i] = S1.ch[i];
            }
            for (int i = 0; i < S2.length; ++i) {
                T.ch[S1.length + i] = S2.ch[i];
            }
            T.length = S1.length + S2.length;
            return true;
        }
    }

    // Get index of substring (BF pattern matching)
    int Index(const SString &S, const SString &T, int pos) {
        if (pos < 1 || pos > S.length || T.length == 0) return 0;
        int i = pos - 1; // 0-based index for S
        int j = 0;       // 0-based index for T
        while (i < S.length && j < T.length) {
            if (S.ch[i] == T.ch[j]) {
                ++i;
                ++j;
            } else {
                i = i - j + 1; // Backtrack
                j = 0;
            }
        }
        if (j >= T.length) {
            return i - T.length + 1; // Return 1-based index
        }
        return 0;
    }

    // Clear string to an empty string
    bool ClearString(SString &S) {
        S.length = 0;
        return true;
    }

    // Destroy string
    bool DestroyString(SString &S) {
        S.length = 0;
        return true;
    }

    // ==================== 3. Basic Operations for Heap-storage String ====================

    // Assign chars to string
    bool StrAssign(HString &T, const char *chars) {
        if (T.ch) {
            delete[] T.ch;
            T.ch = nullptr;
        }
        if (!chars) {
            T.length = 0;
            return true;
        }
        int len = 0;
        while (chars[len] != '\0') {
            len++;
        }
        if (len == 0) {
            T.ch = nullptr;
            T.length = 0;
            return true;
        }
        T.ch = new (std::nothrow) char[static_cast<std::size_t>(len)];
        if (!T.ch) {
            T.length = 0;
            return false;
        }
        for (int i = 0; i < len; ++i) {
            T.ch[i] = chars[i];
        }
        T.length = len;
        return true;
    }

    // Copy string to another
    bool StrCopy(HString &T, const HString &S) {
        if (&T == &S) return true; // Self-copy check
        if (T.ch) {
            delete[] T.ch;
            T.ch = nullptr;
        }
        if (S.length == 0 || !S.ch) {
            T.length = 0;
            return true;
        }
        T.ch = new (std::nothrow) char[static_cast<std::size_t>(S.length)];
        if (!T.ch) {
            T.length = 0;
            return false;
        }
        for (int i = 0; i < S.length; ++i) {
            T.ch[i] = S.ch[i];
        }
        T.length = S.length;
        return true;
    }

    // Check if string is empty
    bool StrEmpty(const HString &S) {
        return S.length == 0;
    }

    // Compare 2 strings
    int StrCompare(const HString &S, const HString &T) {
        for (int i = 0; i < S.length && i < T.length; ++i) {
            if (S.ch[i] != T.ch[i]) {
                return S.ch[i] - T.ch[i];
            }
        }
        return S.length - T.length;
    }

    // Get string length
    int StrLength(const HString &S) {
        return S.length;
    }

    // Get substring
    bool SubString(HString &Sub, const HString &S, int pos, int len) {
        if (Sub.ch) {
            delete[] Sub.ch;
            Sub.ch = nullptr;
        }
        if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length) {
            Sub.length = 0;
            return false;
        }
        if (len == 0) {
            Sub.length = 0;
            return true;
        }
        Sub.ch = new (std::nothrow) char[static_cast<std::size_t>(len)];
        if (!Sub.ch) {
            Sub.length = 0;
            return false;
        }
        for (int i = 0; i < len; ++i) {
            Sub.ch[i] = S.ch[pos - 1 + i];
        }
        Sub.length = len;
        return true;
    }

    // Concatenate 2 strings
    bool Concat(HString &T, const HString &S1, const HString &S2) {
        if (T.ch) {
            delete[] T.ch;
            T.ch = nullptr;
        }
        int totalLen = S1.length + S2.length;
        if (totalLen == 0) {
            T.length = 0;
            return true;
        }
        T.ch = new (std::nothrow) char[static_cast<std::size_t>(totalLen)];
        if (!T.ch) {
            T.length = 0;
            return false;
        }
        for (int i = 0; i < S1.length; ++i) {
            T.ch[i] = S1.ch[i];
        }
        for (int i = 0; i < S2.length; ++i) {
            T.ch[S1.length + i] = S2.ch[i];
        }
        T.length = totalLen;
        return true;
    }

    // Get index of substring (BF pattern matching)
    int Index(const HString &S, const HString &T, int pos) {
        if (pos < 1 || pos > S.length || T.length == 0) return 0;
        int i = pos - 1; // 0-based index for S
        int j = 0;       // 0-based index for T
        while (i < S.length && j < T.length) {
            if (S.ch[i] == T.ch[j]) {
                ++i;
                ++j;
            } else {
                i = i - j + 1; // Backtrack
                j = 0;
            }
        }
        if (j >= T.length) {
            return i - T.length + 1; // Return 1-based index
        }
        return 0;
    }

    // Clear string to an empty string
    bool ClearString(HString &S) {
        if (S.ch) {
            delete[] S.ch;
            S.ch = nullptr;
        }
        S.length = 0;
        return true;
    }

    // Destroy string
    bool DestroyString(HString &S) {
        return ClearString(S);
    }
}