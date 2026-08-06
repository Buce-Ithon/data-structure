#ifndef STACK_CPP_H
#define STACK_CPP_H

#include <new>

namespace StackCppStyle{
    // ==================== 1. Abstract Base Interface ====================
    // Unifies common stack operations for any data type T using template polymorphism.
    template <typename T>
    class IStack {
    public:
        virtual ~IStack() = default;

        virtual bool isEmpty() const = 0;
        virtual bool push(const T &element) = 0;
        virtual bool pop(T &element) = 0;
        virtual bool getTop(T &element) const = 0;
        virtual void clear() = 0;
    };

    // ==================== 2. Sequential Stack Implementation ====================
    // Fixed-size array stack implementation.
    template <typename T, std::size_t MaxSize = 128>
    class SqStack : public IStack<T> {
    private:
        T data[MaxSize];
        int topIndex;

    public:
        // Constructor initializes an empty stack
        SqStack() : topIndex(-1) {}

        // Destructor
        ~SqStack() override = default;

        // Check if the stack is empty
        bool isEmpty() const override {
            return topIndex == -1;
        }

        // Push an element onto the stack
        bool push(const T &element) override {
            if (topIndex >= static_cast<int>(MaxSize) - 1) {
                return false; // Stack overflow
            }
            data[++topIndex] = element;
            return true;
        }

        // Pop an element from the stack
        bool pop(T &element) override {
            if (isEmpty()) {
                return false; // Stack underflow
            }
            element = data[topIndex--];
            return true;
        }

        // Get the top element without removing it
        bool getTop(T &element) const override {
            if (isEmpty()) {
                return false;
            }
            element = data[topIndex];
            return true;
        }

        // Reset the stack index (clears the stack)
        void clear() override {
            topIndex = -1;
        }
    };

    // ==================== 3. Linked Stack Implementation ====================
    // Singly-linked node stack implementation.
    template <typename T>
    class LiStack : public IStack<T> {
    private:
        struct LinkNode {
            T data;
            LinkNode *next;
            LinkNode(const T &val, LinkNode *n = nullptr) : data(val), next(n) {}
        };

        LinkNode *topNode;

    public:
        // Constructor initializes an empty linked stack
        LiStack() : topNode(nullptr) {}

        // Destructor frees all remaining node memory via clear()
        ~LiStack() override {
            clear();
        }

        // Check if the stack is empty
        bool isEmpty() const override {
            return topNode == nullptr;
        }

        // Push an element onto the stack (head insertion)
        bool push(const T &element) override {
            LinkNode *newNode = new (std::nothrow) LinkNode(element, topNode);
            if (!newNode) {
                return false; // Allocation failed
            }
            topNode = newNode;
            return true;
        }

        // Pop an element from the stack
        bool pop(T &element) override {
            if (isEmpty()) {
                return false;
            }
            LinkNode *temp = topNode;
            element = temp->data;
            topNode = topNode->next;
            delete temp;
            return true;
        }

        // Get the top element without removing it
        bool getTop(T &element) const override {
            if (isEmpty()) {
                return false;
            }
            element = topNode->data;
            return true;
        }

        // Delete all nodes and reset top pointer
        void clear() override {
            while (topNode != nullptr) {
                LinkNode *temp = topNode;
                topNode = topNode->next;
                delete temp;
            }
        }
    };
}

#endif // STACK_CPP_H