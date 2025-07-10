#pragma once

#include "SystemHealthy.pb.h"

namespace MySystemHealthy {
class DiskInfoTools {
public:
    DiskInfoTools();
    ~DiskInfoTools();
    static SystemHealthy::DiskInfo CollectDiskInfo();
};
}