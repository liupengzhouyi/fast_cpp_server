// DiskInfoTools.cpp
#include "DiskInfoTools.h"
#include <fstream>
#include <sstream>
#include <string>

namespace MySystemHealthy {

DiskInfoTools::DiskInfoTools() {};

DiskInfoTools::~DiskInfoTools() {};

SystemHealthy::DiskInfo DiskInfoTools::CollectDiskInfo() {
    SystemHealthy::DiskInfo info;
    std::ifstream file("/proc/diskstats");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string device;
        int major, minor;
        uint64_t reads, read_sectors, writes, write_sectors;
        ss >> major >> minor >> device >> reads >> reads >> read_sectors;
        for (int i = 0; i < 4; ++i) ss >> writes;
        ss >> write_sectors;

        info.set_read_bytes(info.read_bytes() + read_sectors * 512);
        info.set_write_bytes(info.write_bytes() + write_sectors * 512);
    }
    return info;
}
}