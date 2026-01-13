#pragma once
#include <string>
#include <mutex>
#include <nlohmann/json.hpp>

class MyJSONConfig {
public:
    static void Init(const std::string& path);
    static MyJSONConfig& GetInstance();

    bool Get(const std::string& key,
             const nlohmann::json& def,
             nlohmann::json& out) const;

    const nlohmann::json& Raw() const;
    std::string ShowConfig() const;

private:
    MyJSONConfig() = default;
    bool Load(const std::string& path);

    static MyJSONConfig* instance_;
    static std::once_flag init_flag_;

    nlohmann::json config_;
};
