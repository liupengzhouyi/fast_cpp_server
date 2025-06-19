#pragma once
#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {
class MemInfoTools {
public:
    MemInfoTools();
    ~MemInfoTools();
    static SystemHealthy::MemInfo CollectMemInfo();
};
}