#pragma once
#include <string>
#include <mutex>
#include <yaml-cpp/yaml.h>

class MyYAMLConfig {
public:
    static void Init(const std::string& path);
    static MyYAMLConfig& GetInstance();

    bool GetString(const std::string& key, const std::string& def, std::string& out) const;
    bool GetInt(const std::string& key, int def, int& out) const;
    bool GetDouble(const std::string& key, double def, double& out) const;
    bool GetBool(const std::string& key, bool def, bool& out) const;

    std::string ShowConfig() const;

private:
    MyYAMLConfig() = default;
    bool Load(const std::string& path);

    static MyYAMLConfig* instance_;
    static std::once_flag init_flag_;

    YAML::Node root_;
};
