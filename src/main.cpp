#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h> // for sleep function

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "system_healthy.h"

using json = nlohmann::json;

int main() {

    spdlog::info("Hello World");

    spdlog::info("Test jsoncpp libary.");
    json data = json::parse(R"({"key": "value"})");
    std::string value = data["key"];
    data["new_item_01"] = 1;
    data["new_item_02"] = std::vector<int>{1, 2, 3, 4};
    // std::shared_ptr<std::vector<int>> ptr = std::make_shared<std::vector<int>>({1, 2, 3, 4});
    // data["new_item_03"] = ptr;

    std::string json_str = data.dump();
    std::cout << json_str << std::endl;

    SystemHealthy systemHealthy;

    if (systemHealthy.checkSystemHealth()) {
        std::cout << "System is healthy." << std::endl;
    } else {
        std::cout << "System is not healthy." << std::endl;
    }

    auto report = systemHealthy.getHealthReport();
    for (const auto& [key, value] : report) {
        std::cout << key << ": " << value << std::endl;
    }

    

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


    while (true)
    {
        /* code */
        sleep(10);
    }
    

    return 0;
}