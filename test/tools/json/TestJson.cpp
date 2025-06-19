#include "gtest/gtest.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>


TEST(JsonReadWriteTest, WriteAndReadJsonFile) {
    std::string test_file = "test_rw_config.json";

    // 1. 构造 JSON 对象
    nlohmann::json write_json = {
        {"server", {
            {"host", "127.0.0.1"},
            {"port", 8080}
        }},
        {"debug", true},
        {"version", 1.2}
    };

    // 2. 写入 JSON 文件
    {
        std::ofstream o(test_file);
        ASSERT_TRUE(o.is_open());
        o << write_json.dump(4);  // 缩进4个空格
    }

    // 3. 读取 JSON 文件
    nlohmann::json read_json;
    {
        std::ifstream i(test_file);
        ASSERT_TRUE(i.is_open());
        i >> read_json;
    }

    // 4. 校验内容一致
    EXPECT_EQ(write_json, read_json);

    // 5. 清理测试文件
    std::filesystem::remove(test_file);
}
