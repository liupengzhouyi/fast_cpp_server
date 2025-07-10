#include "SoftHealthMonitorConfig.h"

SoftHealthMonitorConfig::SoftHealthMonitorConfig() {}

SoftHealthMonitorConfig SoftHealthMonitorConfig::getDefaultConfig() {
    return SoftHealthMonitorConfig();
}

void SoftHealthMonitorConfig::setIntervalSeconds(int sec) {
    interval_seconds_ = sec;
}

void SoftHealthMonitorConfig::setEnableThreadTree(bool enable) {
    enable_thread_tree_ = enable;
}

void SoftHealthMonitorConfig::setOutputToConsole(bool enable) {
    output_to_console_ = enable;
}

void SoftHealthMonitorConfig::setEnableSubprocessCheck(bool enable) {
    enable_subprocess_check_ = enable;
}

int SoftHealthMonitorConfig::getIntervalSeconds() const {
    return interval_seconds_;
}

bool SoftHealthMonitorConfig::isThreadTreeEnabled() const {
    return enable_thread_tree_;
}

bool SoftHealthMonitorConfig::isConsoleOutputEnabled() const {
    return output_to_console_;
}

bool SoftHealthMonitorConfig::isSubprocessCheckEnabled() const {
    return enable_subprocess_check_;
}

std::string SoftHealthMonitorConfig::toJsonString() const {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"interval_seconds\": " << interval_seconds_ << ",\n";
    oss << "  \"enable_thread_tree\": " << (enable_thread_tree_ ? "true" : "false") << ",\n";
    oss << "  \"output_to_console\": " << (output_to_console_ ? "true" : "false") << ",\n";
    oss << "  \"enable_subprocess_check\": " << (enable_subprocess_check_ ? "true" : "false") << "\n";
    oss << "}";
    return oss.str();
}