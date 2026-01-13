#include "MyYAMLConfig.h"
#include <sstream>

MyYAMLConfig* MyYAMLConfig::instance_ = nullptr;
std::once_flag MyYAMLConfig::init_flag_;

void MyYAMLConfig::Init(const std::string& path) {
    std::call_once(init_flag_, [&]() {
        instance_ = new MyYAMLConfig();
        instance_->Load(path);
    });
}

MyYAMLConfig& MyYAMLConfig::GetInstance() {
    return *instance_;
}

bool MyYAMLConfig::Load(const std::string& path) {
    try {
        root_ = YAML::LoadFile(path);
        return true;
    } catch (...) {
        return false;
    }
}

bool MyYAMLConfig::GetString(const std::string& key, const std::string& def, std::string& out) const {
    if (!root_[key]) { out = def; return false; }
    out = root_[key].as<std::string>();
    return true;
}

bool MyYAMLConfig::GetInt(const std::string& key, int def, int& out) const {
    if (!root_[key]) { out = def; return false; }
    out = root_[key].as<int>();
    return true;
}

bool MyYAMLConfig::GetDouble(const std::string& key, double def, double& out) const {
    if (!root_[key]) { out = def; return false; }
    out = root_[key].as<double>();
    return true;
}

bool MyYAMLConfig::GetBool(const std::string& key, bool def, bool& out) const {
    if (!root_[key]) { out = def; return false; }
    out = root_[key].as<bool>();
    return true;
}

std::string MyYAMLConfig::ShowConfig() const {
    std::stringstream ss;
    ss << root_;
    return ss.str();
}
