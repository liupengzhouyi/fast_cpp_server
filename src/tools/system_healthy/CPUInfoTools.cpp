#include "CPUInfoTools.h"
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace SystemHealthyTools {

CPUInfoTools::CPUInfoTools() {};

CPUInfoTools::~CPUInfoTools() {};

SystemHealthy::CPUInfo CPUInfoTools::CollectCPUInfo() {
    SystemHealthy::CPUInfo info;
    info.set_architecture("x86_64"); // 示例
    info.set_core_count(sysconf(_SC_NPROCESSORS_ONLN));

    std::ifstream file("/proc/stat");
    std::string line;
    int core_id = 0;
    while (std::getline(file, line)) {
        if (line.rfind("cpu", 0) == 0 && line[3] >= '0' && line[3] <= '9') {
            std::istringstream ss(line);
            std::string cpu;
            ss >> cpu;
            uint64_t user, nice, system, idle;
            ss >> user >> nice >> system >> idle;
            float usage = (user + nice + system) * 100.0f / (user + nice + system + idle);

            auto* core = info.add_cores();
            core->set_core_id(core_id++);
            core->set_usage_percent(usage);
        }
    }
    return info;
}
}