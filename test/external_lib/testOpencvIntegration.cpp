#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

TEST(OpenCVTest, CanCreateMat) {
    // 创建一个 3x3 的黑色图像
    cv::Mat image = cv::Mat::zeros(3, 3, CV_8UC1);
    
    ASSERT_EQ(image.rows, 3);
    ASSERT_EQ(image.cols, 3);
    ASSERT_EQ(image.type(), CV_8UC1);
}

TEST(OpenCVTest, CanDrawCircle) {
    cv::Mat img = cv::Mat::zeros(100, 100, CV_8UC3);
    cv::circle(img, cv::Point(50, 50), 20, cv::Scalar(255, 0, 0), -1);

    // 取中心像素颜色应为蓝色（BGR）
    cv::Vec3b color = img.at<cv::Vec3b>(50, 50);
    EXPECT_EQ(color[0], 255);
    EXPECT_EQ(color[1], 0);
    EXPECT_EQ(color[2], 0);
}
