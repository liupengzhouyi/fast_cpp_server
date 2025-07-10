#pragma once
#include "SystemHealthy.pb.h"

namespace MySystemHealthy {
class MemInfoTools {
public:
    MemInfoTools();
    ~MemInfoTools();
    static SystemHealthy::MemInfo CollectMemInfo();
};
}