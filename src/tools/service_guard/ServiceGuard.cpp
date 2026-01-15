#include "ServiceGuard.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>
#include <cstring>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <algorithm>


namespace tools {
namespace service_guard {

std::string ServiceGuard::ExecuteShell(const std::string& cmd) {
    char buffer[128];
    std::string result = "";
    std::cout << "Executing command: " << cmd << std::endl;
    // 使用 popen 打开管道执行指令
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() 执行失败!");
    }
    
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    
    // 去除末尾换行符
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    return result;
}

std::vector<std::string> ServiceGuard::ExecuteShellLines(const std::string& cmd) {
    std::vector<std::string> lines;
    char buffer[256];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return lines;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        lines.emplace_back(buffer);
    }
    pclose(pipe);
    return lines;
}

void ServiceGuard::Execute(bool is_hard_setup) {
    // 选择自检方式
    // 如果需要通过 systemctl 方式，可以调用下面的函数
    // ExecuteBySystemcal(is_hard_setup);
    // 这里我们选择通过 ps 指令进行自检
    ExecuteByPS(is_hard_setup);
}

void ServiceGuard::ExecuteByPS(bool is_hard_setup) {
    // 定义要搜索的进程关键字，确保唯一性
    std::string process_keyword = "fast_cpp_server";
    pid_t my_pid = getpid();

    std::cout << "* Arg: 自检模块, Value: 正在通过 ps 指令检索进程关键字 [" << process_keyword << "]" << std::endl;

    try {
        // 使用 grep -v grep 过滤掉 grep 命令本身
        std::string cmd = "ps -ef | grep " + process_keyword + " | grep -v grep";
        std::vector<std::string> ps_output = ExecuteShellLines(cmd);
        std::cout << "* Arg: ps 指令输出行数, Value: " << ps_output.size() << std::endl;
        std::vector<pid_t> other_pids;

        for (const auto& line : ps_output) {
            std::stringstream ss(line);
            std::string user, pid_str;
            ss >> user >> pid_str; // ps -ef 的第二列通常是 PID
            std::cout << "* Arg: 发现进程行, Value: " << line << std::endl;
            if (pid_str.empty()) continue;

            pid_t found_pid = std::stoi(pid_str);
            
            // 关键：排除掉当前进程自己
            if (found_pid != my_pid) {
                other_pids.push_back(found_pid);
            }
        }

        if (!other_pids.empty()) {
            std::cout << "* Arg: 冲突检测, Value: 发现 " << other_pids.size() << " 个存活的旧实例" << std::endl;

            if (is_hard_setup) {
                std::cout << "* Arg: 执行策略, Value: 正在清理旧实例..." << std::endl;
                for (pid_t pid : other_pids) {
                    std::cout << "  -> 正在杀死进程 PID: " << pid << std::endl;
                    kill(pid, SIGKILL); // 强制停止
                }
                // 给系统一点响应时间
                sleep(1); 
            } else {
                std::cout << "* Arg: 执行策略, Value: 检测到存活实例，为避免冲突，程序即将退出。" << std::endl;
                std::cout << "* Arg: 自检提示, Value: 若要强制启动，请附带 --setup hard 参数。" << std::endl;
                exit(0);
            }
        } else {
            std::cout << "* Arg: 冲突检测, Value: 未发现冲突进程，自检通过。" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "* Arg: 自检异常, Value: " << e.what() << std::endl;
    }
}

void ServiceGuard::ExecuteBySystemcal(bool hardsetup) {
    bool is_hard_setup = hardsetup;
    std::cout  <<"=== ServiceGuard 自检模块启动 (" << hardsetup << ") ===" << std::endl;
    // 1. 初始状态打印
    if (!is_hard_setup) {
        std::cout << "* Arg: 自检模块, Value: 以常规模式运行" << std::endl;
    } else {
        std::cout << "* Arg: 自检模块, Value: 以强制模式运行 (hard setup)" << std::endl;
    }

    try {
        // 2. 检测服务是否处于 active 状态
        std::string cmd_check = "systemctl is-active " + service_name_;
        std::string status = ExecuteShell(cmd_check);

        std::cout << "* Arg: 服务当前状态, Value: " << status << std::endl;

        if (status == "active") {
            std::cout << "* Arg: 冲突检测, Value: 检测到服务 " << service_name_ << " 正在运行" << std::endl;

            if (is_hard_setup) {
                // 3. 强制模式：停止旧服务
                std::cout << "* Arg: 执行策略, Value: 检测到 hardsetup 参数，准备强制停止旧服务..." << std::endl;
                
                std::string cmd_stop = "systemctl stop " + service_name_;
                ExecuteShell(cmd_stop);
                
                std::cout << "* Arg: 执行结果, Value: 旧服务已停止，允许重新启动" << std::endl;
            } else {
                // 4. 常规模式：退出程序
                std::cout << "* Arg: 执行策略, Value: 程序已在运行中。如需重新启动，请使用 hardsetup 参数或手动 systemctl stop" << std::endl;
                std::cout << "* Arg: 自检提示, Value: 程序即将直接退出..." << std::endl;
                exit(0);
            }
        } else {
            std::cout << "* Arg: 冲突检测, Value: 未发现活跃的旧服务实例，自检通过" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "* Arg: 自检模块异常, Value: 执行指令时出错: " << e.what() << std::endl;
        // 异常情况下，通常建议保守起见，直接退出或根据业务需求决定是否继续
    } catch (...) {
        std::cerr << "* Arg: 自检模块异常, Value: 捕获到未知错误" << std::endl;
    }
}

bool ServiceGuard::IsProcessAlive(int pid) {
    // 发送信号 0 是一种标准方法，用于检查进程是否存在且有权访问
    return (kill(pid, 0) == 0);
}

void ServiceGuard::KillOldProcess(int pid) {
    if (pid <= 1) return; // 保护系统进程

    // 尝试优雅停止 (SIGTERM)
    kill(pid, SIGTERM);
    
    // 等待一段时间让其退出
    for (int i = 0; i < 5; ++i) {
        if (!IsProcessAlive(pid)) return;
        printf("* Arg: {}, Value: {}", "等待退出", "正在等待旧进程(" + std::to_string(pid) + ")关闭...");
        sleep(1);
    }

    // 如果还没死，暴力杀掉 (SIGKILL)
    if (IsProcessAlive(pid)) {
        printf("* Arg: {}, Value: {}", "强制清理", "旧进程未响应，发送 SIGKILL");
        kill(pid, SIGKILL);
        sleep(1);
    }
}

void ServiceGuard::WritePidFile() {
    std::ofstream pid_file_out(pid_file_path_, std::ios::trunc);
    if (!pid_file_out.is_open()) {
        throw std::runtime_error("无法创建 PID 文件: " + pid_file_path_);
    }
    pid_file_out << getpid();
    pid_file_out.close();
}

ServiceGuard::~ServiceGuard() {
    // 注意：如果是 hardsetup 杀掉别人，不建议在析构里删除 PID 文件
    // 只有正常退出的实例才清理
}

} // namespace pipeline
} // namespace tools