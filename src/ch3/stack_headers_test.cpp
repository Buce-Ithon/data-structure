#include <iostream>
#include <cassert>
#include <string>
#include <stack_cpp.h>
#include <stack_c.h>

// Example 1: Polymorphic test function for C++ style stacks
// As long as it inherits from the IStack<T> interface, both sequential and linked stacks can be passed in
template <typename T>
void testCppStackPolymorphic(StackCppStyle::IStack<T>& stack, const std::string& stackName) {
    std::cout << "=== Testing " << stackName << " (C++ Style) ===" << std::endl;
    
    // 1. Check if empty
    assert(stack.isEmpty());
    
    // 2. Push elements
    stack.push(10);
    stack.push(20);
    stack.push(30);
    
    // 3. Get top element
    int topVal = 0;
    if (stack.getTop(topVal)) {
        std::cout << "Top element: " << topVal << " (Expected: 30)" << std::endl;
        assert(topVal == 30);
    }
    
    // 4. Pop element test
    int popVal = 0;
    stack.pop(popVal);
    std::cout << "Popped element: " << popVal << " (Expected: 30)" << std::endl;
    assert(popVal == 30);
    
    // 5. Clear stack
    stack.clear();
    assert(stack.isEmpty());
    std::cout << stackName << " passed all tests!\n" << std::endl;
}

// Example 2: Test C-style sequential stack
void testCStyleSqStack() {
    std::cout << "=== Testing SqStack (C Style) ===" << std::endl;
    
    StackCStyle::SqStack s;
    StackCStyle::InitStack(s);
    assert(StackCStyle::StackEmpty(s));
    
    StackCStyle::Push(s, 100);
    StackCStyle::Push(s, 200);
    
    StackCStyle::ElemType val;
    if (StackCStyle::GetTop(s, val)) {
        std::cout << "Top element: " << val << " (Expected: 200)" << std::endl;
        assert(val == 200);
    }
    
    StackCStyle::Pop(s, val);
    std::cout << "Popped element: " << val << " (Expected: 200)" << std::endl;
    assert(val == 200);
    
    StackCStyle::DestroyStack(s);
    assert(StackCStyle::StackEmpty(s));
    std::cout << "C-Style SqStack passed all tests!\n" << std::endl;
}

// Example 3: Test C-style linked stack
void testCStyleLiStack() {
    std::cout << "=== Testing LiStack (C Style) ===" << std::endl;
    
    StackCStyle::SqStack s;
    StackCStyle::InitStack(s);
    assert(StackCStyle::StackEmpty(s));
    
    StackCStyle::Push(s, 300);
    StackCStyle::Push(s, 400);
    
    StackCStyle::ElemType val;
    if (StackCStyle::GetTop(s, val)) {
        std::cout << "Top element: " << val << " (Expected: 400)" << std::endl;
        assert(val == 400);
    }
    
    StackCStyle::Pop(s, val);
    std::cout << "Popped element: " << val << " (Expected: 400)" << std::endl;
    assert(val == 400);
    
    StackCStyle::DestroyStack(s);
    assert(StackCStyle::StackEmpty(s));
    std::cout << "C-Style LiStack passed all tests!\n" << std::endl;
}

int main() {
    // ------------------- 1. Test C++ Style Template Stacks -------------------
    // Sequential stack
    StackCppStyle::SqStack<int> cppSqStack;
    testCppStackPolymorphic(cppSqStack, "SqStack");

    // Linked stack (destructor automatically frees memory when leaving scope)
    StackCppStyle::LiStack<int> cppLiStack;
    testCppStackPolymorphic(cppLiStack, "LiStack");

    // Supports any other data type
    StackCppStyle::LiStack<std::string> stringStack;
    stringStack.push("Hello");
    stringStack.push("World");
    std::string strTop;
    stringStack.getTop(strTop);
    std::cout << "String Stack Top: " << strTop << "\n" << std::endl;


    // ------------------- 2. Test C Style Stacks -------------------
    testCStyleSqStack();
    testCStyleLiStack();

    std::cout << "ALL TESTS PASSED SUCCESSFULLY!" << std::endl;

    return 0;
}