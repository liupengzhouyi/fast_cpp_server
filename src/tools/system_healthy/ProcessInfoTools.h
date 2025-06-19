#pragma once
#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {
class ProcessInfoTools {
public:

    ProcessInfoTools();

    ~ProcessInfoTools();
    
    static std::vector<SystemHealthy::ProcessInfo> CollectProcessInfo();
};
}