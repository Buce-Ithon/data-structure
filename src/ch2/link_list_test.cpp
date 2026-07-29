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

/**
 * @brief Application Q2: Delete the node with the minimum value from a singly linked list with a dummy head node.
 *        Assumes that the minimum node is unique and frees its allocated memory.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 */
void DeleteMin(LinkList L) {
    // 1. Check if the list is empty (only contains the head node)
    if (L == nullptr || L->next == nullptr) {
        return;
    }

    // 2. Initialize pointers to track the minimum node and traversal state
    LNode *p = L->next;    // Current pointer traversing the list
    LNode *pre = L;        // Pointer preceding current pointer 'p'
    LNode *minP = p;       // Pointer to the node with the minimum value found so far
    LNode *minPre = pre;   // Pointer preceding 'minP'

    // 3. Traverse the list to find the minimum value node and its preceding node
    while (p != nullptr) {
        if (p->data < minP->data) {
            minP = p;
            minPre = pre;
        }
        pre = p;
        p = p->next;
    }

    // 4. Remove the minimum node from the list and free memory
    minPre->next = minP->next;
    delete minP; // Free memory allocated for the deleted node
}

/**
 * @brief Application Q3: Reverse a singly linked list with a dummy head node in-place.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 */
void ReverseList(LinkList L) {
    // 1. Check if the list is empty or has only one element
    if (L == nullptr || L->next == nullptr || L->next->next == nullptr) {
        return;
    }

    // 2. Disconnect the head node from the list; p will traverse the remaining nodes
    LNode *p = L->next;
    L->next = nullptr;

    // 3. Traverse the remaining nodes and insert each at the front (head insertion)
    while (p != nullptr) {
        LNode *nxt = p->next; // Save pointer to the next node
        p->next = L->next;    // Insert current node after head node
        L->next = p;          // Update head's next pointer
        p = nxt;              // Move to the next node
    }
}

/**
 * @brief Application Q4: Delete all nodes with values strictly between minVal and maxVal
 *        (i.e., in range (minVal, maxVal)) from an unsorted singly linked list with a head node.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 * @param minVal Lower bound of the exclusion range (exclusive)
 * @param maxVal Upper bound of the exclusion range (exclusive)
 */
void DeleteRange(LinkList L, ElemType minVal, ElemType maxVal) {
    // 1. Check if the list is empty or the range is invalid
    if (L == nullptr || L->next == nullptr || minVal >= maxVal) {
        return;
    }

    // 2. Initialize pointers: 'pre' tracks the node before 'p'
    LNode *pre = L;
    LNode *p = L->next;

    // 3. Traverse through the linked list
    while (p != nullptr) {
        // Check if current node's value falls strictly within (minVal, maxVal)
        if (p->data >= minVal && p->data <= maxVal) {
            LNode *temp = p;
            pre->next = p->next; // Bypass current node
            p = p->next;        // Move to next node
            delete temp;        // Free memory allocated for deleted node
        } else {
            pre = p;            // Advance pre pointer
            p = p->next;        // Advance current pointer
        }
    }
}

int main() {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Hello, world~ Holla, link list~" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    
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

    // Application Q2
    std::cout << "===== Application Q2 =====" << std::endl;

    SinglyLinkedList list2;
    LinkList L2 = list2.GetHead();

    // Insert sample elements: [ 25, 10, 5, 20, 30 ]
    list2.ListInsert(L2, 1, 25);
    list2.ListInsert(L2, 2, 10);
    list2.ListInsert(L2, 3, 5);
    list2.ListInsert(L2, 4, 20);
    list2.ListInsert(L2, 5, 30);

    std::cout << "Initial list: ";
    list2.PrintList(); // Expected output: [ 25 10 5 20 30 ]

    DeleteMin(L2);

    std::cout << "After deleting minimum node: ";
    list2.PrintList(); // Expected output: [ 25 10 20 30 ]

    // Application Q3
    std::cout << "===== Application Q3 =====" << std::endl;

    SinglyLinkedList list3;
    LinkList L3 = list3.GetHead();

    // Insert sample elements: [ 10, 20, 30, 40, 50 ]
    list3.ListInsert(L3, 1, 10);
    list3.ListInsert(L3, 2, 20);
    list3.ListInsert(L3, 3, 30);
    list3.ListInsert(L3, 4, 40);
    list3.ListInsert(L3, 5, 50);

    std::cout << "Initial list: ";
    list3.PrintList(); // Expected output: [ 10 20 30 40 50 ]

    ReverseList(L3);

    std::cout << "Reversed list: ";
    list3.PrintList(); // Expected output: [ 50 40 30 20 10 ]

    // Application Q4
    std::cout << "===== Application Q4 =====" << std::endl;

    SinglyLinkedList list4;
    LinkList L4 = list4.GetHead();

    // Insert sample elements: [ 10, 25, 5, 18, 30, 12, 40 ]
    list4.ListInsert(L4, 1, 10);
    list4.ListInsert(L4, 2, 25);
    list4.ListInsert(L4, 3, 5);
    list4.ListInsert(L4, 4, 18);
    list4.ListInsert(L4, 5, 30);
    list4.ListInsert(L4, 6, 12);
    list4.ListInsert(L4, 7, 40);

    std::cout << "Initial list: ";
    list4.PrintList(); // Expected output: [ 10 25 5 18 30 12 40 ]

    ElemType minVal = 10;
    ElemType maxVal = 25;
    DeleteRange(L4, minVal, maxVal);

    std::cout << "After deleting elements between [" << minVal << ", " << maxVal << "]: ";
    list4.PrintList(); // Expected output: [ 5 30 40 ]

    return 0;
}