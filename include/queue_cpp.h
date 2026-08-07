#ifndef QUEUE_CPP_H
#define QUEUE_CPP_H

#include <new>
#include <cstddef>

namespace QueueCppStyle {
    // ==================== 1. Abstract Base Interface ====================
    // Unifies common queue operations for any data type T using template polymorphism.
    template <typename T>
    class IQueue {
    public:
        virtual ~IQueue() = default;

        virtual bool isEmpty() const = 0;
        virtual bool enqueue(const T &element) = 0; // Enqueue element
        virtual bool dequeue(T &element) = 0;       // Dequeue element
        virtual bool front(T &element) const = 0;   // Get front element without removing
        virtual void clear() = 0;
    };

    // ==================== 2. Sequential Queue Implementation ====================
    // Fixed-size circular array queue implementation.
    template <typename T, std::size_t MaxSize = 128>
    class SqQueue : public IQueue<T> {
    private:
        T data[MaxSize];
        int frontIdx; // Head pointer -> frontIdx
        int rearIdx;  // Tail pointer -> rearIdx

    public:
        // Constructor initializes an empty circular queue
        SqQueue() : frontIdx(0), rearIdx(0) {}

        // Destructor
        ~SqQueue() override = default;

        // Check if the queue is empty
        bool isEmpty() const override {
            return frontIdx == rearIdx;
        }

        // Check if the queue is full
        bool isFull() const {
            return frontIdx == (rearIdx + 1) % static_cast<int>(MaxSize);
        }

        // Enqueue an element
        bool enqueue(const T &element) override {
            if (isFull()) {
                return false; // Queue overflow
            }
            data[rearIdx] = element;
            rearIdx = (rearIdx + 1) % static_cast<int>(MaxSize);
            return true;
        }

        // Dequeue an element
        bool dequeue(T &element) override {
            if (isEmpty()) {
                return false; // Queue underflow
            }
            element = data[frontIdx];
            frontIdx = (frontIdx + 1) % static_cast<int>(MaxSize);
            return true;
        }

        // Get the front element without removing it
        bool front(T &element) const override {
            if (isEmpty()) {
                return false;
            }
            element = data[frontIdx];
            return true;
        }

        // Reset the front and rear pointers (clears the queue)
        void clear() override {
            frontIdx = 0;
            rearIdx = 0;
        }
    };

    // ==================== 3. Linked Queue Implementation ====================
    // Singly-linked list queue implementation with a dummy head node.
    template <typename T>
    class LinkQueue : public IQueue<T> {
    private:
        struct LinkNode {
            T data;
            LinkNode *next;

            // Default constructor (used for the dummy head node)
            LinkNode() : data(), next(nullptr) {}

            // Value constructor
            LinkNode(const T &val, LinkNode *n = nullptr) : data(val), next(n) {}
        };

        LinkNode *frontNode; // Points to the dummy head node
        LinkNode *rearNode;  // Points to the last node in the queue

    public:
        // Constructor creates the dummy head node
        LinkQueue() {
            frontNode = rearNode = new LinkNode();
        }

        // Destructor frees all nodes (including dummy head node) via clear()
        ~LinkQueue() override {
            clear();
            delete frontNode; // Delete dummy head node
            frontNode = rearNode = nullptr;
        }

        // Explicitly disable copy constructor and assignment operator
        LinkQueue(const LinkQueue &) = delete;
        LinkQueue &operator=(const LinkQueue &) = delete;

        // Check if the queue is empty
        bool isEmpty() const override {
            return frontNode == rearNode;
        }

        // Enqueue an element (insert at tail)
        bool enqueue(const T &element) override {
            LinkNode *newNode = new (std::nothrow) LinkNode(element, nullptr);
            if (!newNode) {
                return false; // Allocation failed
            }
            rearNode->next = newNode;
            rearNode = newNode;
            return true;
        }

        // Dequeue an element (remove from head)
        bool dequeue(T &element) override {
            if (isEmpty()) {
                return false;
            }
            LinkNode *p = frontNode->next; // First real node
            element = p->data;
            frontNode->next = p->next;

            // If deleting the last element, update rearNode back to dummy head
            if (rearNode == p) {
                rearNode = frontNode;
            }

            delete p;
            return true;
        }

        // Get the front element without removing it
        bool front(T &element) const override {
            if (isEmpty()) {
                return false;
            }
            element = frontNode->next->data;
            return true;
        }

        // Clear all elements (retains the dummy head node)
        void clear() override {
            while (frontNode->next != nullptr) {
                LinkNode *p = frontNode->next;
                frontNode->next = p->next;
                delete p;
            }
            rearNode = frontNode;
        }
    };
}

#endif // QUEUE_CPP_H