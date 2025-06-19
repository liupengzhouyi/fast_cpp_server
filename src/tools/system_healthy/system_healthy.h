#ifndef SYSTEM_HEALTHY_H
#define SYSTEM_HEALTHY_H

#include <string>
#include <vector>
#include <map>

/**
 * @class SystemHealthy
 * @brief This class represents a SystemHealthy class.
 */
class SystemHealthy {
public:
    // 构造函数
    SystemHealthy();

    // 检查系统健康状态
    bool checkSystemHealth();

    // 获取健康状态报告
    std::map<std::string, std::string> getHealthReport() const;

private:
    // 检查CPU使用率
    bool checkCPUUsage();

    // 检查内存使用率
    bool checkMemoryUsage();

    // 检查磁盘使用率
    bool checkDiskUsage();

    /** 
    * @brief A repoter of system healthy.
    */
    std::map<std::string, std::string> healthReport;
};

#endif // SYSTEM_HEALTHY_H