#pragma once
#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>
#include "MyLog.h"

namespace tools {
namespace pipeline {

// 终端配置结构
struct TerminalConfig {
    int id;
    std::string ip;
    int port;
    bool enabled;
};

// 具体的连接管道类
class ConnectionPipe {
public:
    explicit ConnectionPipe(const TerminalConfig& config);
    ~ConnectionPipe();

    void Start();
    void Stop();
    int GetId() const { return config_.id; }

private:
    void WorkLoop();

    TerminalConfig config_;
    std::unique_ptr<std::thread> worker_thread_;
    std::atomic<bool> is_running_{false};
};

// Pipeline 管理类 (单例)
class Pipeline {
public:
    static Pipeline& GetInstance() {
        static Pipeline instance;
        return instance;
    }

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    // 根据配置文件初始化并构建管道
    void Init(const nlohmann::json& config);
    
    // 启动所有构建好的管道线程
    void Start();
    
    // 停止并清理
    void Stop();

private:
    Pipeline() = default;
    ~Pipeline();

    std::vector<std::unique_ptr<ConnectionPipe>> pipes_;
};

} // namespace pipeline
} // namespace tools

#endif // PIPELINE_H