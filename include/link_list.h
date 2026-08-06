#ifndef LINK_LIST_H
#define LINK_LIST_H

namespace LinkListStyle {
    // Reserved element type, using int as a default example. 
    // Can be changed to other types in practical use.
    using ElemType = int;

    // Structure definitions: 
    // ============================================
    // 1. Singly Linked List - with dummy head node
    // ============================================
    class SinglyLinkedList {
    public:
        // Node structure
        typedef struct LNode {
            ElemType data;
            struct LNode *next;
            LNode(ElemType val = 0, LNode *n = nullptr) : data(val), next(n) {}
        } LNode, *LinkList;

    private:
        LinkList head; // Pointer to dummy head node

    public:
        SinglyLinkedList();
        ~SinglyLinkedList();

        // Initialize singly linked list (with dummy head node)
        bool InitList(LinkList &L);

        // Get length of the list
        int Length(LinkList L) const;

        // Locate node by position (1-indexed)
        LNode* GetElem(LinkList L, int i) const;

        // Locate node by value
        LNode* LocateElem(LinkList L, ElemType e) const;

        // Insert node at the specified position (1-indexed)
        bool ListInsert(LinkList &L, int i, ElemType e);

        // Delete node at the specified position (1-indexed) and return its value in e
        bool ListDelete(LinkList &L, int i, ElemType &e);

        // Create a singly linked list using head insertion
        LinkList List_HeadInsert(LinkList &L);

        // Create a singly linked list using tail insertion
        LinkList List_TailInsert(LinkList &L);

        // Print all elements in the list
        void PrintList() const;

        // Getter for class instance head pointer
        LinkList GetHead() const { return head; }
    };

    // ============================================
    // 2. Doubly Linked List (With Dummy Head Node)
    // ============================================
    class DoublyLinkedList {
    public:
        // Node structure
        typedef struct DNode {
            ElemType data;
            struct DNode *prior;
            struct DNode *next;
            DNode(ElemType val = 0, DNode *p = nullptr, DNode *n = nullptr)
                : data(val), prior(p), next(n) {}
        } DNode, *DLinkList;

    private:
        DLinkList head; // Pointer to dummy head node

    public:
        DoublyLinkedList();
        ~DoublyLinkedList();

        // Initialize doubly linked list
        bool InitList(DLinkList &L);

        // Insert node at the specified position (1-indexed)
        bool ListInsert(DLinkList &L, int i, ElemType e);

        // Delete node at the specified position (1-indexed) and return its value in e
        bool ListDelete(DLinkList &L, int i, ElemType &e);

        // Print all elements in the list
        void PrintList() const;

        // Getter for class instance head pointer
        DLinkList GetHead() const { return head; }
    };

    // =================================================================
    // 3-1. Circular Singly Linked List (With Dummy Head Node)
    // =================================================================
    class CircularSinglyLinkedList {
    public:
        typedef SinglyLinkedList::LNode LNode;
        typedef SinglyLinkedList::LinkList LinkList;

    private:
        LinkList head; // Dummy head node, last node points back to head

    public:
        CircularSinglyLinkedList();
        ~CircularSinglyLinkedList();

        // Initialize circular singly linked list
        bool InitList(LinkList &L);

        // Insert node at the specified position (1-indexed)
        bool ListInsert(LinkList &L, int i, ElemType e);

        // Delete node at the specified position (1-indexed) and return its value in e
        bool ListDelete(LinkList &L, int i, ElemType &e);

        // Print all elements in the list
        void PrintList() const;

        // Getter for class instance head pointer
        LinkList GetHead() const { return head; }
    };

    // =================================================================
    // 3-2. Circular Doubly Linked List (With Dummy Head Node)
    // =================================================================
    class CircularDoublyLinkedList {
    public:
        typedef DoublyLinkedList::DNode DNode;
        typedef DoublyLinkedList::DLinkList DLinkList;

    private:
        DLinkList head; // Dummy head node, head->prior points to tail, tail->next points to head

    public:
        CircularDoublyLinkedList();
        ~CircularDoublyLinkedList();

        // Initialize circular doubly linked list
        bool InitList(DLinkList &L);

        // Insert node at the specified position (1-indexed)
        bool ListInsert(DLinkList &L, int i, ElemType e);

        // Delete node at the specified position (1-indexed) and return its value in e
        bool ListDelete(DLinkList &L, int i, ElemType &e);

        // Print all elements in the list
        void PrintList() const;

        // Getter for class instance head pointer
        DLinkList GetHead() const { return head; }
    };

    // =====================
    // 4. Static Linked List
    // =====================
    // Replace the original `#define MaxSize 128` to prevent macros from polluting the global scope.
    constexpr int MaxSize = 128;

    class StaticLinkedList {
    public:
        typedef struct {
            ElemType data;
            int next; // Cursor/index pointing to the next node (-1 acts as nullptr)
        } Component, SLinkList[MaxSize];

    private:
        SLinkList space;
        int head; // Index of the data list head
        int free; // Index of the free node list head

        // Internal allocation and deallocation helper methods
        int AllocateNode();
        void DeallocateNode(int index);

    public:
        StaticLinkedList();

        // Initialize static linked list
        void InitList(SLinkList L);

        // Insert node at the specified position (1-indexed)
        bool ListInsert(SLinkList L, int i, ElemType e);

        // Delete node at the specified position (1-indexed) and return its value in e
        bool ListDelete(SLinkList L, int i, ElemType &e);

        // Print all elements in the list
        void PrintList() const;
    };
} // namespace LinkListStyle

#endif // LINK_LIST_H