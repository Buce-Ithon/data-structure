#include <cstddef>
#include <iostream>
#include <link_list.h>
#include <vector>
#include <cmath>

// Although using namespace is not so much "standard", but it's very sumbersome to change so much indentifers.
// And the file itself is only for this chapter exercises and this would not influents other code files, so I choose namespace method to refactor code.
using namespace LinkListStyle;

// Alias common definition for readability
using LinkList = SinglyLinkedList::LinkList;
using LNode    = SinglyLinkedList::LNode;

using DLinkList = DoublyLinkedList::DLinkList;
using DNode = DoublyLinkedList::DNode;

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

/**
 * @brief Application Q11: Determine whether a doubly circular linked list with a dummy head node is symmetric.
 *        Checks if the sequence of elements reads the same forwards and backwards.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the doubly circular linked list
 * @return true if the list is empty or symmetric; false otherwise
 */
bool IsSymmetric(DLinkList L) {
    // 1. Handle edge cases: Null list or empty list (only head node) is symmetric
    if (L == nullptr || L->next == L) {
        return true;
    }

    // 2. Initialize left pointer pointing to the first data node,
    //    and right pointer pointing to the last data node
    DNode *left = L->next;
    DNode *right = L->prior;

    // 3. Traverse inward from both ends towards the center
    while (left != right && right->next != left) {
        // If data values do not match, the list is not symmetric
        if (left->data != right->data) {
            return false;
        }

        // Advance left pointer clockwise and right pointer counter-clockwise
        left = left->next;
        right = right->prior;
    }

    // 4. All corresponding node pairs matched
    return true;
}

/**
 * @brief Application Q12: Concatenate circular singly linked list h2 to the end of h1,
 *        maintaining a valid circular singly linked list structure and freeing h2's dummy head node.
 *        Time Complexity: O(m + n), Space Complexity: O(1)
 * @param h1 Pointer to the dummy head node of the first circular singly linked list
 * @param h2 Pointer to the dummy head node of the second circular singly linked list
 * @return LinkList Pointer to the merged circular singly linked list's dummy head node (h1)
 */
LinkList MergeCircularLists(LinkList h1, LinkList h2) {
    // 1. Handle cases where either list is null
    if (h1 == nullptr) return h2;
    if (h2 == nullptr) return h1;

    // 2. Find the tail node of the first circular list (p1)
    LNode *p1 = h1;
    while (p1->next != h1) {
        p1 = p1->next;
    }

    // 3. Find the tail node of the second circular list (p2)
    LNode *p2 = h2;
    while (p2->next != h2) {
        p2 = p2->next;
    }

    // 4. Link the tail of h1 to the first data node of h2
    p1->next = h2->next;

    // 5. Link the tail of h2 back to the head of h1 to close the loop
    p2->next = h1;

    // 6. Delete the redundant dummy head node of h2
    delete h2;

    return h1;
}

struct DNodeF {
    ElemType data;
    int freq;
    DNodeF *prior;
    DNodeF *next;
    
    DNodeF(ElemType val = 0, int f = 0, DNodeF *p = nullptr, DNodeF *n = nullptr)
        : data(val), freq(f), prior(p), next(n) {}
};

using DLinkListF = DNodeF*;

/**
 * @brief Application Q13: Search for element x in a non-circular doubly linked list L with dummy head node.
 *        Increments the target node's freq field by 1 and moves it forward so that nodes are sorted
 *        by freq in descending order (with most recently accessed nodes placed before nodes of equal freq).
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the doubly linked list
 * @param x The element value to search for
 * @return DNodeF* Pointer to the located node, or nullptr if x is not found
 */
DNodeF* Locate(DLinkListF L, ElemType x) {
    // 1. Check if the list is empty or invalid
    if (L == nullptr || L->next == nullptr) {
        return nullptr;
    }

    // 2. Search for node with value x
    DNodeF *p = L->next;
    while (p != nullptr && p->data != x) {
        p = p->next;
    }

    // 3. If x is not found, return nullptr
    if (p == nullptr) {
        return nullptr;
    }

    // 4. Increment the frequency count of the found node
    p->freq++;

    // 5. Detach node p from its current position
    p->prior->next = p->next;
    if (p->next != nullptr) {
        p->next->prior = p->prior;
    }

    // 6. Find insertion position: scan backward for a node with freq > p->freq
    //    (Strictly greater ensures p is placed BEFORE any nodes with equal freq)
    DNodeF *q = p->prior;
    while (q != L && q->freq <= p->freq) {
        q = q->prior;
    }

    // 7. Insert node p after node q
    p->next = q->next;
    if (q->next != nullptr) {
        q->next->prior = p;
    }
    p->prior = q;
    q->next = p;

    // 8. Return pointer to the located node
    return p;
}

/**
 * @brief Application Q14: Rotate a singly linked list WITHOUT a dummy head node to the right by k positions.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param head Reference to the head pointer of the singly linked list (without dummy head)
 * @param k Number of positions to rotate right (0 < k < n)
 */
void RotateRight(LNode* &head, int k) {
    // 1. Handle edge cases: empty list or single node
    if (head == nullptr || head->next == nullptr || k <= 0) {
        return;
    }

    // 2. Compute the length of the list and find the tail node
    int n = 1;
    LNode *tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
        n++;
    }

    // 3. Normalize k to prevent redundant full rotations
    k = k % n;
    if (k == 0) {
        return;
    }

    // 4. Form a circular linked list by connecting tail to head
    tail->next = head;

    // 5. Find the new tail position: (n - k) steps from head (1-indexed count is n - k)
    int stepsToNewTail = n - k;
    LNode *newTail = head;
    for (int i = 1; i < stepsToNewTail; ++i) {
        newTail = newTail->next;
    }

    // 6. Set the new head and break the circle
    head = newTail->next;
    newTail->next = nullptr;
}

/**
 * @brief Application Q15: Determine whether a singly linked list contains a cycle using Floyd's Cycle-Finding Algorithm.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 * @return true if a cycle exists in the list; false otherwise
 */
bool HasCycle(LinkList L) {
    // 1. Handle edge cases: Null list or empty list (only head node) cannot have a cycle
    if (L == nullptr || L->next == nullptr) {
        return false;
    }

    // 2. Initialize slow and fast pointers starting from the first data node
    LNode *slow = L->next;
    LNode *fast = L->next;

    // 3. Traverse the list: fast moves 2 steps, slow moves 1 step
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        // 4. If slow and fast pointers meet, a cycle is detected
        if (slow == fast) {
            return true;
        }
    }

    // 5. Fast pointer reached the end (nullptr), meaning no cycle exists
    return false;
}

/**
 * @brief Application Q16: Find the maximum twin sum of a singly linked list WITHOUT a dummy head (length n is even).
 *        Twin sum of the i-th node is defined as node[i]->data + node[n - 1 - i]->data.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param head Pointer to the head node of the unheaded singly linked list
 * @return int Maximum twin sum of the list
 */
int PairSum(LNode* head) {
    if (head == nullptr) return 0;

    // 1. Locate the start of the second half using fast and slow pointers
    LNode *slow = head;
    LNode *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Reverse the second half of the list in-place
    LNode *prev = nullptr;
    LNode *curr = slow;
    while (curr != nullptr) {
        LNode *nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    // 'prev' now points to the head of the reversed second half

    // 3. Traverse both halves simultaneously to compute twin sums
    int maxTwinSum = 0;
    LNode *firstHalf = head;
    LNode *secondHalf = prev;
    
    while (secondHalf != nullptr) {
        int currentSum = firstHalf->data + secondHalf->data;
        if (currentSum > maxTwinSum) {
            maxTwinSum = currentSum;
        }
        firstHalf = firstHalf->next;
        secondHalf = secondHalf->next;
    }

    // 4. Restore the second half to its original structure
    curr = prev;
    prev = nullptr;
    while (curr != nullptr) {
        LNode *nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }

    return maxTwinSum;
}

/**
 * @brief Application Q17: Find the k-th node from the end in a singly linked list with a dummy head node.
 *        Does not modify the linked list.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 * @param k The position from the end (1-indexed, positive integer)
 * @return int Returns 1 and prints the data if found; returns 0 otherwise
 */
int SearchkthToLast(LinkList L, int k) {
    // 1. Handle invalid inputs or empty list
    if (L == nullptr || L->next == nullptr || k <= 0) {
        return 0;
    }

    // 2. Initialize fast pointer (p) and slow pointer (q) to the first data node
    LNode *p = L->next;
    LNode *q = L->next;

    // 3. Move fast pointer p forward by k steps
    int count = 0;
    while (p != nullptr && count < k) {
        p = p->next;
        count++;
    }

    // 4. If count < k, the list length is less than k
    if (count < k) {
        return 0;
    }

    // 5. Move both pointers forward until p reaches the end (nullptr)
    while (p != nullptr) {
        p = p->next;
        q = q->next;
    }

    // 6. Node q is now pointing to the k-th node from the end
    std::cout << "Data at k-th position from last: " << q->data << std::endl;
    return 1;
}

/**
 * @brief Application Q18: Find the starting node of the shared suffix of two words 
 *        represented by singly linked lists with dummy head nodes.
 *        Time Complexity: O(m + n), Space Complexity: O(1)
 * @param str1 Pointer to the dummy head node of the first word list
 * @param str2 Pointer to the dummy head node of the second word list
 * @return LNode* Pointer to the first shared node, or nullptr if no shared suffix exists
 */
LNode* FindSharedSuffixStart(LinkList str1, LinkList str2) {
    // 1. Handle edge cases where either list is null or empty
    if (str1 == nullptr || str2 == nullptr || str1->next == nullptr || str2->next == nullptr) {
        return nullptr;
    }

    // 2. Compute length of list str1
    int len1 = 0;
    LNode *p1 = str1->next;
    while (p1 != nullptr) {
        len1++;
        p1 = p1->next;
    }

    // 3. Compute length of list str2
    int len2 = 0;
    LNode *p2 = str2->next;
    while (p2 != nullptr) {
        len2++;
        p2 = p2->next;
    }

    // 4. Reset pointers to the first data nodes
    p1 = str1->next;
    p2 = str2->next;

    // 5. Advance the pointer of the longer list by the length difference
    if (len1 > len2) {
        int diff = len1 - len2;
        while (diff--) {
            p1 = p1->next;
        }
    } else if (len2 > len1) {
        int diff = len2 - len1;
        while (diff--) {
            p2 = p2->next;
        }
    }

    // 6. Move both pointers together until they meet (address match)
    while (p1 != nullptr && p2 != nullptr && p1 != p2) {
        p1 = p1->next;
        p2 = p2->next;
    }

    // 7. Return the shared starting node (or nullptr if p1 reach the end)
    return p1;
}

/**
 * @brief Application Q19: Delete nodes with duplicate absolute values from a singly linked list with a dummy head.
 *        Time Complexity: O(m), Space Complexity: O(n)
 * @param L Pointer to the dummy head node of the singly linked list
 * @param n Maximum possible absolute value of data (|data| <= n)
 */
void DeleteAbsDuplicates(LinkList L, int n) {
    if (L == nullptr || L->next == nullptr) {
        return;
    }
    
    // 1. Auxiliary array to track od seen absolute values, size (n + 1)
    std::vector<bool> visited(static_cast<std::size_t>(n + 1), false);
    
    LNode *pre = L;
    LNode *p = L->next;
    
    // 2. Traverse the list once
    while (p != nullptr) {
        int absVal = std::abs(p->data);
        
        if (!visited[static_cast<std::size_t>(absVal)]) {
            // First time seeing value found -> delete node p
            visited[static_cast<std::size_t>(absVal)] = true;
            pre = p;
            p = p->next;
        } else {
            // Duplicate absolute value found -> delete node p
            LNode *temp = p;
            pre->next = p->next;
            p = p->next;
            delete temp;
        }
    }
}

/**
 * @brief Application Q20: Reorder list L=(a1, a2, ..., an) into L'=(a1, an, a2, an-1, ...)
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Pointer to the dummy head node of the singly linked list
 */
void ReorderList(LinkList L) {
    if (L == nullptr || L->next == nullptr || L->next->next == nullptr) {
        return; // No reordering needed for lists with 0, 1, or 2 nodes
    }

    // 1. Find the middle node using slow and fast pointers
    LNode *slow = L->next;
    LNode *fast = L->next;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 2. Split the list into two halves and reverse the second half
    LNode *head2 = slow->next;
    slow->next = nullptr; // Disconnect first half

    LNode *prev = nullptr;
    LNode *curr = head2;
    while (curr != nullptr) {
        LNode *nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    head2 = prev; // Head of reversed second half

    // 3. Interleave/Merge the two lists (L1 and reversed L2)
    LNode *p1 = L->next;
    LNode *p2 = head2;

    while (p2 != nullptr) {
        LNode *p1Next = p1->next;
        LNode *p2Next = p2->next;

        p1->next = p2;
        p2->next = p1Next;

        p1 = p1Next;
        p2 = p2Next;
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

    // Application Q11
    std::cout << "===== Application Q11 =====" << std::endl;

    // Test Case 1: Symmetric List [ 1, 2, 3, 2, 1 ]
    CircularDoublyLinkedList list11_1;
    DLinkList L11_1 = list11_1.GetHead();
    list11_1.ListInsert(L11_1, 1, 1);
    list11_1.ListInsert(L11_1, 2, 2);
    list11_1.ListInsert(L11_1, 3, 3);
    list11_1.ListInsert(L11_1, 4, 2);
    list11_1.ListInsert(L11_1, 5, 1);

    std::cout << "List 1: ";
    list11_1.PrintList(); // Expected: [ 1 2 3 2 1 ]
    std::cout << "Is List 1 symmetric? "
              << (IsSymmetric(L11_1) ? "Yes" : "No") << std::endl; // Expected: Yes

    // Test Case 2: Asymmetric List [ 1, 2, 3, 4, 1 ]
    CircularDoublyLinkedList list11_2;
    DLinkList L11_2 = list11_2.GetHead();
    list11_2.ListInsert(L11_2, 1, 1);
    list11_2.ListInsert(L11_2, 2, 2);
    list11_2.ListInsert(L11_2, 3, 3);
    list11_2.ListInsert(L11_2, 4, 4);
    list11_2.ListInsert(L11_2, 5, 1);

    std::cout << "List 2: ";
    list11_2.PrintList(); // Expected: [ 1 2 3 4 1 ]
    std::cout << "Is List 2 symmetric? "
              << (IsSymmetric(L11_2) ? "Yes" : "No") << std::endl; // Expected: No

    // Application Q12
    std::cout << "===== Application Q12 =====" << std::endl;

    CircularSinglyLinkedList list12_1, list12_2;
    LinkList h1 = list12_1.GetHead();
    LinkList h2 = list12_2.GetHead();

    // Populate List 1: [ 1, 2, 3 ]
    list12_1.ListInsert(h1, 1, 1);
    list12_1.ListInsert(h1, 2, 2);
    list12_1.ListInsert(h1, 3, 3);

    // Populate List 2: [ 4, 5, 6 ]
    list12_2.ListInsert(h2, 1, 4);
    list12_2.ListInsert(h2, 2, 5);
    list12_2.ListInsert(h2, 3, 6);

    std::cout << "List 1 before merge: ";
    list12_1.PrintList(); // Expected: [ 1 2 3 ]

    std::cout << "List 2 before merge: ";
    list12_2.PrintList(); // Expected: [ 4 5 6 ]

    // Merge lists
    LinkList mergedHead = MergeCircularLists(h1, h2);

    // Output merged circular list elements
    std::cout << "Merged List: [ ";
    LNode *p = mergedHead->next;
    while (p != mergedHead) {
        std::cout << p->data << " ";
        p = p->next;
    }
    std::cout << "]" << std::endl; // Expected: [ 1 2 3 4 5 6 ]

    // Application Q13
    std::cout << "===== Application Q13 =====" << std::endl;

    // Create a dummy head node
    DLinkListF L13 = new DNodeF();

    // Helper lambda to insert node at tail
    auto appendNode = [](DLinkListF head, ElemType val) {
        DNodeF *p13 = head;
        while (p13->next != nullptr) p13 = p13->next;
        DNodeF *newNode = new DNodeF(val, 0, p13, nullptr);
        p13->next = newNode;
    };

    // Construct initial non-circular doubly linked list: [ 10, 20, 30, 40 ]
    appendNode(L13, 10);
    appendNode(L13, 20);
    appendNode(L13, 30);
    appendNode(L13, 40);

    // Lambda to print list with node frequencies
    auto printListWithFreq = [](DLinkListF head) {
        std::cout << "[ ";
        DNodeF *p13 = head->next;
        while (p13 != nullptr) {
            std::cout << p13->data << "(freq:" << p13->freq << ") ";
            p13 = p13->next;
        }
        std::cout << "]" << std::endl;
    };

    std::cout << "Initial List: ";
    printListWithFreq(L13); // Expected: [ 10(freq:0) 20(freq:0) 30(freq:0) 40(freq:0) ]

    // Access node 30 once
    Locate(L13, 30);
    std::cout << "After Locate(L, 30): ";
    printListWithFreq(L13); // Expected: [ 30(freq:1) 10(freq:0) 20(freq:0) 40(freq:0) ]

    // Access node 20 twice
    Locate(L13, 20);
    Locate(L13, 20);
    std::cout << "After Locate(L, 20) x2: ";
    printListWithFreq(L13); // Expected: [ 20(freq:2) 30(freq:1) 10(freq:0) 40(freq:0) ]

    // Access node 30 again (freq becomes 2, should jump ahead of 20 due to recent-access priority)
    Locate(L13, 30);
    std::cout << "After Locate(L, 30) again: ";
    printListWithFreq(L13); // Expected: [ 30(freq:2) 20(freq:2) 10(freq:0) 40(freq:0) ]

    // Access node 40
    Locate(L13, 40);
    std::cout << "After Locate(L, 40): ";
    printListWithFreq(L13); // Expected: [ 30(freq:2) 20(freq:2) 40(freq:1) 10(freq:0) ]

    // Application Q14
    std::cout << "===== Application Q14 =====" << std::endl;

    // Helper lambda to print a list without a dummy head node
    auto printListNoHead = [](LNode *head) {
        std::cout << "[ ";
        LNode *p14 = head;
        while (p14 != nullptr) {
            std::cout << p14->data << " ";
            p14 = p14->next;
        }
        std::cout << "]" << std::endl;
    };

    // Construct an unheaded list: {0, 1, 2, 3}
    LNode *head14 = new LNode(0);
    head14->next = new LNode(1);
    head14->next->next = new LNode(2);
    head14->next->next->next = new LNode(3);

    std::cout << "Original List: ";
    printListNoHead(head14); // Expected: [ 0 1 2 3 ]

    // Rotate right by k = 1
    int k = 1;
    RotateRight(head14, k);

    std::cout << "After Rotate Right by " << k << ": ";
    printListNoHead(head14); // Expected: [ 3 0 1 2 ]

    // Rotate right by k = 2
    k = 2;
    RotateRight(head14, k);

    std::cout << "After Rotate Right by " << k << ": ";
    printListNoHead(head14); // Expected: [ 1 2 3 0 ]

    // Memory cleanup
    while (head14 != nullptr) {
        LNode *temp = head14;
        head14 = head14->next;
        delete temp;
    }
    
    // Application Q15
    std::cout << "===== Application Q15 =====" << std::endl;

    SinglyLinkedList list15;
    LinkList L15 = list15.GetHead();

    // Populate List: [ 1, 2, 3, 4, 5 ]
    list15.ListInsert(L15, 1, 1);
    list15.ListInsert(L15, 2, 2);
    list15.ListInsert(L15, 3, 3);
    list15.ListInsert(L15, 4, 4);
    list15.ListInsert(L15, 5, 5);

    // Test Case 1: Standard linear list (no cycle)
    std::cout << "Has cycle in normal list? " 
              << (HasCycle(L15) ? "Yes" : "No") << std::endl; // Expected: No

    // Test Case 2: Manually create a cycle (connect tail node 5 to node 3)
    LNode *node3 = L15->next->next->next; // Pointing to node '3'
    LNode *tail = L15->next;
    while (tail->next != nullptr) {
        tail = tail->next;
    }
    tail->next = node3; // Tail (5) now points to Node 3

    std::cout << "Has cycle after creating loop? " 
              << (HasCycle(L15) ? "Yes" : "No") << std::endl; // Expected: Yes

    // Cleanup: Break the cycle manually to prevent memory leak / infinite loop during destruction
    tail->next = nullptr;
    
    // Application Q16
    std::cout << "===== Application Q16 =====" << std::endl;

    // Helper lambda to print a list without a dummy head node
    auto printListNoHead16 = [](LNode *h) {
        std::cout << "[ ";
        LNode *p16 = h;
        while (p16 != nullptr) {
            std::cout << p16->data << " ";
            p16 = p16->next;
        }
        std::cout << "]" << std::endl;
    };

    // Construct an unheaded list: {5, 4, 2, 1}
    // Twin pairs: (5, 1) -> sum 6; (4, 2) -> sum 6; Max twin sum = 6
    LNode *head16 = new LNode(5);
    head16->next = new LNode(4);
    head16->next->next = new LNode(2);
    head16->next->next->next = new LNode(1);

    std::cout << "List: ";
    printListNoHead16(head16);

    std::cout << "Maximum Twin Sum: " << PairSum(head16) << std::endl; // Expected: 6

    // Construct another list: {1, 100, 3, 4}
    // Twin pairs: (1, 4) -> sum 5; (100, 3) -> sum 103; Max twin sum = 103
    LNode *head16_2 = new LNode(1);
    head16_2->next = new LNode(100);
    head16_2->next->next = new LNode(3);
    head16_2->next->next->next = new LNode(4);

    std::cout << "\nList 2: ";
    printListNoHead16(head16_2);

    std::cout << "Maximum Twin Sum: " << PairSum(head16_2) << std::endl; // Expected: 103

    // Memory cleanup
    while (head16 != nullptr) {
        LNode *temp = head16;
        head16 = head16->next;
        delete temp;
    }
    while (head16_2 != nullptr) {
        LNode *temp = head16_2;
        head16_2 = head16_2->next;
        delete temp;
    }
    
    // Application Q17
    std::cout << "===== Application Q17 =====" << std::endl;

    SinglyLinkedList list17;
    LinkList L17 = list17.GetHead();

    // Populate List: [ 10, 20, 30, 40, 50 ]
    list17.ListInsert(L17, 1, 10);
    list17.ListInsert(L17, 2, 20);
    list17.ListInsert(L17, 3, 30);
    list17.ListInsert(L17, 4, 40);
    list17.ListInsert(L17, 5, 50);

    std::cout << "Original List: ";
    list17.PrintList(); // Expected: [ 10 20 30 40 50 ]

    // Test Case 1: Find 2nd node from end (Expected: 40, returns 1)
    int k1 = 2;
    std::cout << "Searching for k = " << k1 << ": " << std::endl;
    int res1 = SearchkthToLast(L17, k1);
    std::cout << "Return value: " << res1 << std::endl;

    // Test Case 2: Find 5th node from end (Expected: 10, returns 1)
    int k2 = 5;
    std::cout << "\nSearching for k = " << k2 << ": " << std::endl;
    int res2 = SearchkthToLast(L17, k2);
    std::cout << "Return value: " << res2 << std::endl;

    // Test Case 3: Out of bound k = 10 (Expected: returns 0)
    int k3 = 10;
    std::cout << "\nSearching for k = " << k3 << ": " << std::endl;
    int res3 = SearchkthToLast(L17, k3);
    std::cout << "Return value: " << res3 << std::endl;
    
    // Application Q18
    std::cout << "===== Application Q18 =====" << std::endl;

    // Construct common suffix nodes: 'i' -> 'n' -> 'g'
    LNode *nodeG = new LNode('g');
    LNode *nodeN = new LNode('n', nodeG);
    LNode *nodeI = new LNode('i', nodeN); // Suffix start node

    // Construct List 1 ("loading"): 'l' -> 'o' -> 'a' -> 'd' -> 'i' -> 'n' -> 'g'
    SinglyLinkedList list18_1;
    LinkList str1 = list18_1.GetHead();
    LNode *nodeD = new LNode('d', nodeI);
    LNode *nodeA = new LNode('a', nodeD);
    LNode *nodeO = new LNode('o', nodeA);
    LNode *nodeL = new LNode('l', nodeO);
    str1->next = nodeL;

    // Construct List 2 ("being"): 'b' -> 'e' -> 'i' -> 'n' -> 'g'
    SinglyLinkedList list18_2;
    LinkList str2 = list18_2.GetHead();
    LNode *nodeE = new LNode('e', nodeI);
    LNode *nodeB = new LNode('b', nodeE);
    str2->next = nodeB;

    // Find the shared suffix start
    LNode *sharedStart = FindSharedSuffixStart(str1, str2);

    if (sharedStart != nullptr) {
        std::cout << "Shared suffix starts at node with data: " 
                  << static_cast<char>(sharedStart->data) << std::endl; // Expected: 'i'
    } else {
        std::cout << "No shared suffix found." << std::endl;
    }

    // Memory cleanup
    // Unlink shared nodes to allow independent list cleanup / avoiding double-free
    nodeD->next = nullptr;
    nodeE->next = nullptr;
    while (nodeI != nullptr) {
        LNode *temp = nodeI;
        nodeI = nodeI->next;
        delete temp;
    }
    
    // Application Q19
    std::cout << "===== Application Q19 =====" << std::endl;

    SinglyLinkedList list19;
    LinkList L19 = list19.GetHead();

    // Max absolute value n = 25
    int n19 = 25;

    // Populate List: [ 21, -15, -15, 15, 7, -21 ]
    list19.ListInsert(L19, 1, 21);
    list19.ListInsert(L19, 2, -15);
    list19.ListInsert(L19, 3, -15);
    list19.ListInsert(L19, 4, 15);
    list19.ListInsert(L19, 5, 7);
    list19.ListInsert(L19, 6, -21);

    std::cout << "Original List: ";
    list19.PrintList(); // Expected: [ 21 -15 -15 15 7 -21 ]

    DeleteAbsDuplicates(L19, n19);

    std::cout << "After Removing Absolute Duplicates: ";
    list19.PrintList(); // Expected: [ 21 -15 7 ]

    // Application Q20
    std::cout << "===== Application Q20 =====" << std::endl;

    SinglyLinkedList list20;
    LinkList L20 = list20.GetHead();

    // Populate List with even number of nodes: [ 1, 2, 3, 4, 5, 6 ]
    for (int i = 1; i <= 6; ++i) {
        list20.ListInsert(L20, i, i);
    }

    std::cout << "Original List (Even): ";
    list20.PrintList(); // Expected: [ 1 2 3 4 5 6 ]

    ReorderList(L20);

    std::cout << "Reordered List (Even):  ";
    list20.PrintList(); // Expected: [ 1 6 2 5 3 4 ]

    // Populate another list with odd number of nodes: [ 10, 20, 30, 40, 50 ]
    SinglyLinkedList list20_odd;
    LinkList L20_odd = list20_odd.GetHead();
    for (int i = 1; i <= 5; ++i) {
        list20_odd.ListInsert(L20_odd, i, i * 10);
    }

    std::cout << "\nOriginal List (Odd):  ";
    list20_odd.PrintList(); // Expected: [ 10 20 30 40 50 ]

    ReorderList(L20_odd);

    std::cout << "Reordered List (Odd):   ";
    list20_odd.PrintList(); // Expected: [ 10 50 20 40 30 ]

    return 0;
}