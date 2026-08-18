#ifndef STRING_C_H
#define STRING_C_H

namespace StringCStyle {
    // Replace the original `#define MAXLEN 256` to prevent macros from polluting the global scope.
    constexpr int MAXLEN = 255;
    
    // ==================== 1. Data Structure Definitions ====================
    
    // Sequential-storage string
    typedef struct {
        char ch[MAXLEN];
        int length;
    } SString;
    
    // Heap-storage string
    typedef struct {
        char *ch; // Allocate storage according to string length, ch points to the base address of the string
        int length;
    } HString;
    
    // ==================== 2. Basic Operations for Sequential-storage String ====================
    
    // Assign chars to string
    bool StrAssign(SString &T, const char *chars);

    // Copy string to another
    bool StrCopy(SString &T, const SString &S);

    // Check if string is empty
    bool StrEmpty(const SString &S);

    // Compare 2 strings
    int StrCompare(const SString &S, const SString &T);
    
    // Get string length
    int StrLength(const SString &S);

    // Get substring
    bool SubString(SString &Sub, const SString &S, int pos, int len);

    // Concatenate 2 strings
    bool Concat(SString &T, const SString &S1, const SString &S2);

    // Get index of substring
    int Index(const SString &S, const SString &T, int pos = 1);
    
    // Clear string to an empty string
    bool ClearString(SString &S);
    
    // Destroy string
    bool DestroyString(SString &S);

    // ==================== 3. Basic Operations for Heap-storage String ====================
    
    // Assign chars to string
    bool StrAssign(HString &T, const char *chars);

    // Copy string to another
    bool StrCopy(HString &T, const HString &S);

    // Check if string is empty
    bool StrEmpty(const HString &S);

    // Compare 2 strings
    int StrCompare(const HString &S, const HString &T);
    
    // Get string length
    int StrLength(const HString &S);

    // Get substring
    bool SubString(HString &Sub, const HString &S, int pos, int len);

    // Concatenate 2 strings
    bool Concat(HString &T, const HString &S1, const HString &S2);

    // Get index of substring
    int Index(const HString &S, const HString &T, int pos = 1);
    
    // Clear string to an empty string
    bool ClearString(HString &S);
    
    // Destroy string
    bool DestroyString(HString &S);
}

#endif // STRING_C_H