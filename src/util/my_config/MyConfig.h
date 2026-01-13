#pragma once

#include <nlohmann/json.hpp>
#include <string>

class MyConfig {
public:
    // 初始化配置（只调用一次，建议在 main 函数最开头调用）
    static void Init(const std::string& config_file_path);

    // 获取配置单例
    static MyConfig& GetInstance();

    // 获取整个 JSON 配置
    const nlohmann::json& GetConfig() const;

    // Get 返回配置项，如果失败返回默认值，成功返回结果引用
    bool Get(const std::string& key, const nlohmann::json& default_value, const nlohmann::json*& out_value) const;

    const std::string& ShowConfig() const;

private:
    MyConfig() = default;

    bool LoadConfig(const std::string& path);

    nlohmann::json config_;
};
