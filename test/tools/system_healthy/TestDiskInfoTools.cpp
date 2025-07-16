#include <gtest/gtest.h>
#include "DiskInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(DiskInfoToolsTest, GetDiskInfoTest) {
    SystemHealthy::DiskInfo diskInfo = MySystemHealthy::DiskInfoTools::CollectDiskInfo();
    EXPECT_GT(diskInfo.read_bytes(), 0);
    EXPECT_GT(diskInfo.write_bytes(), 0);
}
