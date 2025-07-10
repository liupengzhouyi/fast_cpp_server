#include "SoftHealthMonitorManager.h"
#include "SoftHealthMonitorConfig.h"
#include "SoftHealthSnapshot.h"
#include "MyLog.h"

// 模拟采集器头文件（你应当替换为真实实现）
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

SoftHealthMonitorManager::SoftHealthMonitorManager()
    : running_(false), initialized_(false), interval_seconds_(5) {}

SoftHealthMonitorManager::~SoftHealthMonitorManager() {
    stop();
}

void SoftHealthMonitorManager::init(const SoftHealthMonitorConfig& config) {
    std::string logInfo = "SoftHealthMonitorManager init with interval: " + std::to_string(config.getIntervalSeconds()) + " seconds";
    MyLog::Info(logInfo); // 使用MyLog记录初始化信息

    config_ = config;
    interval_seconds_ = config.getIntervalSeconds();
    start_time_ = std::chrono::steady_clock::now();
    initialized_ = true;
}

void SoftHealthMonitorManager::setIntervalSeconds(int seconds) {
    interval_seconds_ = seconds;
}

void SoftHealthMonitorManager::start() {
    if (!initialized_) {
        try {
            throw std::runtime_error("SoftHealthMonitorManager must be initialized before start()");
        } catch (const std::exception& ex) {
            MyLog::Error(std::string("SoftHealthMonitorManager start failed: ") + ex.what());
            return;
        }
    }

    if (running_) return;

    running_ = true;
    monitor_thread_ = std::thread(&SoftHealthMonitorManager::monitorLoop, this);
}

void SoftHealthMonitorManager::stop() {
    if (!running_) return;

    running_ = false;
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
}

/// 后台线程主循环
void SoftHealthMonitorManager::monitorLoop() {
    while (running_) {
        auto start = std::chrono::steady_clock::now();

        try {
            collectSnapshot();
        } catch (const std::exception& ex) {
            MyLog::Error(std::string("[HealthMonitor] Exception: ") + ex.what());
        }

        // 等待下一轮
        auto elapsed = std::chrono::steady_clock::now() - start;
        auto sleep_time = std::chrono::seconds(interval_seconds_) - elapsed;
        if (sleep_time > std::chrono::seconds(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}

/// 单次快照采集逻辑（使用伪逻辑占位）
void SoftHealthMonitorManager::collectSnapshot() {
    HealthSnapshot snapshot;

    // 设置运行时长
    snapshot.setUptime(start_time_);

    // 以下数据采集应调用采集器模块，这里我们用模拟数据填充
    ProcessInfo proc_info;
    // proc_info.pid = getpid();
    // proc_info.ppid = getppid();
    proc_info.name = "demo_program";
    proc_info.vm_rss_kb = 123456;
    proc_info.vm_size_kb = 234567;
    proc_info.children_pids = {};  // 可选：采集子进程

    snapshot.setProcessInfo(proc_info);

    // 模拟线程信息
    std::vector<ThreadSnapshot> thread_list;
    ThreadSnapshot main_thread;
    main_thread.tid = proc_info.pid;
    main_thread.name = "main";
    main_thread.cpu_usage_percent = 1.5;
    main_thread.stack_kb = 512;
    main_thread.parent_tid = 0;
    main_thread.utime_ticks = 100;
    main_thread.stime_ticks = 50;

    thread_list.push_back(main_thread);
    snapshot.setThreadSnapshots(thread_list);

    // 可选线程树构建
    if (config_.isThreadTreeEnabled()) {
        snapshot.buildThreadTree();
    }

    // 输出快照到控制台
    if (config_.isConsoleOutputEnabled()) {
        std::cout << "======== Health Snapshot ========" << std::endl;
        std::cout << "Uptime: " << snapshot.getUptimeSeconds() << " seconds" << std::endl;
        std::cout << "PID: " << proc_info.pid << ", PPID: " << proc_info.ppid << std::endl;
        std::cout << "Memory: RSS = " << proc_info.vm_rss_kb << " KB, "
                  << "VmSize = " << proc_info.vm_size_kb << " KB" << std::endl;
        std::cout << "Threads: " << thread_list.size() << std::endl;
        for (const auto& t : thread_list) {
            std::cout << "  [TID " << t.tid << "] " << t.name
                      << ", CPU: " << t.cpu_usage_percent << "%, Stack: " << t.stack_kb << "KB" << std::endl;
        }
        std::cout << "=================================" << std::endl;
    }

    // 在 collectSnapshot 中加入这一句：
    {
        std::lock_guard<std::mutex> lock(snapshot_mutex_);
        last_snapshot_ = snapshot;
    }

    // TODO: 你可以在这里添加日志记录、JSON 输出、远程推送等功能

}


HealthSnapshot SoftHealthMonitorManager::getLastSnapshot() {
    std::lock_guard<std::mutex> lock(snapshot_mutex_);
    return last_snapshot_;
}