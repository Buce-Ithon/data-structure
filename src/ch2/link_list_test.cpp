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

/**
 * @brief Application Q6: Split list C = {a1, b1, a2, b2, ..., an, bn} in-place into two lists:
 *        A = {a1, a2, ..., an} and B = {b1, b2, ..., bn}.
 *        Reuses C's head node as list A's head, and creates/initializes list B's head node.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param C Pointer to the dummy head node of list C (becomes list A after execution)
 * @param B Reference to the pointer to list B's head node (to be assigned a dummy head node)
 */
void SplitList(LinkList C, LinkList &B) {
    // 1. Check if the list is empty or has fewer than 2 elements
    if (C == nullptr || C->next == nullptr) {
        B = new LNode(); // Create empty head node for B
        return;
    }

    // 2. Initialize head node for list B
    B = new LNode();

    // 3. Initialize tail pointers for both list A (reusing C) and list B
    LNode *ra = C; // Tail pointer for list A
    LNode *rb = B; // Tail pointer for list B

    // 4. Traverse list C and alternately append nodes to A and B
    LNode *p = C->next; // Current node to process
    while (p != nullptr) {
        // Append a_i to list A
        ra->next = p;
        ra = p;
        p = p->next;

        // If b_i exists, append b_i to list B
        if (p != nullptr) {
            rb->next = p;
            rb = p;
            p = p->next;
        }
    }

    // 5. Terminate both lists with nullptr
    ra->next = nullptr;
    rb->next = nullptr;
}

/**
 * @brief Application Q7: Delete duplicate elements from an ascending sorted singly linked list with a dummy head node.
 *        Frees the memory allocated for each deleted duplicate node.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 */
void RemoveDuplicates(LinkList L) {
    // 1. Check if the list is empty or has only one element
    if (L == nullptr || L->next == nullptr) {
        return;
    }

    // 2. Initialize pointer p to the first data node
    LNode *p = L->next;

    // 3. Traverse the sorted list and remove duplicates
    while (p->next != nullptr) {
        // Compare current node's value with next node's value
        if (p->data == p->next->data) {
            LNode *duplicate = p->next; // Store node to be deleted
            p->next = duplicate->next;  // Bypass duplicate node
            delete duplicate;            // Free memory allocated for duplicate node
        } else {
            p = p->next;                // Move forward if no duplicate
        }
    }
}

/**
 * @brief Application Q8: Extract common elements from two ascending sorted singly linked lists A and B
 *        and construct a new list C containing these elements without altering original lists A and B.
 *        Time Complexity: O(m + n), Space Complexity: O(k) where k is the number of common elements.
 * @param A Pointer to the dummy head node of sorted list A
 * @param B Pointer to the dummy head node of sorted list B
 * @param C Reference to the pointer of the new dummy head node for list C
 */
void ExtractCommonElements(LinkList A, LinkList B, LinkList &C) {
    // 1. Initialize head node and tail pointer for the result list C
    C = new LNode();
    LNode *rc = C; // Tail pointer for list C

    // 2. Initialize pointers to the first data nodes of lists A and B
    LNode *pA = (A != nullptr) ? A->next : nullptr;
    LNode *pB = (B != nullptr) ? B->next : nullptr;

    // 3. Traverse both lists simultaneously using two-pointer approach
    while (pA != nullptr && pB != nullptr) {
        if (pA->data < pB->data) {
            pA = pA->next; // Advance A's pointer if A's element is smaller
        } else if (pA->data > pB->data) {
            pB = pB->next; // Advance B's pointer if B's element is smaller
        } else {
            // Found a common element: create a new node and append to list C
            LNode *newNode = new LNode();
            newNode->data = pA->data;
            rc->next = newNode;
            rc = newNode;

            // Advance both pointers
            pA = pA->next;
            pB = pB->next;
        }
    }

    // 4. Terminate list C with nullptr
    rc->next = nullptr;
}

/**
 * @brief Application Q10: Determine whether sequence B is a continuous subsequence of sequence A.
 *        Time Complexity: O(m * n) worst-case, Space Complexity: O(1)
 * @param A Pointer to the dummy head node of list A
 * @param B Pointer to the dummy head node of list B
 * @return true if B is a continuous subsequence of A or B is empty; false otherwise
 */
bool IsSubsequence(LinkList A, LinkList B) {
    // 1. Handle edge cases: Empty B is always a subsequence; empty A with non-empty B is not
    if (B == nullptr || B->next == nullptr) {
        return true;
    }
    if (A == nullptr || A->next == nullptr) {
        return false;
    }

    // 2. Initialize pointers for traversal
    LNode *pA = A->next; // Outer loop pointer tracking the start matching position in A
    LNode *pB = B->next; // Pointer tracking sequence B
    LNode *pMatch = nullptr; // Temporary pointer tracking sequence A during match check

    // 3. Traverse list A to find matching sub-segments
    while (pA != nullptr) {
        pMatch = pA;
        pB = B->next;

        // Compare elements sequentially while values match
        while (pMatch != nullptr && pB != nullptr && pMatch->data == pB->data) {
            pMatch = pMatch->next;
            pB = pB->next;
        }

        // If pB reaches the end, all elements of B matched continuously
        if (pB == nullptr) {
            return true;
        }

        // Advance starting pointer in list A for next alignment attempt
        pA = pA->next;
    }

    // 4. No matching continuous subsequence found
    return false;
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

    // Application Q6
    std::cout << "===== Application Q6 =====" << std::endl;

    SinglyLinkedList listC;
    LinkList C = listC.GetHead();

    // Insert sample elements: C = { a1, b1, a2, b2, a3, b3 } = [ 1, 10, 2, 20, 3, 30 ]
    listC.ListInsert(C, 1, 1);
    listC.ListInsert(C, 2, 10);
    listC.ListInsert(C, 3, 2);
    listC.ListInsert(C, 4, 20);
    listC.ListInsert(C, 5, 3);
    listC.ListInsert(C, 6, 30);

    std::cout << "Initial list C: ";
    listC.PrintList(); // Expected output: [ 1 10 2 20 3 30 ]

    LinkList B = nullptr;
    SplitList(C, B);

    std::cout << "List A: ";
    listC.PrintList(); // Expected output: [ 1 2 3 ]

    // Print list B using auxiliary traversal
    std::cout << "List B: [ ";
    LNode *pB = B->next;
    while (pB != nullptr) {
        std::cout << pB->data << " ";
        pB = pB->next;
    }
    std::cout << "]" << std::endl; // Expected output: [ 10 20 30 ]

    // Application Q7
    std::cout << "===== Application Q7 =====" << std::endl;

    SinglyLinkedList list7;
    LinkList L7 = list7.GetHead();

    // Insert sample elements: [ 7, 10, 10, 21, 30, 42, 42, 42, 51, 70 ]
    list7.ListInsert(L7, 1, 7);
    list7.ListInsert(L7, 2, 10);
    list7.ListInsert(L7, 3, 10);
    list7.ListInsert(L7, 4, 21);
    list7.ListInsert(L7, 5, 30);
    list7.ListInsert(L7, 6, 42);
    list7.ListInsert(L7, 7, 42);
    list7.ListInsert(L7, 8, 42);
    list7.ListInsert(L7, 9, 51);
    list7.ListInsert(L7, 10, 70);

    std::cout << "Initial list: ";
    list7.PrintList(); // Expected output: [ 7 10 10 21 30 42 42 42 51 70 ]

    RemoveDuplicates(L7);

    std::cout << "After removing duplicates: ";
    list7.PrintList(); // Expected output: [ 7 10 21 30 42 51 70 ]

    // Application Q8
    std::cout << "===== Application Q8 =====" << std::endl;

    SinglyLinkedList listA8, listB8;
    LinkList A8 = listA8.GetHead();
    LinkList B8 = listB8.GetHead();

    // Insert sorted elements into List A: [ 2, 5, 8, 12, 20, 25 ]
    listA8.ListInsert(A8, 1, 2);
    listA8.ListInsert(A8, 2, 5);
    listA8.ListInsert(A8, 3, 8);
    listA8.ListInsert(A8, 4, 12);
    listA8.ListInsert(A8, 5, 20);
    listA8.ListInsert(A8, 6, 25);

    // Insert sorted elements into List B: [ 1, 5, 10, 12, 25, 30 ]
    listB8.ListInsert(B8, 1, 1);
    listB8.ListInsert(B8, 2, 5);
    listB8.ListInsert(B8, 3, 10);
    listB8.ListInsert(B8, 4, 12);
    listB8.ListInsert(B8, 5, 25);
    listB8.ListInsert(B8, 6, 30);

    std::cout << "List A8: ";
    listA8.PrintList(); // Expected output: [ 2 5 8 12 20 25 ]

    std::cout << "List B8: ";
    listB8.PrintList(); // Expected output: [ 1 5 10 12 25 30 ]

    LinkList C8 = nullptr;
    ExtractCommonElements(A8, B8, C8);

    // Print common elements list C8
    std::cout << "Common elements list C8: [ ";
    LNode *pC8 = C8->next;
    while (pC8 != nullptr) {
        std::cout << pC8->data << " ";
        pC8 = pC8->next;
    }
    std::cout << "]" << std::endl; // Expected output: [ 5 12 25 ]

    // Verify original lists A8 and B8 are intact
    std::cout << "List A8 intact check: ";
    listA8.PrintList(); // Expected output: [ 2 5 8 12 20 25 ]

    std::cout << "List B8 intact check: ";
    listB8.PrintList(); // Expected output: [ 1 5 10 12 25 30 ]

    // Application Q10
    std::cout << "===== Application Q10 =====" << std::endl;

    SinglyLinkedList listA, listB1, listB2;
    LinkList A = listA.GetHead();
    LinkList B1 = listB1.GetHead();
    LinkList B2 = listB2.GetHead();

    // Setup List A: [ 1, 2, 3, 4, 5, 6 ]
    listA.ListInsert(A, 1, 1);
    listA.ListInsert(A, 2, 2);
    listA.ListInsert(A, 3, 3);
    listA.ListInsert(A, 4, 4);
    listA.ListInsert(A, 5, 5);
    listA.ListInsert(A, 6, 6);

    // Setup List B1 (valid continuous subsequence): [ 3, 4, 5 ]
    listB1.ListInsert(B1, 1, 3);
    listB1.ListInsert(B1, 2, 4);
    listB1.ListInsert(B1, 3, 5);

    // Setup List B2 (invalid subsequence): [ 3, 5, 6 ]
    listB2.ListInsert(B2, 1, 3);
    listB2.ListInsert(B2, 2, 5);
    listB2.ListInsert(B2, 3, 6);

    std::cout << "Sequence A: ";
    listA.PrintList(); // Expected output: [ 1 2 3 4 5 6 ]

    std::cout << "Sequence B1: ";
    listB1.PrintList(); // Expected output: [ 3 4 5 ]

    std::cout << "Is B1 a continuous subsequence of A? "
              << (IsSubsequence(A, B1) ? "Yes" : "No") << std::endl; // Expected: Yes

    std::cout << "Sequence B2: ";
    listB2.PrintList(); // Expected output: [ 3 5 6 ]

    std::cout << "Is B2 a continuous subsequence of A? "
              << (IsSubsequence(A, B2) ? "Yes" : "No") << std::endl; // Expected: No

    return 0;
}