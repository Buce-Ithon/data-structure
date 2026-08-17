#include <queue_c.h>
#include <iostream>
#include <stack_c.h>

/**
 * @struct TagSqQueue
 * @brief Application 1: Circular queue structure using a status flag domain (tag).
 * 
 * Distinguishes the queue status ("empty" vs "full") when front == rear
 * by using the tag value (0 for empty, 1 for full), allowing all MaxSize slots
 * in the array to be fully utilized.
 */
typedef struct {
    QueueCStyle::ElemType data[QueueCStyle::MaxSize]; /**< Array storing queue elements */
    int front; /**< Head pointer */
    int rear;  /**< Tail pointer */
    int tag;   /**< Status flag: 0 indicates the last operation was Dequeue (empty on front==rear), 1 indicates Enqueue (full on front==rear) */
} TagSqQueue;

/**
 * @brief Initializes the circular queue with a tag flag domain.
 * @param Q Reference to the circular queue to be initialized.
 */
void InitQueue_Tag(TagSqQueue &Q) {
    Q.front = 0;
    Q.rear = 0;
    Q.tag = 0; // Initial state is empty
}

/**
 * @brief Enqueues an element into the circular queue with a tag flag.
 * @param Q Reference to the circular queue.
 * @param x The element to be enqueued.
 * @return bool Returns true if enqueued successfully; returns false if the queue is full.
 */
bool EnQueue_Tag(TagSqQueue &Q, QueueCStyle::ElemType x) {
    // Queue full condition: front and rear pointers coincide and tag is 1
    if (Q.front == Q.rear && Q.tag == 1) {
        return false;
    }
    Q.data[Q.rear] = x;
    Q.rear = (Q.rear + 1) % QueueCStyle::MaxSize;
    Q.tag = 1; // Mark last operation as Enqueue
    return true;
}

/**
 * @brief Dequeues an element from the circular queue with a tag flag.
 * @param Q Reference to the circular queue.
 * @param x Reference variable to store the dequeued element.
 * @return bool Returns true if dequeued successfully; returns false if the queue is empty.
 */
bool DeQueue_Tag(TagSqQueue &Q, QueueCStyle::ElemType &x) {
    // Queue empty condition: front and rear pointers coincide and tag is 0
    if (Q.front == Q.rear && Q.tag == 0) {
        return false;
    }
    x = Q.data[Q.front];
    Q.front = (Q.front + 1) % QueueCStyle::MaxSize;
    Q.tag = 0; // Mark last operation as Dequeue
    return true;
}

/**
 * @brief Application Q2: Reverses the order of elements in a sequential queue using an auxiliary sequential stack.
 * 
 * Transfers all elements from queue Q to stack S (LIFO order), and then pushes 
 * them back into Q, resulting in a fully reversed queue.
 * 
 * @param Q Reference to the sequential queue to be reversed.
 * @param S Reference to the auxiliary sequential stack (assumed empty initially).
 */
void ReverseQueue(QueueCStyle::SqQueue &Q, StackCStyle::SqStack &S) {
    QueueCStyle::ElemType temp;

    // Step 1: Dequeue each element from Q and push it onto S
    while (!QueueCStyle::QueueEmpty(Q)) {
        QueueCStyle::DeQueue(Q, temp);
        StackCStyle::Push(S, temp);
    }

    // Step 2: Pop each element from S and enqueue it back into Q
    while (!StackCStyle::StackEmpty(S)) {
        StackCStyle::Pop(S, temp);
        QueueCStyle::EnQueue(Q, temp);
    }
}

/**
 * @struct StackQueue
 * @brief Application 3: Queue simulated using two sequential stacks S1 and S2.
 * 
 * Stack S1 acts as the input buffer for enqueue operations, while Stack S2
 * acts as the output buffer for dequeue operations.
 */
typedef struct {
    StackCStyle::SqStack S1; /**< Input stack for enqueueing elements */
    StackCStyle::SqStack S2; /**< Output stack for dequeueing elements */
} StackQueue;

/**
 * @brief Initializes the queue simulated by two stacks.
 * @param Q Reference to the two-stack queue structure to be initialized.
 */
void InitQueue_Stack(StackQueue &Q) {
    StackCStyle::InitStack(Q.S1);
    StackCStyle::InitStack(Q.S2);
}

/**
 * @brief Checks if the two-stack queue is empty.
 * @param Q Reference to the two-stack queue structure.
 * @return bool Returns true if both S1 and S2 are empty; returns false otherwise.
 */
bool QueueEmpty_Stack(const StackQueue &Q) {
    return StackCStyle::StackEmpty(Q.S1) && StackCStyle::StackEmpty(Q.S2);
}

/**
 * @brief Enqueues an element into the two-stack queue.
 * 
 * If S1 is full and S2 is empty, all elements in S1 are popped and pushed into S2 first.
 * If S1 is full and S2 is not empty, the queue is full.
 * 
 * @param Q Reference to the two-stack queue structure.
 * @param x The element to be enqueued.
 * @return bool Returns true if enqueued successfully; returns false if the queue is full.
 */
bool EnQueue_Stack(StackQueue &Q, StackCStyle::ElemType x) {
    if (Q.S1.top == StackCStyle::MaxSize - 1) {
        if (StackCStyle::StackEmpty(Q.S2)) {
            StackCStyle::ElemType temp;
            while (!StackCStyle::StackEmpty(Q.S1)) {
                StackCStyle::Pop(Q.S1, temp);
                StackCStyle::Push(Q.S2, temp);
            }
        } else {
            return false; // Queue overflow
        }
    }
    return StackCStyle::Push(Q.S1, x);
}

/**
 * @brief Dequeues an element from the two-stack queue.
 * 
 * If S2 is empty, transfers all elements from S1 into S2 before popping.
 * If both S1 and S2 are empty, the queue is empty.
 * 
 * @param Q Reference to the two-stack queue structure.
 * @param x Reference variable to store the dequeued element.
 * @return bool Returns true if dequeued successfully; returns false if the queue is empty.
 */
bool DeQueue_Stack(StackQueue &Q, StackCStyle::ElemType &x) {
    if (StackCStyle::StackEmpty(Q.S2)) {
        if (StackCStyle::StackEmpty(Q.S1)) {
            return false; // Queue underflow (empty)
        }
        StackCStyle::ElemType temp;
        while (!StackCStyle::StackEmpty(Q.S1)) {
            StackCStyle::Pop(Q.S1, temp);
            StackCStyle::Push(Q.S2, temp);
        }
    }
    return StackCStyle::Pop(Q.S2, x);
}

/**
 * @struct DynQueueNode
 * @brief Prepare for Application Q4: Node for the auto-expanding circular linked list queue.
 */
typedef struct DynQueueNode {
    QueueCStyle::ElemType data; /**< Data payload */
    struct DynQueueNode *next;  /**< Pointer to next node */
} DynQueueNode;

/**
 * @struct DynQueue
 * @brief Application Q4: Auto-expanding queue using a self-recycling circular linked list.
 * 
 * Guarantees O(1) time complexity for both Enqueue and Dequeue while allowing space to expand
 * dynamically when full, and reusing dequeued nodes without releasing allocated memory.
 */
typedef struct {
    DynQueueNode *front; /**< Head pointer (points to current front node) */
    DynQueueNode *rear;  /**< Tail pointer (points to current rear node) */
} DynQueue;

/**
 * @brief Initializes the dynamic self-expanding circular queue.
 * @param Q Reference to the dynamic queue structure.
 */
void InitQueue_Dyn(DynQueue &Q) {
    DynQueueNode *dummy = new DynQueueNode();
    dummy->next = dummy; // Self-referential single-node cycle
    Q.front = dummy;
    Q.rear = dummy;
}

/**
 * @brief Checks if the dynamic circular queue is empty.
 * @param Q Reference to the dynamic queue structure.
 * @return bool Returns true if front == rear; returns false otherwise.
 */
bool QueueEmpty_Dyn(const DynQueue &Q) {
    return Q.front == Q.rear;
}

/**
 * @brief Enqueues an element into the dynamic circular queue in O(1) time.
 * 
 * If no unused node is available (rear->next == front), a new node is allocated and inserted.
 * Otherwise, an existing unused node from previous dequeues is reused.
 * 
 * @param Q Reference to the dynamic queue structure.
 * @param x The element to be enqueued.
 * @return bool Returns true if enqueued successfully; returns false if memory allocation fails.
 */
bool EnQueue_Dyn(DynQueue &Q, QueueCStyle::ElemType x) {
    if (Q.rear->next == Q.front) { // Dynamic Expansion Required
        DynQueueNode *s = new (std::nothrow) DynQueueNode();
        if (!s) return false;
        s->next = Q.front;
        Q.rear->next = s;
    }
    Q.rear = Q.rear->next;
    Q.rear->data = x;
    return true;
}

/**
 * @brief Dequeues an element from the dynamic circular queue in O(1) time.
 * 
 * Advances the front pointer without deleting nodes, allowing allocated space to be reused.
 * 
 * @param Q Reference to the dynamic queue structure.
 * @param x Reference variable to store the dequeued element.
 * @return bool Returns true if dequeued successfully; returns false if the queue is empty.
 */
bool DeQueue_Dyn(DynQueue &Q, QueueCStyle::ElemType &x) {
    if (Q.front == Q.rear) {
        return false; // Queue empty
    }
    Q.front = Q.front->next;
    x = Q.front->data;
    return true;
}

int main() {
    // Application Q1
    std::cout << "===== Application Q1 =====" << std::endl;

    TagSqQueue Q;
    InitQueue_Tag(Q);

    // Basic operation test
    QueueCStyle::ElemType val;

    if (EnQueue_Tag(Q, 100)) {
        std::cout << "Successfully enqueued: 100" << std::endl;
    }

    if (DeQueue_Tag(Q, val)) {
        std::cout << "Successfully dequeued: " << val << std::endl;
    }

    // Empty test
    if (!DeQueue_Tag(Q, val)) {
        std::cout << "Queue is empty, dequeue failed as expected." << std::endl;
    }
    
    // Application Q2
    std::cout << "===== Application Q2 =====" << std::endl;

    QueueCStyle::SqQueue Q2;
    StackCStyle::SqStack S;
    QueueCStyle::InitQueue(Q2);
    StackCStyle::InitStack(S);

    // Populate queue with initial elements: [10, 20, 30, 40]
    std::cout << "Original queue order: ";
    for (int i = 10; i <= 40; i += 10) {
        QueueCStyle::EnQueue(Q2, i);
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Reverse the queue elements
    ReverseQueue(Q2, S);

    // Display reversed elements: [40, 30, 20, 10]
    std::cout << "Reversed queue order: ";
    while (!QueueCStyle::QueueEmpty(Q2)) {
        QueueCStyle::DeQueue(Q2, val);
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Application Q3
    std::cout << "===== Application Q3 =====" << std::endl;

    StackQueue sq;
    InitQueue_Stack(sq);

    std::cout << "Is queue empty? " << (QueueEmpty_Stack(sq) ? "Yes" : "No") << std::endl;

    // Enqueue elements: 1, 2, 3
    std::cout << "Enqueueing: 1, 2, 3" << std::endl;
    EnQueue_Stack(sq, 1);
    EnQueue_Stack(sq, 2);
    EnQueue_Stack(sq, 3);

    std::cout << "Is queue empty? " << (QueueEmpty_Stack(sq) ? "Yes" : "No") << std::endl;

    // Dequeue elements to verify FIFO behavior
    while (!QueueEmpty_Stack(sq)) {
        DeQueue_Stack(sq, val);
        std::cout << "Dequeued: " << val << std::endl;
    }
    
    std::cout << "Is queue empty? " << (QueueEmpty_Stack(sq) ? "Yes" : "No") << std::endl;
    
    // Application Q4
    std::cout << "===== Application Q4 =====" << std::endl;

    DynQueue dq;
    InitQueue_Dyn(dq);

    std::cout << "Is dynamic queue empty? " << (QueueEmpty_Dyn(dq) ? "Yes" : "No") << std::endl;

    // Enqueue 100, 200 (Expands linked list nodes)
    EnQueue_Dyn(dq, 100);
    EnQueue_Dyn(dq, 200);

    // Dequeue 100 (Node stays in the cycle for recycling)
    DeQueue_Dyn(dq, val);
    std::cout << "Dequeued (space recycled): " << val << std::endl;

    // Enqueue 300 (Reuses recycled space without new allocation)
    EnQueue_Dyn(dq, 300);

    // Drain remaining elements
    while (!QueueEmpty_Dyn(dq)) {
        DeQueue_Dyn(dq, val);
        std::cout << "Dequeued: " << val << std::endl;
    }

    return 0;
}