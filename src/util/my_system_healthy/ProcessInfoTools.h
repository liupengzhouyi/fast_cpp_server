#pragma once
#include "SystemHealthy.pb.h"

namespace MySystemHealthy {
class ProcessInfoTools {
public:

    ProcessInfoTools();

    ~ProcessInfoTools();
    
    static std::vector<SystemHealthy::ProcessInfo> CollectProcessInfo();
};
}