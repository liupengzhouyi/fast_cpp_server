#include <gtest/gtest.h>
#include "GPUInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(GPUInfoToolsTest, GetGPUInfoTest) {
    std::vector<SystemHealthy::GPUInfo> GPUInfos = SystemHealthyTools::GPUInfoTools::CollectGPUInfo();
    EXPECT_GT(GPUInfos.size(), 0);
}
