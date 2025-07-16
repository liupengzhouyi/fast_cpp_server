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

// #include "system_healthy.h"
#include "MySystemHealthyManager.h"

#include "SimpleTcpClient.h"
#include "ThreadSafeBuffer.h"
#include "DeviceOnlineMonitor.h"

using namespace tools;
using namespace MySystemHealthy;
using json = nlohmann::json;

int main(int argc, char* argv[]) {

    ArgumentParser parser;
    parser.addOption("-h", "--help", "Show help info");
    parser.addOption("-v", "--version", "Show version info");
    parser.addOption("-c", "--config", "Set config file", true);

    auto args = parser.parse(argc, argv);

    for (const auto& item : args) {
        std::cout << "Arg: " << item.at("key") << ", Value: " << item.at("value") << std::endl;
    }

    // 如果存在 help 参数就打印帮助
    for (const auto& item : args) {
        if (item.at("key") == "-h" || item.at("key") == "--help") {
            parser.printHelp();
            return 0;
        }
    }

    MyLog::Init("logs/app.log");  // ✅ 只调用一次

    MYLOG_INFO("Hello World");
    MYLOG_DEBUG("This is a debug log");
    MYLOG_WARN("Warning message");
    MYLOG_ERROR("Error message");

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

    MySystemHealthyManager::GetInstance().Init(5);

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
        std::cout << "Device online: " << (monitor.isOnline() ? "YES" : "NO") << std::endl;
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

    return 0;
}