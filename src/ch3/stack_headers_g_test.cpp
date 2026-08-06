#include <gtest/gtest.h>
#include "stack_cpp.h"

using namespace StackCppStyle;

// ==================== 1. 基础测试 (TEST 宏) ====================
TEST(SqStackBasicTest, PushPopOperation) {
    SqStack<int> stack;

    // 断言初始状态为空
    EXPECT_TRUE(stack.isEmpty());

    // 测试压栈与取栈顶
    EXPECT_TRUE(stack.push(100));
    int topVal = 0;
    EXPECT_TRUE(stack.getTop(topVal));
    EXPECT_EQ(topVal, 100);

    // 测试出栈
    int popVal = 0;
    EXPECT_TRUE(stack.pop(popVal));
    EXPECT_EQ(popVal, 100);
    EXPECT_TRUE(stack.isEmpty());
}

// ==================== 2. 使用 Fixture 测试边界条件 ====================
class LiStackTest : public ::testing::Test {
protected:
    LiStack<int> stack;

    // 每个 TEST_F 执行前调用
    void SetUp() override {
        stack.push(1);
        stack.push(2);
        stack.push(3);
    }
};

TEST_F(LiStackTest, ClearStack) {
    EXPECT_FALSE(stack.isEmpty());
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(LiStackTest, PopOrder) {
    int val = 0;
    stack.pop(val);
    EXPECT_EQ(val, 3); // 后进先出
    stack.pop(val);
    EXPECT_EQ(val, 2);
}

// ==================== 3. 主测试入口 ====================
int main(int argc, char **argv) {
    // 初始化 GoogleTest 框架并解析命令行参数
    ::testing::InitGoogleTest(&argc, argv);
    
    // 自动运行所有被注册的 TEST 和 TEST_F 宏
    return RUN_ALL_TESTS();
}