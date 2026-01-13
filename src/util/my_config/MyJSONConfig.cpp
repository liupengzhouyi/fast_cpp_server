#include "MyJSONConfig.h"
#include <fstream>

MyJSONConfig* MyJSONConfig::instance_ = nullptr;
std::once_flag MyJSONConfig::init_flag_;

void MyJSONConfig::Init(const std::string& path) {
    std::call_once(init_flag_, [&]() {
        instance_ = new MyJSONConfig();
        instance_->Load(path);
    });
}

MyJSONConfig& MyJSONConfig::GetInstance() {
    return *instance_;
}

bool MyJSONConfig::Load(const std::string& path) {
    try {
        std::ifstream ifs(path);
        if (!ifs.is_open()) return false;
        ifs >> config_;
        return true;
    } catch (...) {
        return false;
    }
}

bool MyJSONConfig::Get(const std::string& key,
                       const nlohmann::json& def,
                       nlohmann::json& out) const {
    if (!config_.contains(key)) {
        out = def;
        return false;
    }
    out = config_[key];
    return true;
}

const nlohmann::json& MyJSONConfig::Raw() const {
    return config_;
}

std::string MyJSONConfig::ShowConfig() const {
    return config_.dump(2);
}
