#include <gtest/gtest.h>

#include "examples/gtest_demo.h"

class CalculatorTest : public ::testing::Test {
protected:
    // 如果需要在每个测试之前/之后执行的代码
    void SetUp() override {
        // 初始化代码，如分配内存或打开文件
    }

    void TearDown() override {
        // 清理代码，如释放内存或关闭文件
    }
};

// 测试 Add 方法
TEST_F(CalculatorTest, AddTest) {
    Calculator calc;
    EXPECT_EQ(calc.Add(1, 1), 2);
    EXPECT_TRUE(calc.Add(-1, -1) == -2);
    EXPECT_FALSE(calc.Add(-1, 1) == 1);
}

// 测试 Subtract 方法
TEST_F(CalculatorTest, SubTest) {
    Calculator calc;
    EXPECT_EQ(calc.Sub(2, 1), 1);
    EXPECT_TRUE(calc.Sub(-1, -1) == 0);
    EXPECT_FALSE(calc.Sub(-1, 1) == 1);
}
