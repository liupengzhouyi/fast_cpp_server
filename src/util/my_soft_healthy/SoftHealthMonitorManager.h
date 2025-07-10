#include "SoftHealthMonitorConfig.h"
#include "SoftHealthSnapshot.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <iostream>


class SoftHealthMonitorManager {
public:
    SoftHealthMonitorManager();
    ~SoftHealthMonitorManager();

    /// 初始化配置（必须先调用）
    void init(const SoftHealthMonitorConfig& config);

    /// 启动、停止
    void start();
    void stop();

    /// 可选单独设置间隔
    void setIntervalSeconds(int seconds);

    /// 获取当前配置
    HealthSnapshot getLastSnapshot();

private:
    void monitorLoop();
    void collectSnapshot();

private:
    std::atomic<bool> running_;
    std::thread monitor_thread_;
    std::mutex snapshot_mutex_;
    std::chrono::steady_clock::time_point start_time_;

    int interval_seconds_ = 5;
    SoftHealthMonitorConfig config_;  // 当前配置
    bool initialized_ = false;        // 防止 start() 在 init() 之前调用
    // 成员变量
    HealthSnapshot last_snapshot_;
};
