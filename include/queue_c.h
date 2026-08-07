#ifndef QUEUE_C_H
#define QUEUE_C_H

#include <new>

namespace QueueCStyle {
    // Define element type, modify it as needed
    typedef int ElemType;
    
    // Replace the original `#define MaxSize 128` to prevent macros from polluting the global scope.
    constexpr int MaxSize = 128;
    
    // ==================== 1. Data Structure Definitions ====================

    // Sequential queue structure
    // Circular queue
    typedef struct {
        ElemType data[MaxSize];
        int front, rear; // Head pointer -> front; tail pointer -> rear
    } SqQueue;
    
    // Linked queue node and pointer definition (implemented using a singly linked list with a head node)
    typedef struct LinkNode{
        ElemType data;
        struct LinkNode *next;
    } LinkNode;
    typedef struct {
        LinkNode *front, *rear; // Head pointer -> front; tail pointer -> rear
    } LinkQueue;
    
    // ==================== 2. Basic Operations for Sequential queue ====================

    // Initialize sequential queue
    inline void InitQueue(SqQueue &Q) {
        Q.front = 0;
        Q.rear = 0; 
    }
    
    // Check if sequential queue is Empty
    inline bool QueueEmpty(SqQueue Q) {
        if (Q.front == Q.rear) {
            return true;
        }
        else {
            return false;
        }
    }
    
    // Enqueue element into sequential queue
    inline bool EnQueue(SqQueue &Q, ElemType x) {
        if (Q.front == (Q.rear + 1) % MaxSize) { // Queue overflow
            return false;
        }
        Q.data[Q.rear] = x;
        Q.rear = (Q.rear + 1) % MaxSize;
        return true;
    }
    
    // Dequeue element from sequential queue
    inline bool DeQueue(SqQueue &Q, ElemType &x) {
        if (Q.front == Q.rear) return false; // Queue empty
        x = Q.data[Q.front];
        Q.front = (Q.front + 1) % MaxSize;
        return true;
    }
    
    // ==================== 3. Basic Operations for Linked Queue ====================

    // Initialize linked queue
    inline void InitQueue(LinkQueue &Q) {
        Q.front = new LinkNode();
        Q.rear = new LinkNode();
        Q.front->next = nullptr;
    }
    
    // Check if linked queue is empty
    inline bool QueueEmpty(LinkQueue Q) {
        if (Q.front == Q.rear) {
            return true;
        }
        else {
            return false;
        }
    }
    
    // Enqueue element into linked queue
    inline bool EnQueue(LinkQueue &Q, ElemType x) {
        LinkNode *s = new (std::nothrow) LinkNode();
        if (!s) return false;
        s->data = x;
        s->next = nullptr;
        Q.rear->next = s;
        Q.rear = s;
        return true;
    }
    
    // Dequeue element into linked queue
    inline bool DeQueue(LinkQueue &Q, ElemType &x) {
        if (Q.front == Q.rear) return false;
        LinkNode *p = Q.front->next;
        x = p->next->data;
        Q.front->next = p->next;
        
        // Only one element in Q
        if (Q.rear == p) {
            Q.rear = Q.front;
        }
        
        delete p;
        return true;
    }
}

#endif // QUEUE_C_H