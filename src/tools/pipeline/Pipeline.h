#pragma once
#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <map>
#include <nlohmann/json.hpp>
#include "MyLog.h"

namespace tools {
namespace pipeline {

class Pipeline {
public:
    static Pipeline& GetInstance() {
        static Pipeline instance;
        return instance;
    }

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    // 1. 初始化：保存配置并打印日志
    void Init(const nlohmann::json& config);

    // 2. 启动：根据 model_name 手动分发到不同的启动函数
    void Start();

    // 3. 停止：清理所有线程
    void Stop();

private:
    Pipeline() : is_running_(false) {}
    ~Pipeline();

    // 递归日志记录工具
    void LogJsonParams(const std::string& prefix, const nlohmann::json& j);

    // --- 具体的任务启动逻辑（在这里独立编写每个任务的启动方式） ---
    void StartHeartbeat(const nlohmann::json& args);
    void StartComm(const nlohmann::json& args);
    void StartSystemHealthy(const nlohmann::json& args);

    nlohmann::json full_config_;
    std::atomic<bool> is_running_;
    std::vector<std::thread> worker_threads_;
};

} // namespace pipeline
} // namespace tools

#endif // PIPELINE_H