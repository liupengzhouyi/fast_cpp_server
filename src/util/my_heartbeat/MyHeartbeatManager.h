#pragma once
#include <atomic>
#include <thread>
#include <nlohmann/json.hpp>

class HeartbeatManager {
public:
    static HeartbeatManager& Instance();

    void Init(const nlohmann::json& config);
    void Start();
    void Stop();

private:
    HeartbeatManager() = default;
    void WorkerLoop();
    nlohmann::json BuildHeartbeat() const;
    void SendHeartbeat(const nlohmann::json& data);

private:
    std::atomic<bool> running_{false};
    std::thread worker_;

    nlohmann::json config_;
    int interval_sec_{5};
    time_t start_time_{0};
};
