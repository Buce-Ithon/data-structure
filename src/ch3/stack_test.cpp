#include <iostream>
#include <vector>
#include <link_list.h>
#include <stack_c.h>

/**
 * @brief Determines whether a singly linked list with a dummy head node is symmetric (a palindrome).
 * 
 * Uses the fast and slow pointer algorithm to locate the midpoint of the linked list
 * while pushing elements of the first half onto a C-style sequential stack. Then, it compares 
 * the second half of the list against the elements popped from the stack.
 * 
 * @param L Pointer to the dummy head node of the singly linked list.
 * @return true If the linked list contents are symmetric or empty.
 * @return false If the linked list contents are not symmetric.
 */
bool IsSymmetric(LinkListStyle::SinglyLinkedList::LinkList L) {
    if (L == nullptr || L->next == nullptr) {
        return true; // Empty list or list with dummy head node only
    }

    LinkListStyle::SinglyLinkedList::LinkList slow = L->next;
    LinkListStyle::SinglyLinkedList::LinkList fast = L->next;

    StackCStyle::SqStack S;
    StackCStyle::InitStack(S); // Initialize the auxiliary stack

    // Advance fast pointer by 2 nodes and slow pointer by 1 node; push slow's data onto stack
    while (fast != nullptr && fast->next != nullptr) {
        StackCStyle::Push(S, slow->data);
        slow = slow->next;
        fast = fast->next->next;
    }

    // If list length is odd, skip the central element
    if (fast != nullptr) {
        slow = slow->next;
    }

    // Traverse the second half and compare with popped elements from the stack
    StackCStyle::ElemType topVal;
    while (slow != nullptr) {
        StackCStyle::Pop(S, topVal);
        if (topVal != slow->data) {
            return false; // Mismatch found
        }
        slow = slow->next;
    }

    return true; // All elements match
}

/**
 * @brief Helper function to create a singly linked list with a dummy head node from a vector of characters.
 * 
 * @param chars Vector containing character elements.
 * @return LinkListStyle::SinglyLinkedList::LinkList Pointer to the head node of the newly allocated list.
 */
LinkListStyle::SinglyLinkedList::LinkList CreateListFromChars(const std::vector<char>& chars) {
    LinkListStyle::SinglyLinkedList::LinkList head = new LinkListStyle::SinglyLinkedList::LNode(); // Allocate head node
    LinkListStyle::SinglyLinkedList::LinkList tail = head;
    for (char ch : chars) {
        LinkListStyle::SinglyLinkedList::LinkList node = new LinkListStyle::SinglyLinkedList::LNode(ch);
        tail->next = node;
        tail = node;
    }
    return head;
}

/**
 * @brief Helper function to safely deallocate all memory associated with a linked list.
 * 
 * @param L Reference to the pointer of the dummy head node to be freed.
 */
void FreeList(LinkListStyle::SinglyLinkedList::LinkList &L) {
    LinkListStyle::SinglyLinkedList::LinkList p = L;
    while (p != nullptr) {
        LinkListStyle::SinglyLinkedList::LinkList temp = p;
        p = p->next;
        delete temp;
    }
    L = nullptr;
}

int main() {
    std::cout << "Hello, stack~" << std::endl;

    // Test Case 1: Even-length palindrome ("a", "b", "b", "a")
    LinkListStyle::SinglyLinkedList::LinkList l1 = CreateListFromChars({'a', 'b', 'b', 'a'});
    std::cout << "Test 1 [abba]: " << (IsSymmetric(l1) ? "Symmetric" : "Not Symmetric") << std::endl;
    FreeList(l1);

    // Test Case 2: Odd-length palindrome ("r", "a", "c", "e", "c", "a", "r")
    LinkListStyle::SinglyLinkedList::LinkList l2 = CreateListFromChars({'r', 'a', 'c', 'e', 'c', 'a', 'r'});
    std::cout << "Test 2 [racecar]: " << (IsSymmetric(l2) ? "Symmetric" : "Not Symmetric") << std::endl;
    FreeList(l2);

    // Test Case 3: Non-palindrome ("a", "b", "c", "d")
    LinkListStyle::SinglyLinkedList::LinkList l3 = CreateListFromChars({'a', 'b', 'c', 'd'});
    std::cout << "Test 3 [abcd]: " << (IsSymmetric(l3) ? "Symmetric" : "Not Symmetric") << std::endl;
    FreeList(l3);

    return 0;
}