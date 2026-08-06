#ifndef STACK_C_H
#define STACK_C_H

#include <new>

namespace StackCStyle {
    // Define element type, modify as needed
    typedef int ElemType;

    // Replace the original `#define MaxSize 128` to prevent macros from polluting the global scope.
    constexpr int MaxSize = 128;

    // ==================== 1. Data Structure Definitions ====================

    // Sequential stack structure
    typedef struct {
        ElemType data[MaxSize];
        int top; // Top pointer, initialized to -1
    } SqStack;

    // Linked stack node and pointer definition (implemented using a singly linked list without a head node)
    typedef struct LinkNode {
        ElemType data;
        struct LinkNode *next;
    } LinkNode, *LiStack;

    // ==================== 2. Basic Operations for Sequential Stack ====================

    // Initialize sequential stack
    inline void InitStack(SqStack &S) {
        S.top = -1;
    }

    // Check if sequential stack is empty
    inline bool StackEmpty(const SqStack &S) {
        return S.top == -1;
    }

    // Push element into sequential stack
    inline bool Push(SqStack &S, ElemType x) {
        if (S.top == MaxSize - 1) { // Stack overflow
            return false;
        }
        S.data[++S.top] = x;
        return true;
    }

    // Pop element from sequential stack
    inline bool Pop(SqStack &S, ElemType &x) {
        if (S.top == -1) { // Stack underflow
            return false;
        }
        x = S.data[S.top--];
        return true;
    }

    // Get top element of sequential stack
    inline bool GetTop(const SqStack &S, ElemType &x) {
        if (S.top == -1) { // Stack empty
            return false;
        }
        x = S.data[S.top];
        return true;
    }

    // Destroy sequential stack (statically allocated memory, simply reset the pointer)
    inline void DestroyStack(SqStack &S) {
        S.top = -1;
    }

    // ==================== 3. Basic Operations for Linked Stack ====================

    // Initialize linked stack
    inline void InitStack(LiStack &S) {
        S = nullptr;
    }

    // Check if linked stack is empty
    inline bool StackEmpty(const LiStack &S) {
        return S == nullptr;
    }

    // Push element into linked stack (head insertion)
    inline bool Push(LiStack &S, ElemType x) {
        LinkNode *p = new (std::nothrow) LinkNode;
        if (!p) { // Memory allocation failed
            return false;
        }
        p->data = x;
        p->next = S;
        S = p;
        return true;
    }

    // Pop element from linked stack
    inline bool Pop(LiStack &S, ElemType &x) {
        if (S == nullptr) { // Stack empty
            return false;
        }
        LinkNode *p = S;
        x = p->data;
        S = S->next;
        delete p;
        return true;
    }

    // Get top element of linked stack
    inline bool GetTop(const LiStack &S, ElemType &x) {
        if (S == nullptr) { // Stack empty
            return false;
        }
        x = S->data;
        return true;
    }

    // Destroy linked stack (free all dynamically allocated nodes)
    inline void DestroyStack(LiStack &S) {
        LinkNode *p = S;
        while (p != nullptr) {
            LinkNode *temp = p;
            p = p->next;
            delete temp;
        }
        S = nullptr;
    }
}

#endif // STACK_C_H