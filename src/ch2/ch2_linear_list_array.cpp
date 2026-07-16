#include <iostream>
#include <linear_list_array.h>

int main() {
    std::cout << "Hello, linear list!" << "\n";

    SqList L;
    InitList(L); // Initialization

    // Insertion tests
    ListInsert(L, 1, 10);
    ListInsert(L, 2, 20);
    ListInsert(L, 1, 5); // Insert 5 at the very front
    ListInsert(L, 1, 1);
    PrintList(L);        // Expected output: [ 1 5 10 20 ]
    // Deletion test
    int deletedVal;
    if (ListDelete(L, 2, deletedVal)) {
        std::cout << "Successfully deleted: " << deletedVal << std::endl;
    }
    PrintList(L);        // Expected output: [ 1 5 20 ]

    return 0;
}