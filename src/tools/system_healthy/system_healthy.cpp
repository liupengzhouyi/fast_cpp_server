#include "system_healthy.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>

#ifdef __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <unistd.h>
#endif

#include "MyLog.h"


SystemHealthy::SystemHealthy() {
    MyLog::Info("初始化健康状态报告");
    healthReport["CPU Usage"] = "Unknown";
    healthReport["Memory Usage"] = "Unknown";
    healthReport["Disk Usage"] = "Unknown";
}

bool SystemHealthy::checkSystemHealth() {
    bool cpuHealthy = checkCPUUsage();
    bool memoryHealthy = checkMemoryUsage();
    bool diskHealthy = checkDiskUsage();

    return cpuHealthy && memoryHealthy && diskHealthy;
}

std::map<std::string, std::string> SystemHealthy::getHealthReport() const {
    return healthReport;
}

bool SystemHealthy::checkCPUUsage() {
#ifdef __linux__
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        healthReport["CPU Usage"] = "Error reading /proc/stat";
        return false;
    }

    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);

    std::string cpu;
    long user, nice, system, idle;
    iss >> cpu >> user >> nice >> system >> idle;

    long total = user + nice + system + idle;
    long usage = total - idle;

    double cpuUsage = (double)usage / total * 100.0;
    healthReport["CPU Usage"] = std::to_string(cpuUsage) + "%";

    return cpuUsage < 80.0; // 假设CPU使用率低于80%是健康的

#elif __APPLE__
    host_cpu_load_info_data_t cpuInfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuInfo, &count) != KERN_SUCCESS) {
        healthReport["CPU Usage"] = "Error getting CPU info";
        return false;
    }

    long totalTicks = 0;
    for (int i = 0; i < CPU_STATE_MAX; i++) {
        totalTicks += cpuInfo.cpu_ticks[i];
    }

    double cpuUsage = (double)(totalTicks - cpuInfo.cpu_ticks[CPU_STATE_IDLE]) / totalTicks * 100.0;
    healthReport["CPU Usage"] = std::to_string(cpuUsage) + "%";

    return cpuUsage < 80.0; // 假设CPU使用率低于80%是健康的
#endif
}

bool SystemHealthy::checkMemoryUsage() {
#ifdef __linux__
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        healthReport["Memory Usage"] = "Error getting sysinfo";
        return false;
    }

    long totalMemory = info.totalram;
    long freeMemory = info.freeram;
    double memoryUsage = (double)(totalMemory - freeMemory) / totalMemory * 100.0;
    healthReport["Memory Usage"] = std::to_string(memoryUsage) + "%";

    return memoryUsage < 80.0; // 假设内存使用率低于80%是健康的

#elif __APPLE__
    int64_t physicalMemory;
    size_t length = sizeof(int64_t);
    if (sysctlbyname("hw.memsize", &physicalMemory, &length, NULL, 0) != 0) {
        healthReport["Memory Usage"] = "Error getting memory size";
        return false;
    }

    vm_statistics_data_t vmStats;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &count) != KERN_SUCCESS) {
        healthReport["Memory Usage"] = "Error getting VM stats";
        return false;
    }

    int64_t freeMemory = (int64_t)vmStats.free_count * sysconf(_SC_PAGESIZE);
    double memoryUsage = (double)(physicalMemory - freeMemory) / physicalMemory * 100.0;
    healthReport["Memory Usage"] = std::to_string(memoryUsage) + "%";

    return memoryUsage < 80.0; // 假设内存使用率低于80%是健康的
#endif
}

bool SystemHealthy::checkDiskUsage() {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        healthReport["Disk Usage"] = "Error getting statvfs";
        return false;
    }

    long totalBlocks = stat.f_blocks;
    long freeBlocks = stat.f_bfree;
    double diskUsage = (double)(totalBlocks - freeBlocks) / totalBlocks * 100.0;
    healthReport["Disk Usage"] = std::to_string(diskUsage) + "%";

    return diskUsage < 80.0; // 假设磁盘使用率低于80%是健康的
}