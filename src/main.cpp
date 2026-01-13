#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h> // for sleep function

#include <spdlog/spdlog.h>

// #include <easylogging++.h>
// INITIALIZE_EASYLOGGINGPP

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "ArgumentParser.h"
#include "MyLog.h"
#include "MyINIConfig.h"
#include "MyJSONConfig.h"
#include "MyYAMLConfig.h"
#include "MyHeartbeatManager.h"

// #include "system_healthy.h"
#include "MySystemHealthyManager.h"

#include "SimpleTcpClient.h"
#include "ThreadSafeBuffer.h"
#include "DeviceOnlineMonitor.h"

using namespace tools;
// using namespace MySystemHealthy;
using json = nlohmann::json;

bool initLoadConfig(const std::string& type_,
                    const std::string& config_file_path,
                    std::vector<std::string>& logInfos)
{
    bool load_ok = false;

    auto tryLoad = [&](const std::string& name, auto&& initFunc, auto&& showFunc) -> bool {
        try {
            initFunc();
            logInfos.emplace_back("[Config] " + name + " 配置加载成功");
            MYLOG_DEBUG("{} Config:\n{}", name, showFunc());
            return true;
        } catch (const std::exception& e) {
            logInfos.emplace_back("[Config] " + name + " 配置加载失败: " + e.what());
        } catch (...) {
            logInfos.emplace_back("[Config] " + name + " 配置加载失败: unknown exception");
        }
        return false;
    };

    // ========== INI ==========
    if (type_ == "ini" || type_ == "all") {
        load_ok |= tryLoad(
            "INI",
            [&]() { MyINIConfig::Init(config_file_path); },
            [&]() { return MyINIConfig::GetInstance().ShowConfig(); }
        );
    }

    // ========== JSON ==========
    if (type_ == "json" || type_ == "all") {
        load_ok |= tryLoad(
            "JSON",
            [&]() { MyJSONConfig::Init(config_file_path); },
            [&]() { return MyJSONConfig::GetInstance().ShowConfig(); }
        );
    }

    // ========== YAML ==========
    if (type_ == "yaml" || type_ == "all") {
        load_ok |= tryLoad(
            "YAML",
            [&]() { MyYAMLConfig::Init(config_file_path); },
            [&]() { return MyYAMLConfig::GetInstance().ShowConfig(); }
        );
    }

    if (!load_ok) {
        logInfos.emplace_back("[Config] ❌ 所有配置加载失败");
    }

    return load_ok;
}


int main(int argc, char* argv[]) {

    ArgumentParser parser;
    parser.addOption("-h", "--help", "Show help info");
    parser.addOption("-v", "--version", "Show version info");
    parser.addOption("-c", "--config", "Set config file", true);

    auto args = parser.parse(argc, argv);

    // 如果存在 help 参数就打印帮助
    for (const auto& item : args) {
        if (item.at("key") == "-h" || item.at("key") == "--help") {
            parser.printHelp();
            return 0;
        }
        if (item.at("key") == "-v" || item.at("key") == "--version") {
            std::cout << "App Version: 1.0.0" << std::endl;
            return 0;
        }
    }

    std::string appName = "fast_cpp_server";
    // std::string defaultLogFilePath = "/var/log/" + appName + "/logs/" + appName + ".log";
    std::string rootPath                    = "/workspace/";
    std::string defaultLogDirPath           = rootPath + "fast_cpp_server/logs/";
    std::string defaultINIConfigFilePath    = rootPath + "fast_cpp_server/config/config.ini";
    std::string defaultJSONConfigFilePath   = rootPath + "fast_cpp_server/config/config.json";
    std::string defaultYAMLConfigFilePath   = rootPath + "fast_cpp_server/config/config.yaml";
    std::string defaultLogFilePath          = defaultLogDirPath + appName + ".log";
    std::string configFilePath              = "";
    std::string logDirPath                  = "";
    std::string logFilePath                 = "";

    std::vector<std::string> logInfos = {};
    // 加载配置文件
    bool load_ini_config_status  = initLoadConfig("ini",  defaultINIConfigFilePath,  logInfos);
    bool load_json_config_status = initLoadConfig("json", defaultJSONConfigFilePath, logInfos);
    bool load_yaml_config_status = initLoadConfig("yaml", defaultYAMLConfigFilePath, logInfos);

    MyINIConfig::GetInstance().GetString("app_name", appName, appName);
    MyINIConfig::GetInstance().GetString("logger_dir", defaultLogDirPath, logDirPath);
    logFilePath = logDirPath + appName + ".log";
    std::cout << "Log file path: " << logFilePath << std::endl;

    // 初始化日志系统
    MyLog::Init(logFilePath);  // ✅ 只调用一次
    MYLOG_INFO("----------------------------------------------------------------------");
    MYLOG_INFO("Hello World!");
    MYLOG_INFO("App is starting...");
    MYLOG_INFO(" * defaultINIConfigFilePath: {}", defaultINIConfigFilePath);
    MYLOG_INFO(" * defaultJSONConfigFilePath: {}", defaultJSONConfigFilePath);
    MYLOG_INFO(" * defaultYAMLConfigFilePath: {}", defaultYAMLConfigFilePath);
    MYLOG_INFO(" * logFilePath: {}", logFilePath);
    MYLOG_INFO("----------------------------------------------------------------------");
    MYLOG_INFO("Argument parsing completed. Parsed {} arguments.", args.size());
    for (const auto& item : args) {
        MYLOG_INFO("* Arg: {}, Value: {}", item.at("key"), item.at("value"));
    }
    MYLOG_INFO("----------------------------------------------------------------------");
    for (std::string& logItem : logInfos) {
        MYLOG_INFO(logItem);
    }
    if (!load_ini_config_status)  { MYLOG_ERROR("加载INI配置失败");     }
    if (!load_json_config_status) { MYLOG_ERROR("加载JSON配置失败");    }
    if (!load_yaml_config_status) { MYLOG_ERROR("加载YAML配置失败");    }


    auto& hb = HeartbeatManager::Instance();
    nlohmann::json hbConfig = {};
    MyJSONConfig::GetInstance().Get("heartbeat", nlohmann::json::object(), hbConfig);
    hb.Init(hbConfig);
    hb.Start();

























    MYLOG_INFO("Test jsoncpp libary.");
    json data = json::parse(R"({"key": "value"})");
    std::string value = data["key"];
    data["new_item_01"] = 1;
    data["new_item_02"] = std::vector<int>{1, 2, 3, 4};
    // std::shared_ptr<std::vector<int>> ptr = std::make_shared<std::vector<int>>({1, 2, 3, 4});
    // data["new_item_03"] = ptr;

    std::string json_str = data.dump();
    std::cout << json_str << std::endl;

    // SystemHealthy systemHealthy;

    // if (systemHealthy.checkSystemHealth()) {
    //     std::cout << "System is healthy." << std::endl;
    // } else {
    //     std::cout << "System is not healthy." << std::endl;
    // }

    // auto report = systemHealthy.getHealthReport();
    // for (const auto& [key, value] : report) {
    //     std::cout << key << ": " << value << std::endl;
    // }

    MySystemHealthy::MySystemHealthyManager::GetInstance().Init(5);

    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/users/octocat"});

    // 检查请求是否成功
    if (r.status_code == 200) {
        // 输出响应内容
        std::cout << "Request successful!" << std::endl;
        std::cout << "Status code: " << r.status_code << std::endl;
        std::cout << "Response body: " << r.text << std::endl;
    } else {
        // 输出错误信息
        std::cout << "Request failed with status code: " << r.status_code << std::endl;
        std::cout << "Error message: " << r.error.message << std::endl;
    }

    std::string ip = "192.168.1.230";
    int port = 3000;

    // std::string ip = "192.168.1.166";
    // int port = 5000;

    int maxTimeoutMs = 3000;
    SimpleTcpClient simpleTcpClient = SimpleTcpClient(ip, port, maxTimeoutMs);
    std::vector<char> outData = {};

    // DeviceOnlineMonitor monitor("8.8.8.8", 53, 3, 3); // Google DNS
    DeviceOnlineMonitor monitor;
    if (!monitor.init("8.8.8.8", 53, 3, 3)) {
        std::cerr << "Failed to initialize monitor\n";
        return 1;
    }

    monitor.start();

    for (int i = 0; i < 300; ++i) {
        MYLOG_INFO("Device online: {}", monitor.isOnline() ? "YES" : "NO");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    monitor.stop();

    while (true)
    {
        /* code */
        sleep(10);
        // if (simpleTcpClient.readData(outData)) {
        //     for (auto c : outData) {
        //         std::cout << c;
        //     }
        //     std::cout << std::endl;
        // } else {
        //     std::cout << "--- ERROR for get data" << std::endl;
        // }
    }

    // 程序退出前
    hb.Stop();

    return 0;
}