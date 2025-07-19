#include <gtest/gtest.h>
#include <Eigen/Dense>


TEST(EigenTest, MatrixXdTest) {

    Eigen::MatrixXd mat(2, 2);
    mat << 1, 2, 3, 4;

    // 测试矩阵的维度
    EXPECT_EQ(mat.rows(), 2);
    EXPECT_EQ(mat.cols(), 2);

    // 测试矩阵的值
    EXPECT_DOUBLE_EQ(mat(0, 0), 1);
    EXPECT_DOUBLE_EQ(mat(0, 1), 2);
    EXPECT_DOUBLE_EQ(mat(1, 0), 3);
    EXPECT_DOUBLE_EQ(mat(1, 1), 4);
}