#include <gtest/gtest.h>
#include "SystemHealthy.pb.h"
#include "MySystemHealthyManager.h"

using namespace MySystemHealthy;

TEST(MySystemHealthyManagerTest, MySystemHealthyManagerInitAndGetData) {
    MySystemHealthyManager::Instance().Init(5);

    SystemHealthy::SystemInfo systemInfo = MySystemHealthyManager::Instance().GetSystemInfo();

    EXPECT_EQ(systemInfo.os_name(), "Linux");
    
}
