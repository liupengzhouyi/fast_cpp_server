#pragma once

#include <queue>
#include <mutex>
#include <vector>

namespace tools {
/**
 * @brief 线程安全的缓冲区
 * 主要用于存储接收到的网络数据
 */
class ThreadSafeBuffer {
public:
    ThreadSafeBuffer() = default;
    ~ThreadSafeBuffer() = default;

    /**
     * @brief 添加数据到缓冲区
     * @param data 要添加的数据
     */
    void push(const std::vector<char>& data);

    /**
     * @brief 从缓冲区中取出数据
     * @param outData 用于保存取出的数据
     * @return 是否成功取出数据
     */
    bool pop(std::vector<char>& outData);

    /**
     * @brief 获取缓冲区中数据包的数量
     */
    size_t size() const;

private:
    mutable std::mutex mutex_;
    std::queue<std::vector<char>> buffer_;
}; // class ThreadSafeBuffer

}; // namespace tools