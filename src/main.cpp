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

#include "InitTools.h"
#include "FreeFunc.h"
#include "Pipeline.h"
#include "SimpleTcpClient.h"
#include "ThreadSafeBuffer.h"
#include "DeviceOnlineMonitor.h"
#include "ServiceGuard.h"

using namespace tools;
// using namespace MySystemHealthy;
using json = nlohmann::json;



int main(int argc, char* argv[]) {

    std::string appName                     = "fast_cpp_server";
    std::string defaultINIConfigFilePath    = "/etc/fast_cpp_server/config.ini";
    std::string defaultJSONConfigFilePath   = "/etc/fast_cpp_server/config.json";
    std::string defaultYAMLConfigFilePath   = "/etc/fast_cpp_server/config.yaml";
    std::string defaultLogDirPath           = "/var/fast_cpp_server/logs/";
    std::string defaultLogFilePath          = defaultLogDirPath + appName + ".log";
    std::string configFilePath              = defaultINIConfigFilePath;
    std::string logDirPath                  = defaultLogDirPath;
    std::string logFilePath                 = defaultLogFilePath;
    std::vector<std::string> logInfos       = {};
    bool is_hard_setup                      = false; // 是否强制自检

    ArgumentParser parser;
    parser.addOption("-h", "--help",    "Show help info");
    parser.addOption("-v", "--version", "Show version info");
    parser.addOption("-s", "--setup",   "Setup mode",       true);
    parser.addOption("-c", "--config",  "Set config file",  true);

    std::vector<std::map<std::string, std::string>> args = parser.parse(argc, argv);
    for (const auto& item : args) {
        if (item.at("key") == "-h" || item.at("key") == "--help") {
            parser.printHelp();
            return 0;
        } else if (item.at("key") == "-v" || item.at("key") == "--version") {
            std::cout << "App Version: 1.0.0" << std::endl;
            return 0;
        } else if (item.at("key") == "-s" || item.at("key") == "--setup") {
            std::cout << "Setup mode activated: mode:" << item.at("value") << std::endl;
            std::string value = item.at("value");
            if (value == "hard" || value == "true" || value == "1") {
                is_hard_setup = true;
                std::cout << "Hard setup mode activated." << std::endl;
                logInfos.emplace_back("Hard setup mode activated.");
            } else {
                std::cout << "Normal setup mode activated." << std::endl;
                logInfos.emplace_back("Normal setup mode activated.");
            }
            tools::service_guard::ServiceGuard::GetInstance().Execute(is_hard_setup);       // 执行服务自检
        } else if (item.at("key") == "-c" || item.at("key") == "--config") {
            tools::init_tools::loadConfigFromArguments(args, logInfos, configFilePath);
        } else {
            // Unknown argument, can log or ignore
        }
    }

    // 加载配置文件
    bool load_ini_config_status  = tools::init_tools::initLoadConfig("ini",  configFilePath,  logInfos);
    bool load_json_config_status = tools::init_tools::initLoadConfig("json", defaultJSONConfigFilePath, logInfos);
    bool load_yaml_config_status = tools::init_tools::initLoadConfig("yaml", defaultYAMLConfigFilePath, logInfos);

    // 初始化日志系统
    if (!tools::free_func::loadLogConfigFromINIConfig(logInfos, logDirPath, logFilePath, appName)) {
        logDirPath = defaultLogDirPath;
        logFilePath = logDirPath + appName + ".log";
        std::cout << "[LogConfig] 使用默认日志目录: " << logDirPath << std::endl;
        logInfos.emplace_back("[LogConfig] 使用默认日志目录: " + logDirPath);
    }
    MyLog::Init(logFilePath);  // ✅ 只调用一次
    
    MYLOG_INFO("----------------------------------- Init Log -------------------------");
    if (!tools::free_func::checkConfigLoadStatus(load_ini_config_status, load_json_config_status, load_yaml_config_status)) {
        MYLOG_ERROR("配置加载失败，程序退出.");
        return -1;
    }
    for (std::string& logItem : logInfos) {
        MYLOG_INFO(logItem);
    }
    MYLOG_INFO("----------------------------------------------------------------------");
    tools::free_func::logWelcomeMessage();
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

    tools::free_func::showMyConfig("INI");
    tools::free_func::showMyConfig("JSON");
    tools::free_func::showMyConfig("YAML");

    // pipeline::Pipeline::GetInstance().Init(MyJSONConfig::GetInstance().Raw());

    // 1. 模拟或读取配置文件
    nlohmann::json config = {
        {"terminals", {
            {{"id", 101}, {"ip", "192.168.1.10"}, {"port", 5000}, {"enabled", true}},
            {{"id", 102}, {"ip", "192.168.1.11"}, {"port", 5001}, {"enabled", true}},
            {{"id", 103}, {"ip", "192.168.1.12"}, {"port", 5002}, {"enabled", false}} // 不会启动
        }}
    };

    // 模拟读取配置文件得到 json 对象
    nlohmann::json pipelineConfig = {};
    MyJSONConfig::GetInstance().Get("pipeline", {}, pipelineConfig);
    MYLOG_INFO("pipeline Config: \n {}", pipelineConfig.dump(2));
    MYLOG_INFO("======================================================================");


    // 2. 初始化 Pipeline (单例)
    tools::pipeline::Pipeline::GetInstance().Init(pipelineConfig);

    // 3. 启动所有使能的连接
    tools::pipeline::Pipeline::GetInstance().Start();

    // 主线程保持运行
    std::cout << "System is running. Press Enter to exit..." << std::endl;
    std::cin.get();

    // 4. 停止并清理
    tools::pipeline::Pipeline::GetInstance().Stop();

    // auto& hb = HeartbeatManager::Instance();
    // nlohmann::json hbConfig = nlohmann::json::object();
    // try {
    //     MyJSONConfig::GetInstance().Get("heartbeat", nlohmann::json::object(), hbConfig);
    //     MYLOG_INFO("获取 heartbeat 配置成功: {}", hbConfig.dump());
    // } catch (const std::exception& e) {
    //     MYLOG_ERROR("获取 heartbeat 配置异常: {}", e.what());
    // }
    // hb.Init(hbConfig);
    // hb.Start();


    bool break_loop = false;
    while (true) {
        // 获取推出信号
        
        if (break_loop) {
            break;
        } 
        MYLOG_INFO("Main thread is running... : {}", logFilePath);
        sleep(3);
    }


    // MYLOG_INFO("Test jsoncpp libary.");
    // json data = json::parse(R"({"key": "value"})");
    // std::string value = data["key"];
    // data["new_item_01"] = 1;
    // data["new_item_02"] = std::vector<int>{1, 2, 3, 4};
    // // std::shared_ptr<std::vector<int>> ptr = std::make_shared<std::vector<int>>({1, 2, 3, 4});
    // // data["new_item_03"] = ptr;

    // std::string json_str = data.dump();
    // std::cout << json_str << std::endl;

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

    // cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/users/octocat"});

    // // 检查请求是否成功
    // if (r.status_code == 200) {
    //     // 输出响应内容
    //     std::cout << "Request successful!" << std::endl;
    //     std::cout << "Status code: " << r.status_code << std::endl;
    //     std::cout << "Response body: " << r.text << std::endl;
    // } else {
    //     // 输出错误信息
    //     std::cout << "Request failed with status code: " << r.status_code << std::endl;
    //     std::cout << "Error message: " << r.error.message << std::endl;
    // }

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
    monitor.stop();
    // hb.Stop();

    return 0;
}