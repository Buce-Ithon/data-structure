#include <iostream>
#include <link_list.h>

// Alias common definition for readability
using LinkList = SinglyLinkedList::LinkList;
using LNode    = SinglyLinkedList::LNode;

/**
 * @brief Application Q1: Delete all nodes with value x from a singly linked list with a head node.
 *        Frees the memory allocated for each deleted node.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the head node of the singly linked list
 * @param x The target value to be deleted
 */
void DeleteAllX(LinkList L, ElemType x) {
    // 1. Check if the list is empty (only contains head node)
    if (L == nullptr || L->next == nullptr) {
        return;
    }

    // 2. Initialize pointers: 'pre' tracks the node before 'p'
    LNode *pre = L;
    LNode *p = L->next;

    // 3. Traverse through the linked list
    while (p != nullptr) {
        if (p->data == x) {
            // Target node found: update pre's next pointer and free p
            LNode *temp = p;
            pre->next = p->next;
            p = p->next;
            delete temp; // Free memory allocated for deleted node
        } else {
            // Target node not found: advance both pointers
            pre = p;
            p = p->next;
        }
    }
}

int main() {
    std::cout << "Hello, world~" << std::endl;
    
    // Application Q1
    std::cout << "===== Application Q1 =====" << std::endl;

    SinglyLinkedList list;
    LinkList L = list.GetHead();

    // Insert sample elements: [ 10, 25, 20, 25, 30, 25 ]
    list.ListInsert(L, 1, 10);
    list.ListInsert(L, 2, 25);
    list.ListInsert(L, 3, 20);
    list.ListInsert(L, 4, 25);
    list.ListInsert(L, 5, 30);
    list.ListInsert(L, 6, 25);

    std::cout << "Initial list: ";
    list.PrintList(); // Expected output: [ 10 25 20 25 30 25 ]

    ElemType targetX = 25;
    DeleteAllX(L, targetX);

    std::cout << "After deleting all x = " << targetX << ": ";
    list.PrintList(); // Expected output: [ 10 20 30 ]

    return 0;
}