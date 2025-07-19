#include "CPUInfoTools.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "MyLog.h"

namespace MySystemHealthy {

CPUInfoTools::CPUInfoTools() {};

CPUInfoTools::~CPUInfoTools() {};

SystemHealthy::CPUInfo CPUInfoTools::CollectCPUInfo() {

    bool usedLog = false;
    SystemHealthy::CPUInfo info;
    info.set_architecture("x86_64"); // 示例
    info.set_core_count(sysconf(_SC_NPROCESSORS_ONLN));

    std::ifstream file("/proc/stat");
    std::string line;
    int core_id = 0;
    while (std::getline(file, line)) {
        if (usedLog) {
            MYLOG_INFO("line:" + line);
        }
        if (line.rfind("cpu", 0) == 0 && line[3] >= '0' && line[3] <= '9') {
            std::istringstream ss(line);
            std::string cpu;
            ss >> cpu;
            float usage = CPUInfoTools::getUsagePercent(line);
            auto* core = info.add_cores();
            core->set_core_id(core_id++);
            core->set_usage_percent(usage);
            if (usedLog) {
                MYLOG_INFO("CPU Core ID: " + std::to_string(core_id - 1) + ", Usage Percent: " + std::to_string(usage));
            }
        }
    }
    return info;
}

float CPUInfoTools::getUsagePercent(const std::string& oneCPUInfo) {
    try {
        std::istringstream iss(oneCPUInfo);
        std::string cpuLabel;
        iss >> cpuLabel;

        std::vector<uint64_t> fields;
        uint64_t value = 0;
        while (iss >> value) {
            fields.push_back(value);
        }

        if (fields.size() < 4) {
            return -1.0f;  // 或返回 NaN、-1.0f，或调用方约定的“无效值”
        }

        // 根据字段位置定义
        uint64_t user = fields[0];
        uint64_t nice = fields[1];
        uint64_t system = fields[2];
        uint64_t idle = fields[3];
        uint64_t iowait = (fields.size() > 4) ? fields[4] : 0;
        uint64_t irq = (fields.size() > 5) ? fields[5] : 0;
        uint64_t softirq = (fields.size() > 6) ? fields[6] : 0;
        uint64_t steal = (fields.size() > 7) ? fields[7] : 0;
        uint64_t guest = (fields.size() > 8) ? fields[8] : 0;
        uint64_t guestNice = (fields.size() > 9) ? fields[9] : 0;

        uint64_t idleAll = idle + iowait;
        uint64_t nonIdle = user + nice + system + irq + softirq + steal + guest + guestNice;
        uint64_t total = idleAll + nonIdle;

        if (total == 0) return 0.0f;
        return (100.0f * static_cast<float>(nonIdle)) / static_cast<float>(total);
    } catch (const std::exception& ex) {
        std::string errorInfo = "[CPUInfoTools] Failed to parse CPU info: " + std::string(ex.what());
        MYLOG_ERROR(errorInfo);
        return -1.0f;
    } catch (...) {
        MYLOG_ERROR("[CPUInfoTools] Unknown error while parsing CPU info.");
        return -1.0f;
    }
}

std::string CPUInfoTools::getArchitecture() const {
    try {
        std::ifstream cpuinfo("/proc/cpuinfo");
        if (!cpuinfo.is_open()) return "unknown";

        std::string line;
        while (std::getline(cpuinfo, line)) {
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            if (line.find("intel") != std::string::npos || line.find("amd") != std::string::npos) {
                return "x86_64";
            } else if (line.find("aarch64") != std::string::npos || line.find("arm") != std::string::npos) {
                return "aarch64";
            } else if (line.find("riscv") != std::string::npos) {
                return "riscv";
            }
        }

        return "unknown";
    } catch (...) {
        return "unknown";
    }
}

}