#include <gtest/gtest.h>
#include "CPUInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(CPUInfoToolsTest, GetCPUModelTest) {
    SystemHealthy::CPUInfo cpuInfo = SystemHealthyTools::CPUInfoTools::CollectCPUInfo();
    EXPECT_GT(cpuInfo.architecture(), "");
}
