#ifndef LINEAR_LIST_ARRAY_H
#define LINEAR_LIST_ARRAY_H

#include <iostream>

// Define maxsize of capacity.
#define MaxSize 128

// Reserved element type, using int as a default example. 
// Can be changed to other types in practical use.
using ElemType = int;

// Structure definition for statically allocated sequential list
typedef struct {
    ElemType data[MaxSize]; // Static array to store elements
    int length;             // Current length of the list
} SqList;

/**
 * @brief Initializes the static sequential list
 * @param L Reference to the sequential list
 */
inline void InitList(SqList &L) {
    // Simply clear the length -> zero, 
    // the array memory will be managed by the compiler.
    L.length = 0; 
}

/**
 * @brief Inserts an element at a specific position
 * @param L Reference to the sequential list
 * @param i Insertion position (Note: 1-indexed, 1 <= i <= L.length + 1)
 * @param e The element value to be inserted
 * @return True if insertion is successful, false otherwise
 */
inline bool ListInsert(SqList &L, int i, ElemType e) {
    // Check if the position of i is valid (1 <= i <= L.length + 1)
    if (i < 1 || i > L.length + 1) return false;
    // Check if the storage space is full
    if (L.length >= MaxSize) return false;
    // Shift all elements at position i and beyond one position to the right
    for (int j = L.length; j >= i; j--) {
        L.data[j] = L.data[j-1];
    }
    L.data[i-1] = e; // Insert e in position i
    L.length++;      // Increase L.length by 1
    return true;    
}

/**
 * @brief Deletes an element at a specific position and returns the deleted value
 * @param L Reference to the sequential list
 * @param i Deletion position (1-indexed, 1 <= i <= L.length)
 * @param e Reference to a variable to save the deleted element
 * @return True if deletion is successful, false otherwise
 */
inline bool ListDelete(SqList &L, int i, ElemType &e) {
    // Check if the position i is valid
    if (i < 1 || i > L.length) return false;
    e = L.data[i-1]; // Retrieve the element to be deleted
    // Shift elements after position i to the left by one position
    for (int j = i; j < L.length; j++) {
        L.data[j-1] = L.data[j];
    }
    L.length--; // Decrease the list length by 1
    return true;
}

/**
 * @brief Finds an element by its value
 * @param L The sequential list
 * @param e The target value to search for
 * @return The 1-based index (position) of the target element, or 0 if not found
 */
inline int LocateElem(SqList L, ElemType e) {
    int i;
    for (i = 0; i < L.length; i++) {
        if (L.data[i] == e) return i+1; // Return the 1-based position (index + 1)
    }
    return 0; // Search failed
}

/**
 * @brief Prints all elements of the sequential list (for debugging purposes)
 * @param L The sequential list
 */
inline void PrintList(SqList L) {
    std::cout << "List (length=" << L.length << "): [ ";
    for (int i = 0; i < L.length; i++) {
        std::cout << L.data[i] << " ";
    }
    std::cout << "]" << std::endl;
}

#endif // LINEAR_LIST_ARRAY_H