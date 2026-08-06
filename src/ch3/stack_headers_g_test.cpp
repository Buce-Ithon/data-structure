#include <gtest/gtest.h>
#include <string>

#include "stack_c.h"
#include "stack_cpp.h"

// ============================================================================
// 1. Tests for C-Style Sequential Stack (StackCStyle::SqStack)
// ============================================================================

TEST(CStyleSqStackTest, BasicLifecycleAndOperations) {
    StackCStyle::SqStack stack;
    StackCStyle::InitStack(stack);

    // Initial state check
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));

    // Underflow tests on empty stack
    StackCStyle::ElemType val = 0;
    EXPECT_FALSE(StackCStyle::GetTop(stack, val));
    EXPECT_FALSE(StackCStyle::Pop(stack, val));

    // Push elements
    EXPECT_TRUE(StackCStyle::Push(stack, 10));
    EXPECT_TRUE(StackCStyle::Push(stack, 20));
    EXPECT_FALSE(StackCStyle::StackEmpty(stack));

    // Get top element
    EXPECT_TRUE(StackCStyle::GetTop(stack, val));
    EXPECT_EQ(val, 20);

    // Pop element (LIFO)
    EXPECT_TRUE(StackCStyle::Pop(stack, val));
    EXPECT_EQ(val, 20);

    EXPECT_TRUE(StackCStyle::GetTop(stack, val));
    EXPECT_EQ(val, 10);

    // Destroy stack
    StackCStyle::DestroyStack(stack);
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));
}

TEST(CStyleSqStackTest, StackOverflowBoundary) {
    StackCStyle::SqStack stack;
    StackCStyle::InitStack(stack);

    // Fill stack up to MaxSize (128)
    for (int i = 0; i < StackCStyle::MaxSize; ++i) {
        EXPECT_TRUE(StackCStyle::Push(stack, i));
    }

    // 129th push should fail due to overflow
    EXPECT_FALSE(StackCStyle::Push(stack, 999));

    // Verify top element is still the last valid pushed item
    StackCStyle::ElemType topVal = 0;
    EXPECT_TRUE(StackCStyle::GetTop(stack, topVal));
    EXPECT_EQ(topVal, StackCStyle::MaxSize - 1);

    StackCStyle::DestroyStack(stack);
}

// ============================================================================
// 2. Tests for C-Style Linked Stack (StackCStyle::LiStack)
// ============================================================================

TEST(CStyleLiStackTest, BasicLifecycleAndOperations) {
    StackCStyle::LiStack stack;
    StackCStyle::InitStack(stack);

    // Initial state check
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));

    // Underflow tests on empty stack
    StackCStyle::ElemType val = 0;
    EXPECT_FALSE(StackCStyle::GetTop(stack, val));
    EXPECT_FALSE(StackCStyle::Pop(stack, val));

    // Push elements (head insertion)
    EXPECT_TRUE(StackCStyle::Push(stack, 100));
    EXPECT_TRUE(StackCStyle::Push(stack, 200));
    EXPECT_TRUE(StackCStyle::Push(stack, 300));
    EXPECT_FALSE(StackCStyle::StackEmpty(stack));

    // Get top element
    EXPECT_TRUE(StackCStyle::GetTop(stack, val));
    EXPECT_EQ(val, 300);

    // Pop elements sequentially
    EXPECT_TRUE(StackCStyle::Pop(stack, val));
    EXPECT_EQ(val, 300);

    EXPECT_TRUE(StackCStyle::Pop(stack, val));
    EXPECT_EQ(val, 200);

    EXPECT_TRUE(StackCStyle::Pop(stack, val));
    EXPECT_EQ(val, 100);

    // Stack should now be empty
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));

    // Destroy linked stack
    StackCStyle::DestroyStack(stack);
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));
}

TEST(CStyleLiStackTest, DestroyNonEmptyStack) {
    StackCStyle::LiStack stack;
    StackCStyle::InitStack(stack);

    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(StackCStyle::Push(stack, i));
    }

    // DestroyStack should free all allocated nodes without memory leaks
    StackCStyle::DestroyStack(stack);
    EXPECT_TRUE(StackCStyle::StackEmpty(stack));
}

// ============================================================================
// 3. Tests for C++ Style Sequential Stack (StackCppStyle::SqStack)
// ============================================================================

TEST(CppStyleSqStackTest, BasicOperationsAndClear) {
    StackCppStyle::SqStack<int> stack;

    EXPECT_TRUE(stack.isEmpty());

    // Underflow check
    int val = 0;
    EXPECT_FALSE(stack.getTop(val));
    EXPECT_FALSE(stack.pop(val));

    // Push operations
    EXPECT_TRUE(stack.push(11));
    EXPECT_TRUE(stack.push(22));
    EXPECT_FALSE(stack.isEmpty());

    // Get top & Pop
    EXPECT_TRUE(stack.getTop(val));
    EXPECT_EQ(val, 22);

    EXPECT_TRUE(stack.pop(val));
    EXPECT_EQ(val, 22);

    // Clear operation
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_FALSE(stack.getTop(val));
}

TEST(CppStyleSqStackTest, CapacityBoundaryOverflow) {
    // Instantiate a small capacity stack to test overflow boundary
    constexpr std::size_t CustomCapacity = 3;
    StackCppStyle::SqStack<int, CustomCapacity> smallStack;

    EXPECT_TRUE(smallStack.push(1));
    EXPECT_TRUE(smallStack.push(2));
    EXPECT_TRUE(smallStack.push(3));

    // Stack is now full (3/3), next push must fail
    EXPECT_FALSE(smallStack.push(4));

    int topVal = 0;
    EXPECT_TRUE(smallStack.getTop(topVal));
    EXPECT_EQ(topVal, 3);
}

// ============================================================================
// 4. Tests for C++ Style Linked Stack (StackCppStyle::LiStack)
// ============================================================================

TEST(CppStyleLiStackTest, BasicOperationsAndClear) {
    StackCppStyle::LiStack<int> stack;

    EXPECT_TRUE(stack.isEmpty());

    // Underflow check
    int val = 0;
    EXPECT_FALSE(stack.getTop(val));
    EXPECT_FALSE(stack.pop(val));

    // Push elements
    EXPECT_TRUE(stack.push(1000));
    EXPECT_TRUE(stack.push(2000));
    EXPECT_FALSE(stack.isEmpty());

    EXPECT_TRUE(stack.getTop(val));
    EXPECT_EQ(val, 2000);

    EXPECT_TRUE(stack.pop(val));
    EXPECT_EQ(val, 2000);

    // Clear operation
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(CppStyleLiStackTest, CustomDataTypeSupport) {
    // Verify template support for non-primitive types (std::string)
    StackCppStyle::LiStack<std::string> stringStack;

    EXPECT_TRUE(stringStack.push("Hello"));
    EXPECT_TRUE(stringStack.push("World"));

    std::string strVal;
    EXPECT_TRUE(stringStack.getTop(strVal));
    EXPECT_EQ(strVal, "World");

    EXPECT_TRUE(stringStack.pop(strVal));
    EXPECT_EQ(strVal, "World");

    EXPECT_TRUE(stringStack.getTop(strVal));
    EXPECT_EQ(strVal, "Hello");
}

// ============================================================================
// 5. Polymorphic Interface Test (IStack<T>)
// ============================================================================

// Helper function to test any implementation of IStack polymorphically
void VerifyStackInterface(StackCppStyle::IStack<int>& stack) {
    EXPECT_TRUE(stack.isEmpty());

    EXPECT_TRUE(stack.push(5));
    EXPECT_TRUE(stack.push(10));

    int val = 0;
    EXPECT_TRUE(stack.getTop(val));
    EXPECT_EQ(val, 10);

    EXPECT_TRUE(stack.pop(val));
    EXPECT_EQ(val, 10);

    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(CppStylePolymorphismTest, UnifiedInterfaceUsage) {
    // Test SqStack via interface reference
    StackCppStyle::SqStack<int> sqStack;
    VerifyStackInterface(sqStack);

    // Test LiStack via interface reference
    StackCppStyle::LiStack<int> liStack;
    VerifyStackInterface(liStack);
}

// ============================================================================
// 6. Main Entry Point
// ============================================================================

int main(int argc, char **argv) {
    // Initialize GoogleTest framework and parse command-line arguments
    ::testing::InitGoogleTest(&argc, argv);

    // Run all registered tests automatically
    return RUN_ALL_TESTS();
}