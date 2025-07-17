#pragma once

#include "SystemHealthy.pb.h"

namespace MySystemHealthy {

class CPUInfoTools {
public:
    CPUInfoTools();

    ~CPUInfoTools();
    
    static SystemHealthy::CPUInfo CollectCPUInfo();

    static float getUsagePercent(const std::string& oneCPUInfo);

    std::string getArchitecture() const;

};

}