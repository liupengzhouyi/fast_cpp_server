#include "MemInfoTools.h"
#include <fstream>
#include <string>

namespace SystemHealthyTools {

MemInfoTools::MemInfoTools() {};

MemInfoTools::~MemInfoTools() {};

SystemHealthy::MemInfo MemInfoTools::CollectMemInfo() {
    SystemHealthy::MemInfo info;
    std::ifstream file("/proc/meminfo");
    std::string key;
    uint64_t value;
    std::string unit;
    uint64_t memTotal = 0, memAvailable = 0;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") memTotal = value * 1024;
        if (key == "MemAvailable:") memAvailable = value * 1024;
    }
    info.set_total_bytes(memTotal);
    info.set_available_bytes(memAvailable);
    if (memTotal > 0)
        info.set_usage_percent(100.0f * (memTotal - memAvailable) / memTotal);

    return info;
}
}