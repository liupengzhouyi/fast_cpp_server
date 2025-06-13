#include <iostream>
#include <string>
#include <unistd.h> // for sleep function

#include <gtest/gtest.h>
#include <zmq.h>
#include <thread>
#include <chrono>
#include <cstring>
#include "PingByCPR.h"

TEST(PingByCPRTest, PingBaidu) {
    std::string url = "http://www.baidu.com";
    bool result = tools::PingTools::PingFuncByCPR::PingURL(url);
    EXPECT_TRUE(result) << "Failed to ping valid URL: " << url;
}

TEST(PingByCPRTest, PingGoogle) {
    std::string url = "http://www.google.com";
    bool result = tools::PingTools::PingFuncByCPR::PingURL(url);
    EXPECT_FALSE(result) << "Should not be able to ping invalid URL: " << url;
}