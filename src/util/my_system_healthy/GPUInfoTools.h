#pragma once
#include "SystemHealthy.pb.h"

namespace MySystemHealthy {
class GPUInfoTools {
public:
    GPUInfoTools();

    ~GPUInfoTools();
    
    static std::vector<SystemHealthy::GPUInfo> CollectGPUInfo();
};
}
