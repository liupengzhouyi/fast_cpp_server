#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

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


    return 0;
}