#include "MyConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>

void MyConfig::Init(const std::string& config_file_path) {
    GetInstance().LoadConfig(config_file_path);
}

MyConfig& MyConfig::GetInstance() {
    static MyConfig instance;
    return instance;
}

bool MyConfig::LoadConfig(const std::string& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) return false;

        file >> config_;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[MyConfig] 加载配置失败: " << e.what() << std::endl;
        return false;
    }
}

const nlohmann::json& MyConfig::GetConfig() const {
    return config_;
}

// 支持 "server.port" 形式的路径访问
bool MyConfig::Get(const std::string& key, const nlohmann::json& default_value, const nlohmann::json*& out_value) const {
    try {
        const nlohmann::json* current = &config_;
        std::istringstream ss(key);
        std::string part;

        while (std::getline(ss, part, '.')) {
            if (!current->contains(part)) {
                out_value = &default_value;
                return false;  // 没找到
            }
            current = &(*current)[part];
        }

        out_value = current;
        return true;
    } catch (...) {
        out_value = &default_value;
        return false;
    }
}

const std::string& MyConfig::ShowConfig() const {
    return config_.dump(4);
}