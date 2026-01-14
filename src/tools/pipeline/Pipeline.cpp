#include "Pipeline.h"

namespace tools {
namespace pipeline {

// --- ConnectionPipe 实现 ---

ConnectionPipe::ConnectionPipe(const TerminalConfig& config) : config_(config) {
    MYLOG_INFO("* Arg: {}, Value: {}", "TerminalID", config_.id);
    MYLOG_INFO("* Arg: {}, Value: {}", "TargetIP", config_.ip);
    MYLOG_INFO("* Arg: {}, Value: {}", "TargetPort", config_.port);
}

ConnectionPipe::~ConnectionPipe() {
    Stop();
}

void ConnectionPipe::Start() {
    if (is_running_.exchange(true)) return;
    worker_thread_ = std::make_unique<std::thread>(&ConnectionPipe::WorkLoop, this);
    MYLOG_INFO("* Arg: {}, Value: {}", "PipeStatus", "Started Thread for ID " + std::to_string(config_.id));
}

void ConnectionPipe::Stop() {
    is_running_ = false;
    if (worker_thread_ && worker_thread_->joinable()) {
        worker_thread_->join();
    }
}

void ConnectionPipe::WorkLoop() {
    MYLOG_INFO("* Arg: {}, Value: {}", "WorkerLoop", "Entering loop for ID " + std::to_string(config_.id));
    
    while (is_running_) {
        // TODO: 在这里实现具体的管道监听/数据收发逻辑
        // 示例：CheckConnection(), ReadData()...
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    MYLOG_INFO("* Arg: {}, Value: {}", "WorkerLoop", "Exiting loop for ID " + std::to_string(config_.id));
}

// --- Pipeline 实现 ---

Pipeline::~Pipeline() {
    Stop();
}

void Pipeline::Init(const nlohmann::json& config) {
    MYLOG_INFO("* Arg: {}, Value: {}", "PipelineAction", "Initializing from config");

    if (!config.contains("terminals") || !config["terminals"].is_array()) {
        MYLOG_INFO("* Arg: {}, Value: {}", "Error", "Invalid JSON format: 'terminals' array not found");
        return;
    }

    for (const auto& item : config["terminals"]) {
        try {
            bool enabled = item.at("enabled").get<bool>();
            if (enabled) {
                TerminalConfig t_conf;
                t_conf.id = item.at("id").get<int>();
                t_conf.ip = item.at("ip").get<std::string>();
                t_conf.port = item.at("port").get<int>();
                t_conf.enabled = enabled;

                MYLOG_INFO("* Arg: {}, Value: {}", "CreatePipe", "Success for ID " + std::to_string(t_conf.id));
                pipes_.push_back(std::make_unique<ConnectionPipe>(t_conf));
            } else {
                MYLOG_INFO("* Arg: {}, Value: {}", "CreatePipe", "Skipped (Disabled) ID " + item.value("id", "unknown"));
            }
        } catch (const std::exception& e) {
            MYLOG_INFO("* Arg: {}, Value: {}", "Error", std::string("Parse error: ") + e.what());
        }
    }
}

void Pipeline::Start() {
    MYLOG_INFO("* Arg: {}, Value: {}", "PipelineAction", "Starting all enabled pipes");
    for (auto& pipe : pipes_) {
        pipe->Start();
    }
}

void Pipeline::Stop() {
    MYLOG_INFO("* Arg: {}, Value: {}", "PipelineAction", "Stopping all pipes");
    for (auto& pipe : pipes_) {
        pipe->Stop();
    }
    pipes_.clear();
}

} // namespace pipeline
} // namespace tools