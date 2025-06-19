#pragma once

#include "SystemHealthy.pb.h"

namespace SystemHealthyTools {
class DiskInfoTools {
public:
    DiskInfoTools();
    ~DiskInfoTools();
    static SystemHealthy::DiskInfo CollectDiskInfo();
};
}