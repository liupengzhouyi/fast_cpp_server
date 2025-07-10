#include "SoftHealthSnapshot.h"
#include <sstream>
#include <unordered_map>
#include <iomanip>

void HealthSnapshot::setUptime(std::chrono::steady_clock::time_point start_time) {
    auto now = std::chrono::steady_clock::now();
    uptime_seconds_ = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count() / 1000.0;
}

void HealthSnapshot::setThreadSnapshots(const std::vector<ThreadSnapshot>& threads) {
    threads_ = threads;
}

void HealthSnapshot::setProcessInfo(const ProcessInfo& proc_info) {
    process_info_ = proc_info;
}

void HealthSnapshot::buildThreadTree() {
    thread_tree_.clear();
    std::unordered_map<int, ThreadTreeNode> id_to_node;

    // 构建 TID 到 Node 的映射
    for (const auto& thread : threads_) {
        id_to_node[thread.tid] = ThreadTreeNode{thread, {}};
    }

    // 构建父子结构
    for (const auto& thread : threads_) {
        if (thread.parent_tid != 0 && id_to_node.count(thread.parent_tid)) {
            id_to_node[thread.parent_tid].children.push_back(id_to_node[thread.tid]);
        } else {
            thread_tree_.push_back(id_to_node[thread.tid]);
        }
    }
}

double HealthSnapshot::getUptimeSeconds() const {
    return uptime_seconds_;
}

const std::vector<ThreadSnapshot>& HealthSnapshot::getThreads() const {
    return threads_;
}

const std::vector<ThreadTreeNode>& HealthSnapshot::getThreadTree() const {
    return thread_tree_;
}

const ProcessInfo& HealthSnapshot::getProcessInfo() const {
    return process_info_;
}

std::string HealthSnapshot::toJsonString() const {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"uptime_seconds\": " << std::fixed << std::setprecision(2) << uptime_seconds_ << ",\n";
    oss << "  \"process\": {\n";
    oss << "    \"pid\": " << process_info_.pid << ",\n";
    oss << "    \"ppid\": " << process_info_.ppid << ",\n";
    oss << "    \"name\": \"" << process_info_.name << "\",\n";
    oss << "    \"vm_rss_kb\": " << process_info_.vm_rss_kb << ",\n";
    oss << "    \"vm_size_kb\": " << process_info_.vm_size_kb << ",\n";
    oss << "    \"children_pids\": [";
    for (size_t i = 0; i < process_info_.children_pids.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << process_info_.children_pids[i];
    }
    oss << "]\n  },\n";

    oss << "  \"threads\": [\n";
    for (size_t i = 0; i < threads_.size(); ++i) {
        const auto& t = threads_[i];
        oss << "    {\n"
            << "      \"tid\": " << t.tid << ",\n"
            << "      \"name\": \"" << t.name << "\",\n"
            << "      \"cpu_usage_percent\": " << t.cpu_usage_percent << ",\n"
            << "      \"stack_kb\": " << t.stack_kb << ",\n"
            << "      \"parent_tid\": " << t.parent_tid << ",\n"
            << "      \"utime_ticks\": " << t.utime_ticks << ",\n"
            << "      \"stime_ticks\": " << t.stime_ticks << "\n"
            << "    }";
        if (i + 1 < threads_.size()) oss << ",";
        oss << "\n";
    }
    oss << "  ]\n}";

    return oss.str();
}
