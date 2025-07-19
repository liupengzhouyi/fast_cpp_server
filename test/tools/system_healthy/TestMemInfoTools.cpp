#include <gtest/gtest.h>
#include "MemInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(MemInfoToolsTest, GetMemInfoTest) {
    SystemHealthy::MemInfo memInfo = MySystemHealthy::MemInfoTools::CollectMemInfo();
    EXPECT_GT(memInfo.usage_percent(), 0);
}
