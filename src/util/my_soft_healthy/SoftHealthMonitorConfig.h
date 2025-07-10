#ifndef SOFT_HEALTH_MONITOR_CONFIG_H
#define SOFT_HEALTH_MONITOR_CONFIG_H

#include <string>
#include <sstream>


class SoftHealthMonitorConfig {
public:
    SoftHealthMonitorConfig();

    /// 返回默认配置
    static SoftHealthMonitorConfig getDefaultConfig();

    /// 配置项设置器
    void setIntervalSeconds(int sec);
    void setEnableThreadTree(bool enable);
    void setOutputToConsole(bool enable);
    void setEnableSubprocessCheck(bool enable);

    /// 配置项读取器
    int getIntervalSeconds() const;
    bool isThreadTreeEnabled() const;
    bool isConsoleOutputEnabled() const;
    bool isSubprocessCheckEnabled() const;

    std::string toJsonString() const;
private:
    int interval_seconds_ = 5;
    bool enable_thread_tree_ = true;
    bool output_to_console_ = true;
    bool enable_subprocess_check_ = false;
};

#endif // SOFT_HEALTH_MONITOR_CONFIG_H
