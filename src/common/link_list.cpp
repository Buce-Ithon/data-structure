#include <link_list.h>
#include <iostream>

// ===========================================
// 1. Singly LinkedList (With Dummy Head Node)
// ===========================================

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

// ===========================================
// 2. Doubly LinkedList (With Dummy Head Node)
// ===========================================

DoublyLinkedList::DoublyLinkedList() {
    InitList(head);
}

DoublyLinkedList::~DoublyLinkedList() {
    DNode *p = head;
    while (p != nullptr) {
        DNode *temp = p;
        p = p->next;
        delete temp;
    }
    head = nullptr;
}

bool DoublyLinkedList::InitList(DLinkList &L) {
    L = new (std::nothrow) DNode();
    if (L == nullptr) {
        return false;
    }
    L->prior = nullptr;
    L->next = nullptr;
    return true;
}

bool DoublyLinkedList::ListInsert(DLinkList &L, int i, ElemType e) {
    if (i < 1 || L == nullptr) return false;

    // Find the (i-1)th node (i.e., the predecessor node p).
    DNode *p = L;
    int j = 0;
    while (p != nullptr && j < i - 1) {
        p = p->next;
        j++;
    }

    if (p == nullptr) return false; // i exceeds the current length + 1

    DNode *s = new (std::nothrow) DNode(e, p, p->next);
    if (s == nullptr) return false;

    if (p->next != nullptr) {
        p->next->prior = s;
    }
    p->next = s;
    return true;
}

bool DoublyLinkedList::ListDelete(DLinkList &L, int i, ElemType &e) {
    if (i < 1 || L == nullptr) return false;

    // Find the (i-1)th node (i.e., the predecessor node p).
    DNode *p = L;
    int j = 0;
    while (p != nullptr && j < i - 1) {
        p = p->next;
        j++;
    }

    if (p == nullptr || p->next == nullptr) {
        return false; // invalid position
    }

    DNode *q = p->next; // q is the node that was deleted.
    e = q->data;
    p->next = q->next;
    if (q->next != nullptr) {
        q->next->prior = p;
    }
    delete q;
    return true;
}

void DoublyLinkedList::PrintList() const {
    if (head == nullptr) {
        std::cout << "List is uninitialized." << std::endl;
        return;
    }

    DNode *p = head->next;
    std::cout << "Head <-> ";
    while (p != nullptr) {
        std::cout << p->data << " <-> ";
        p = p->next;
    }
    std::cout << "NULL" << std::endl;
}


// =======================================================
// 3-1. Circular Singly Linked List (With Dummy Head Node)
// =======================================================

CircularSinglyLinkedList::CircularSinglyLinkedList() {
    InitList(head);
}

CircularSinglyLinkedList::~CircularSinglyLinkedList() {
    if (head == nullptr) return;

    LNode *p = head->next;
    while (p != head) { // Loop until getting back to the node
        LNode *temp = p;
        p = p->next;
        delete temp;
    }
    delete head;
    head = nullptr;
}

bool CircularSinglyLinkedList::InitList(LinkList &L) {
    L = new (std::nothrow) LNode();
    if (L == nullptr) {
        return false;
    }
    L->next = L; // A circular singly linked list: the head node's next pointer points to itself.
    return true;
}

bool CircularSinglyLinkedList::ListInsert(LinkList &L, int i, ElemType e) {
    if (i < 1 || L == nullptr) return false;

    // Find the (i-1)th node
    LNode *p = L;
    int j = 0;
    while (p->next != L && j < i - 1) {
        p = p->next;
        j++;
    }

    // Special handling: If the insertion point is at the end of the list + 1 (i.e., j == i - 1), the position is valid.
    if (j != i - 1) return false;

    LNode *s = new (std::nothrow) LNode(e, p->next);
    if (s == nullptr) return false;

    p->next = s;
    return true;
}

bool CircularSinglyLinkedList::ListDelete(LinkList &L, int i, ElemType &e) {
    if (i < 1 || L == nullptr || L->next == L) return false; // Empty list

    LNode *p = L;
    int j = 0;
    while (p->next != L && j < i - 1) {
        p = p->next;
        j++;
    }

    if (p->next == L || j != i - 1) {
        return false; // Location out of range
    }

    LNode *q = p->next; // The node that will be deleted.
    e = q->data;
    p->next = q->next;
    delete q;
    return true;
}

void CircularSinglyLinkedList::PrintList() const {
    if (head == nullptr) {
        std::cout << "List is uninitialized." << std::endl;
        return;
    }

    LNode *p = head->next;
    std::cout << "Head -> ";
    while (p != head) {
        std::cout << p->data << " -> ";
        p = p->next;
    }
    std::cout << "(Head)" << std::endl;
}


// =======================================================
// 3-2. Circular Doubly Linked List (With Dummy Head Node)
// =======================================================

CircularDoublyLinkedList::CircularDoublyLinkedList() {
    InitList(head);
}

CircularDoublyLinkedList::~CircularDoublyLinkedList() {
    if (head == nullptr) return;

    DNode *p = head->next;
    while (p != head) {
        DNode *temp = p;
        p = p->next;
        delete temp;
    }
    delete head;
    head = nullptr;
}

bool CircularDoublyLinkedList::InitList(DLinkList &L) {
    L = new (std::nothrow) DNode();
    if (L == nullptr) {
        return false;
    }
    L->next = L;
    L->prior = L;
    return true;
}

bool CircularDoublyLinkedList::ListInsert(DLinkList &L, int i, ElemType e) {
    if (i < 1 || L == nullptr) return false;

    DNode *p = L;
    int j = 0;
    while (p->next != L && j < i - 1) {
        p = p->next;
        j++;
    }

    if (j != i - 1) return false;

    DNode *s = new (std::nothrow) DNode(e, p, p->next);
    if (s == nullptr) return false;

    p->next->prior = s;
    p->next = s;
    return true;
}

bool CircularDoublyLinkedList::ListDelete(DLinkList &L, int i, ElemType &e) {
    if (i < 1 || L == nullptr || L->next == L) return false;

    DNode *p = L;
    int j = 0;
    while (p->next != L && j < i - 1) {
        p = p->next;
        j++;
    }

    if (p->next == L || j != i - 1) return false;

    DNode *q = p->next;
    e = q->data;
    q->next->prior = p;
    p->next = q->next;
    delete q;
    return true;
}

void CircularDoublyLinkedList::PrintList() const {
    if (head == nullptr) {
        std::cout << "List is uninitialized." << std::endl;
        return;
    }

    DNode *p = head->next;
    std::cout << "Head <-> ";
    while (p != head) {
        std::cout << p->data << " <-> ";
        p = p->next;
    }
    std::cout << "(Head)" << std::endl;
}


// =====================
// 4. Static Linked List
// =====================

StaticLinkedList::StaticLinkedList() {
    InitList(space);
}

// Internal method: Allocate a node index from the spare list
int StaticLinkedList::AllocateNode() {
    int i = free;
    if (i != -1) {
        free = space[i].next; // Update free cursor
    }
    return i;
}

// Internal method: Recycle the node with index index and return it to the standby linked list
void StaticLinkedList::DeallocateNode(int index) {
    space[index].next = free;
    free = index;
}

void StaticLinkedList::InitList(SLinkList L) {
    // Position 0 is used as the head node (head) of the data linked list
    head = 0;
    
    // Initialize the standby linked list (linked from index 1 to MaxSize - 1)
    free = 1;
    for (int i = 1; i < MaxSize - 1; ++i) {
        L[i].next = i + 1;
    }
    L[MaxSize - 1].next = -1; // -1 indicates the end of the linked list. NULL

    // Header node 0 initially points to -1
    L[head].next = -1;
}

bool StaticLinkedList::ListInsert(SLinkList L, int i, ElemType e) {
    if (i < 1) return false;

    // Find the i - 1th node
    int p = head;
    int j = 0;
    while (p != -1 && j < i - 1) {
        p = L[p].next;
        j++;
    }

    if (p == -1) return false;

    int sub = AllocateNode();
    if (sub == -1) return false; // Space allocation failed/space is full

    L[sub].data = e;
    L[sub].next = L[p].next;
    L[p].next = sub;
    return true;
}

bool StaticLinkedList::ListDelete(SLinkList L, int i, ElemType &e) {
    if (i < 1) return false;

    int p = head;
    int j = 0;
    while (p != -1 && j < i - 1) {
        p = L[p].next;
        j++;
    }

    if (p == -1 || L[p].next == -1) return false;

    int q = L[p].next; // The index of the node to be deleted
    e = L[q].data;
    L[p].next = L[q].next;
    
    DeallocateNode(q); // recycling node
    return true;
}

void StaticLinkedList::PrintList() const {
    int p = space[head].next;
    std::cout << "Head -> ";
    while (p != -1) {
        std::cout << space[p].data << " -> ";
        p = space[p].next;
    }
    std::cout << "NULL" << std::endl;
}