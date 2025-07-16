#include <gtest/gtest.h>
#include "ProcessInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(ProcessInfoToolsTest, GetProcessInfoTest) {
    std::vector<SystemHealthy::ProcessInfo> processInfoList = MySystemHealthy::ProcessInfoTools::CollectProcessInfo();
    EXPECT_GT(processInfoList.size(), 0);
}
