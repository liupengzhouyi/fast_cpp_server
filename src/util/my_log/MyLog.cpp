#include "MyLog.h"

namespace MyLog {

static std::shared_ptr<spdlog::logger> logger;

void Init(const std::string& log_file, size_t max_file_size, size_t max_files) {
    // 异步线程池初始化
    spdlog::init_thread_pool(8192, 1);  // 队列大小、线程数

    // 创建 rotating file sink（滚动文件）
    auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_file, max_file_size, max_files);

    // 创建异步 logger
    logger = std::make_shared<spdlog::async_logger>(
        "async_logger", sink,
        spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    spdlog::set_default_logger(logger);

    // 设置输出格式（包含文件名、行号、函数名）
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");

    // 设置日志等级
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::info);  // 自动 flush
}

void Info(const std::string& msg) {
    spdlog::info(msg);
}

void Debug(const std::string& msg) {
    spdlog::debug(msg);
}

void Warn(const std::string& msg) {
    spdlog::warn(msg);
}

void Error(const std::string& msg) {
    spdlog::error(msg);
}

void Flush() {
    if (logger) {
        logger->flush();
    }
}

}  // namespace MyLog
