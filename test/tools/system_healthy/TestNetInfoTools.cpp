#include <gtest/gtest.h>
#include "NetInfoTools.h"
#include "SystemHealthy.pb.h"


TEST(NetInfoToolsTest, GetNetInfoTest) {
    SystemHealthy::NetInfo netInfo = SystemHealthyTools::NetInfoTools::CollectNetInfo();
    EXPECT_GT(netInfo.recv_bytes(), 0);
    EXPECT_GT(netInfo.transmit_bytes(), 0);
}
