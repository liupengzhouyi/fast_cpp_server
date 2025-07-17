#include <gtest/gtest.h>
#include <iostream>
#include "CPUInfoTools.h"
#include "SystemHealthy.pb.h"
#include "MyLog.h"

TEST(CPUInfoToolsTest, GetCPUModelTest) {
    SystemHealthy::CPUInfo cpuInfo = MySystemHealthy::CPUInfoTools::CollectCPUInfo();
    std::cout << "cpuInfo:" << cpuInfo.DebugString() << std::endl;
    EXPECT_GT(cpuInfo.architecture(), "");
}

TEST(CPUInfoToolsTest, GetArchitectureReturnsKnownValues) {
    MySystemHealthy::CPUInfoTools info;
    std::string arch = info.getArchitecture();
    MYLOG_INFO("arch:" + arch);
    EXPECT_TRUE(
        arch == "x86_64" ||
        arch == "aarch64" ||
        arch == "riscv" ||
        arch == "unknown"
    );
}

TEST(CPUInfoToolsTest, GracefulFailOnInvalidInput) {
    std::string input = "cpu0 123";
    float usage = MySystemHealthy::CPUInfoTools::getUsagePercent(input);
    EXPECT_FLOAT_EQ(usage, -1.0f);  // 新的返回约定
}

TEST(CPUInfoToolsTest, NormalInput) {
    std::string input = "cpu0 100 50 50 800 20 0 0 0 0 0";
    float usage = MySystemHealthy::CPUInfoTools::getUsagePercent(input);
    EXPECT_GT(usage, 0.0);
    EXPECT_LT(usage, 100.0);
}

TEST(CPUInfoToolsTest, AllIdle) {
    std::string input = "cpu0 0 0 0 1000 0 0 0 0 0 0";
    float usage = MySystemHealthy::CPUInfoTools::getUsagePercent(input);
    EXPECT_FLOAT_EQ(usage, 0.0f);
}

TEST(CPUInfoToolsTest, AllBusy) {
    std::string input = "cpu0 500 200 300 0 0 0 0 0 0 0";
    float usage = MySystemHealthy::CPUInfoTools::getUsagePercent(input);
    EXPECT_FLOAT_EQ(usage, 100.0f);
}

TEST(CPUInfoToolsTest, InvalidInput) {
    std::string input = "cpu0 123";
    EXPECT_FLOAT_EQ(MySystemHealthy::CPUInfoTools::getUsagePercent(input), -1.0f);
}