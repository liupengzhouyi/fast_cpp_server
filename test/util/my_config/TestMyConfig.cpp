#include "MyConfig.h"
#include "gtest/gtest.h"

#include <fstream>
#include <nlohmann/json.hpp>


static const std::string custom_json_path = "test_config.json";

// 帮助函数：写入测试用 JSON 文件
void WriteTestJson() {
    nlohmann::json j = {
        {"server", {
            {"host", "localhost"},
            {"port", 9999}
        }},
        {"A", {
            {"B", {
                {"c", 1}
            }}
        }}
    };

    std::ofstream o(custom_json_path);
    o << j.dump(4);
    o.close();
}

TEST(MyConfigTest, InitFromJsonAndGet) {
    WriteTestJson();

    MyConfig::Init(custom_json_path);

    MyConfig cfg = MyConfig::GetInstance();

    const nlohmann::json* value = nullptr;

    // 1. 正确读取项
    EXPECT_TRUE(cfg.Get("server.host", "default_host", value));
    EXPECT_EQ(*value, "localhost");

    EXPECT_TRUE(cfg.Get("server.port", 0, value));
    EXPECT_EQ(*value, 9999);

    EXPECT_TRUE(cfg.Get("A.B.c", -1, value));
    EXPECT_EQ(*value, 1);

    // 2. 读取不存在项，返回默认值
    EXPECT_FALSE(cfg.Get("nonexistent.key", "default_value", value));
    EXPECT_EQ(*value, "default_value");

    EXPECT_FALSE(cfg.Get("A.B.d", 42, value));
    EXPECT_EQ(*value, 42);
}
