#include "linear_list.h"
#include <iostream>

void InitList(SqList &L) {
    // Simply clear the length -> zero, 
    // the array memory will be managed by the compiler.
    L.length = 0; 
}

bool ListInsert(SqList &L, int i, ElemType e) {
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

bool ListDelete(SqList &L, int i, ElemType &e) {
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

int LocateElem(SqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) return i + 1; // Return the 1-based position (index + 1)
    }
    return 0; // Search failed
}

void PrintList(SqList L) {
    std::cout << "List (length=" << L.length << "): [ ";
    for (int i = 0; i < L.length; i++) {
        std::cout << L.data[i] << " ";
    }
    std::cout << "]" << std::endl;
}