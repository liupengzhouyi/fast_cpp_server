#pragma once

#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {

class CPUInfoTools {
public:
    CPUInfoTools();

    ~CPUInfoTools();
    
    static SystemHealthy::CPUInfo CollectCPUInfo();
};

}