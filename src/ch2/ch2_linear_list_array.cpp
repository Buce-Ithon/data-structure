#include <iostream>
#include <linear_list_array.h>
#include <ostream>

/**
 * @brief Application Q1: Delete the element with the minimum value and fill its gap with the last element.
 * @param L Reference to the sequential list
 * @param value Reference to a variable to store the deleted minimum value
 * @return True if successful, false otherwise
 */
bool Del_Min(SqList &L, ElemType &value){
    // 1. Check if the list is empty
    if (L.length == 0) {
        std::cerr << "Error: The sequential list is empty!" << std::endl;
        exit(EXIT_FAILURE); // Display error and terminate execution
    }
    // 2. Assume the first element is the minimum initially
    int minIndex = 0;
    value = L.data[0];
    // 3. Loop through the list to find the actual minimum element
    for (int i = 1; i < L.length; i++) {
        if (L.data[i] < value) {
            value = L.data[i];
            minIndex = i;
        }
    }
    // 4. Fill the gap with the last element of the list
    L.data[minIndex] = L.data[L.length - 1];
    // 5. Decrease the list length by 1
    //L.length--;

    return true;
}

/**
 * @brief Application Q2: Reverse all elements in the sequential list in-place.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Reference to the sequential list
 */
void Reverse(SqList &L) {
    if (L.length <= 1) return; // 0 or 1 element doesn't need reversal

    int left{0};
    int right{L.length - 1};
    
    while (left < right) {
        // Swap the elements in the postion left and right
        L.data[left]  = L.data[left] + L.data[right];
        L.data[right] = L.data[left] - L.data[right];
        L.data[left]  = L.data[left] - L.data[right];
        // Move pointers to the center
        left++;
        right--;
    }

}

int main() {
    std::cout << "Hello, linear list!" << "\n";

    SqList L;
    InitList(L); // Initialization

    // Insertion tests
    ListInsert(L, 1, 10);
    ListInsert(L, 2, 20);
    ListInsert(L, 1, 5); // Insert 5 at the very front
    ListInsert(L, 1, 1);
    ListInsert(L, 5, 25);
    PrintList(L);        // Expected output: [ 1 5 10 20 25 ]
    // Deletion test
    int deletedVal;
    if (ListDelete(L, 2, deletedVal)) {
        std::cout << "Successfully deleted: " << deletedVal << std::endl;
    }
    // Locate test
    int locateVal{10};
    std::cout << "Element " << locateVal << " in position: " << LocateElem(L, locateVal) << std::endl;
    PrintList(L);        // Expected output: [ 1 10 20 25 ]
    
    // Application Q1
    int del_minValue;
    Del_Min(L, del_minValue);
    std::cout << "Deleted minimal value: " << del_minValue << std::endl;
    PrintList(L);        // Expected output: [ 25 10 20 25 ]

    // Application Q2
    Reverse(L);
    std::cout << "Reversed list: ";
    PrintList(L);
    Reverse(L);
    std::cout << "Reversed list: ";
    PrintList(L);

    return 0;
}