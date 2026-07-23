#include <link_list.h>
#include <iostream>

// Constructor: Allocates the dummy head node and sets the member variable 'head' to point to it
SinglyLinkedList::SinglyLinkedList() {
    InitList(head);
}

// Destructor: Deallocates all dynamically allocated nodes, including the dummy head node
SinglyLinkedList::~SinglyLinkedList() {
    LNode *p = head;
    while (p != nullptr) {
        LNode *temp = p;
        p = p->next;
        delete temp;
    }
    head = nullptr;
}

// Initializes a singly linked list with a dummy head node
bool SinglyLinkedList::InitList(LinkList &L) {
    L = new (std::nothrow) LNode();
    if (L == nullptr) {
        return false; // Memory allocation failed
    }
    L->next = nullptr;
    return true;
}

// Calculates the length of the list (excluding the dummy head node)
int SinglyLinkedList::Length(LinkList L) const {
    int len = 0;
    if (L == nullptr) return 0;
    
    LNode *p = L->next; // Start counting from the first data node
    while (p != nullptr) {
        len++;
        p = p->next;
    }
    return len;
}

// Locates a node by its position (1-indexed)
// Returns the dummy head node when i = 0, or nullptr if i is out of bounds
SinglyLinkedList::LNode* SinglyLinkedList::GetElem(LinkList L, int i) const {
    if (i < 0 || L == nullptr) return nullptr;
    
    LNode *p = L; // p points to the dummy head node (0th position)
    int j = 0;
    
    while (p != nullptr && j < i) {
        p = p->next;
        j++;
    }
    return p; // Returns node pointer if j == i, or nullptr if i > length
}

// Locates a node by value: Returns the pointer to the first node with value 'e', or nullptr if not found
SinglyLinkedList::LNode* SinglyLinkedList::LocateElem(LinkList L, ElemType e) const {
    if (L == nullptr) return nullptr;
    
    LNode *p = L->next; // Start searching from the first data node
    while (p != nullptr && p->data != e) {
        p = p->next;
    }
    return p;
}

// Inserts a new node with value 'e' at the specified position (1-indexed)
bool SinglyLinkedList::ListInsert(LinkList &L, int i, ElemType e) {
    // Find the (i-1)-th node (predecessor node)
    LNode *p = GetElem(L, i - 1);
    if (p == nullptr) {
        return false; // Invalid position: i < 1 or i > length + 1
    }

    // Allocate new node and update pointers
    LNode *s = new (std::nothrow) LNode(e, p->next);
    if (s == nullptr) {
        return false; // Memory allocation failed
    }
    p->next = s;
    return true;
}

// Deletes the node at position 'i' (1-indexed) and returns its data via reference parameter 'e'
bool SinglyLinkedList::ListDelete(LinkList &L, int i, ElemType &e) {
    // Find the (i-1)-th node (predecessor node)
    LNode *p = GetElem(L, i - 1);
    if (p == nullptr || p->next == nullptr) {
        return false; // Invalid position or position exceeds data node range
    }

    LNode *q = p->next; // q points to the node to be deleted
    e = q->data;        // Store node value to return
    p->next = q->next;  // Unlink the target node
    delete q;           // Free allocated memory
    return true;
}

// Creates/populates a list using head insertion (input ends with sentinel value -9999)
SinglyLinkedList::LinkList SinglyLinkedList::List_HeadInsert(LinkList &L) {
    ElemType x;
    std::cout << "Enter node values (enter -9999 to terminate): ";
    while (std::cin >> x && x != -9999) {
        LNode *s = new (std::nothrow) LNode(x, L->next);
        if (s == nullptr) break;
        L->next = s;
    }
    return L;
}

// Creates/populates a list using tail insertion (input ends with sentinel value -9999)
SinglyLinkedList::LinkList SinglyLinkedList::List_TailInsert(LinkList &L) {
    ElemType x;
    LNode *r = L; // Pointer 'r' tracks the tail node
    while (r->next != nullptr) { // Ensure 'r' points to the last node
        r = r->next;
    }

    std::cout << "Enter node values (enter -9999 to terminate): ";
    while (std::cin >> x && x != -9999) {
        LNode *s = new (std::nothrow) LNode(x, nullptr);
        if (s == nullptr) break;
        r->next = s;
        r = s; // Update tail pointer to the new node
    }
    return L;
}

// Prints all elements in the single linked list
void SinglyLinkedList::PrintList() const {
    if (head == nullptr) {
        std::cout << "List is uninitialized." << std::endl;
        return;
    }

    LNode *p = head->next; // Skip the dummy head node
    std::cout << "Head -> ";
    while (p != nullptr) {
        std::cout << p->data << " -> ";
        p = p->next;
    }
    std::cout << "NULL" << std::endl;
}