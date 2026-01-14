#include "MyHeartbeatManager.h"
#include <unistd.h>
#include <iostream>
#include <ctime>
#include "MyLog.h"


HeartbeatManager& HeartbeatManager::Instance() {
    static HeartbeatManager inst;
    return inst;
}

void HeartbeatManager::Init(const nlohmann::json& config) {
    config_ = config;
    interval_sec_ = 5; // 如果不是对象，使用硬编码默认值
    try {
        interval_sec_ = config.value("interval_sec", 5);
        std::cout << "[HeartbeatManager] Config: " << config.dump(2) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[HeartbeatManager] Failed to get interval_sec from global config: " << e.what() << std::endl;
    }
    start_time_ = time(nullptr);
    MYLOG_INFO("HeartbeatManager initialized with interval: {} seconds", interval_sec_);
}

void HeartbeatManager::Start() {
    if (running_) return;
    running_ = true;
    worker_ = std::thread(&HeartbeatManager::WorkerLoop, this);
}

void HeartbeatManager::Stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void HeartbeatManager::WorkerLoop() {
    while (running_) {
        try {
            auto hb = BuildHeartbeat();
            SendHeartbeat(hb);
        } catch (const std::exception& e) {
            MYLOG_ERROR("Heartbeat error: {}", e.what());
        }
        sleep(interval_sec_);
    }
}

nlohmann::json HeartbeatManager::BuildHeartbeat() const {
    nlohmann::json base = config_.value("base", nlohmann::json::object());

    base["pid"] = getpid();
    base["timestamp"] = time(nullptr);
    base["uptime_sec"] = time(nullptr) - start_time_;

    nlohmann::json heartbeat;
    heartbeat["base"] = base;
    heartbeat["extra"] = config_.value("extra", nlohmann::json::object());

    return {{"heartbeat", heartbeat}};
}

void HeartbeatManager::SendHeartbeat(const nlohmann::json& data) {
    std::string sender = config_.value("sender", "log");

    if (sender == "log") {
        MYLOG_INFO("Heartbeat:\n{}", data.dump(2));
    }
    // http / mqtt 可在此扩展
}
