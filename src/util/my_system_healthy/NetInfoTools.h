#pragma once
#include "SystemHealthy.pb.h"

namespace MySystemHealthy {
class NetInfoTools {
public:

    NetInfoTools();

    ~NetInfoTools();
    
    static SystemHealthy::NetInfo CollectNetInfo();
};
}