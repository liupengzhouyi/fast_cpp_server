#pragma once
#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {
class NetInfoTools {
public:

    NetInfoTools();

    ~NetInfoTools();
    
    static SystemHealthy::NetInfo CollectNetInfo();
};
}