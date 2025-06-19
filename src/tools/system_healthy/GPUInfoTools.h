#pragma once
#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {
class GPUInfoTools {
public:
    GPUInfoTools();

    ~GPUInfoTools();
    
    static std::vector<SystemHealthy::GPUInfo> CollectGPUInfo();
};
}
