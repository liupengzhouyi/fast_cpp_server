#pragma once
#ifndef SERVICE_GUARD_H
#define SERVICE_GUARD_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace tools {
namespace service_guard {

class ServiceGuard {
public:
    static ServiceGuard& GetInstance() {
        static ServiceGuard instance;
        return instance;
    }

    // 执行自检与守卫逻辑
    void Execute(bool hardsetup);
    void ExecuteByPS(bool is_hard_setup);
    void ExecuteBySystemcal(bool is_hard_setup);

private:
    ServiceGuard() = default;
    ~ServiceGuard();

    // 执行 Shell 命令并获取输出结果
    std::string ExecuteShell(const std::string& cmd);

    std::vector<std::string> ExecuteShellLines(const std::string& cmd);

    const std::string service_name_ = "fast_cpp_server.service";

    // 检查指定 PID 的进程是否真的在运行
    bool IsProcessAlive(int pid);

    // 杀死旧进程
    void KillOldProcess(int pid);

    // 记录当前进程 PID 到文件
    void WritePidFile();

    const std::string pid_file_path_ = "/tmp/my_pipeline_service.pid";
};

} // namespace pipeline
} // namespace tools

#endif // SERVICE_GUARD_H