#ifndef HEALTH_SNAPSHOT_H
#define HEALTH_SNAPSHOT_H

#include <string>
#include <vector>
#include <map>
#include <chrono>

/// 描述一个线程的快照信息
struct ThreadSnapshot {
    int tid;                        // 线程 ID
    std::string name;              // 线程名称（从 /proc/<pid>/task/<tid>/comm 获取）
    double cpu_usage_percent;      // CPU 使用率（百分比，基于上一轮采样差值计算）
    size_t stack_kb;               // 栈空间占用（估算）
    int parent_tid;                // 父线程 ID
    long utime_ticks;              // 用户态 CPU 时间（单位：jiffies）
    long stime_ticks;              // 内核态 CPU 时间（单位：jiffies）
};

/// 描述当前进程（主程序）的快照信息
struct ProcessInfo {
    int pid;                       // 当前进程 ID（/proc/self/status 中的 Pid）
    int ppid;                      // 父进程 ID（/proc/self/status 中的 PPid）
    std::string name;              // 程序名称（/proc/self/comm）
    std::vector<int> children_pids;// 子进程列表（可选，从 /proc 遍历）
    
    // 内存使用情况（单位：KB）
    size_t vm_size_kb;             // 虚拟地址空间大小（VmSize）
    size_t vm_rss_kb;              // 常驻内存大小（VmRSS）
};

/// 用于构建线程的父子结构树
struct ThreadTreeNode {
    ThreadSnapshot thread;               // 当前线程快照
    std::vector<ThreadTreeNode> children; // 子线程列表
};

/// 健康快照数据类
class HealthSnapshot {
public:
    /// 设置程序启动时间（用于计算运行时长）
    void setUptime(std::chrono::steady_clock::time_point start_time);

    /// 设置线程快照信息
    void setThreadSnapshots(const std::vector<ThreadSnapshot>& threads);

    /// 设置进程相关信息（包括内存）
    void setProcessInfo(const ProcessInfo& proc_info);

    /// 构建线程父子关系树结构（可用于图形展示）
    void buildThreadTree();

    /// 获取当前程序已运行的时间（单位：秒）
    double getUptimeSeconds() const;

    /// 获取所有线程信息（线性）
    const std::vector<ThreadSnapshot>& getThreads() const;

    /// 获取线程树结构（主线程及其子线程）
    const std::vector<ThreadTreeNode>& getThreadTree() const;

    /// 获取当前进程信息（含内存/子进程）
    const ProcessInfo& getProcessInfo() const;

    /// 将当前快照转为 JSON 字符串（接口，稍后实现）
    std::string toJsonString() const;

private:
    double uptime_seconds_ = 0.0;                         // 程序运行时长（秒）
    std::vector<ThreadSnapshot> threads_;                 // 所有线程的平铺列表
    ProcessInfo process_info_;                            // 进程信息（名称、PID、内存等）
    std::vector<ThreadTreeNode> thread_tree_;             // 线程父子结构树
};

#endif // HEALTH_SNAPSHOT_H