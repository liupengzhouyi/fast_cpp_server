#include "MyLog.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace MyLog;


// 测试日志初始化和输出
TEST(MyLogTest, DefaultInit) {
    std::string log_file = "logs/server.log";

    // 删除旧文件
    if (fs::exists(log_file)) fs::remove(log_file);

    MyLog::Init();  // 默认初始化

    MYLOG_INFO("默认日志初始化");
    MYLOG_DEBUG("默认日志初始化");
    MYLOG_WARN("默认日志初始化");
    MYLOG_ERROR("默认日志初始化");

    ASSERT_TRUE(fs::exists(log_file));
    std::ifstream in(log_file);
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    ASSERT_NE(content.find("默认日志初始化"), std::string::npos);
}

TEST(MyLogTest, CustomInit) {
    std::string custom_file = "logs/custom_test.log";
    if (fs::exists(custom_file)) fs::remove(custom_file);

    MyLog::Init(custom_file, 1024 * 1024, 2);  // 自定义路径和大小

    MYLOG_INFO("自定义日志初始化");
    MYLOG_DEBUG("自定义日志初始化");
    MYLOG_WARN("自定义日志初始化");
    MYLOG_ERROR("自定义日志初始化");

    ASSERT_TRUE(fs::exists(custom_file));
}
