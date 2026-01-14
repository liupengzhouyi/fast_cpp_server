#include "Pipeline.h"

namespace tools {
namespace pipeline {

void Pipeline::LogJsonParams(const std::string& prefix, const nlohmann::json& j) {
    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            LogJsonParams(prefix + (prefix.empty() ? "" : ".") + it.key(), it.value());
        }
    } else if (j.is_array()) {
        for (size_t i = 0; i < j.size(); ++i) {
            LogJsonParams(prefix + "[" + std::to_string(i) + "]", j[i]);
        }
    } else {
        // 符合你要求的日志样式
        MYLOG_INFO("* Arg: {}, Value: {}", prefix, j.dump());
    }
}

void Pipeline::Init(const nlohmann::json& config) {
    full_config_ = config;

    // 记录全局参数日志
    if (full_config_.contains("server")) LogJsonParams("server", full_config_["server"]);
    if (full_config_.contains("logging")) LogJsonParams("logging", full_config_["logging"]);
    
    MYLOG_INFO("* Arg: {}, Value: {}", "Pipeline", "Init Success");
}

void Pipeline::Start() {
    if (is_running_.exchange(true)) return;

    if (!full_config_.contains("pipeline") || !full_config_["pipeline"].contains("executes")) {
        MYLOG_INFO("* Arg: {}, Value: {}", "Error", "No execution nodes found in config");
        return;
    }

    const auto& executes = full_config_["pipeline"]["executes"];

    // 遍历配置文件中的每一个执行节点
    for (auto it = executes.begin(); it != executes.end(); ++it) {
        const auto& node = it.value();
        std::string model_name = node.value("model_name", "");
        const auto& args = node.at("model_args");

        MYLOG_INFO("* Arg: {}, Value: {}", "Launching_Node", it.key() + " (" + model_name + ")");
        
        // 打印该节点的所有参数
        LogJsonParams("Node_" + it.key() + ".Args", args);

        // --- 手动分发逻辑：根据 model_name 调用对应的启动函数 ---
        if (model_name == "heartbeat") {
            StartHeartbeat(args);
        } 
        else if (model_name == "comm") {
            StartComm(args);
        } 
        else if (model_name == "system_healthy") {
            StartSystemHealthy(args);
        }
        else {
            MYLOG_INFO("* Arg: {}, Value: {}", "Warning", "Unknown model_name: " + model_name);
        }
    }
}

// --- 在这里独立编写每个任务的启动方式 ---

void Pipeline::StartHeartbeat(const nlohmann::json& args) {
    int interval = args.value("interval_sec_", 1);
    
    worker_threads_.emplace_back([this, interval]() {
        MYLOG_INFO("* Arg: {}, Value: {}", "Thread", "Heartbeat started");
        while (is_running_) {
            // 执行具体的心跳逻辑...
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    });
}

void Pipeline::StartComm(const nlohmann::json& args) {
    int interval = args.value("interval_sec_", 1);
    
    worker_threads_.emplace_back([this, interval]() {
        MYLOG_INFO("* Arg: {}, Value: {}", "Thread", "Communication started");
        while (is_running_) {
            // 执行具体的通信逻辑...
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    });
}

void Pipeline::StartSystemHealthy(const nlohmann::json& args) {
    // 即使参数很复杂（比如带 edges），你也可以直接在这里解析
    int interval = args.value("interval_sec_", 1);
    
    if (args.contains("edges")) {
        for (auto& edge_it : args["edges"].items()) {
            MYLOG_INFO("* Arg: {}, Value: {}", "SystemHealthy_Edge", edge_it.value().at("name").get<std::string>());
        }
    }

    worker_threads_.emplace_back([this, interval, args]() {
        MYLOG_INFO("* Arg: {}, Value: {}", "Thread", "SystemHealthy started");
        // 这里可以直接拿到 args 进行更复杂的连接操作
        while (is_running_) {
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    });
}

void Pipeline::Stop() {
    is_running_ = false;
    for (auto& t : worker_threads_) {
        if (t.joinable()) t.join();
    }
    worker_threads_.clear();
    MYLOG_INFO("* Arg: {}, Value: {}", "Pipeline", "All threads stopped");
}

Pipeline::~Pipeline() {
    Stop();
}

} // namespace pipeline
} // namespace tools