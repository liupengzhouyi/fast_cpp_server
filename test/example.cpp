#include "gtest/gtest.h"
#include "example.h"


TEST(ExampleTest, Add) {
    Example example;
    EXPECT_EQ(example.add(1, 1), 2);
    EXPECT_EQ(example.add(-1, 1), 0);
    EXPECT_EQ(example.add(-1, -1), -2);
}

TEST(ExampleTest, Subtract) {
    Example example;
    EXPECT_EQ(example.subtract(1, 1), 0);
    EXPECT_EQ(example.subtract(-1, 1), -2);
    EXPECT_EQ(example.subtract(-1, -1), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}