#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include "ThreadSafeBuffer.h"


namespace tools {

/**
 * @brief TCP 连接工具类
 * 负责自动连接、断线重连、接收数据写缓冲区
 */
class SimpleTcpClient {
public:
    /**
     * @brief 构造函数
     * @param ip 服务器 IP
     * @param port 服务器端口
     * @param maxTimeoutMs 最大连接超时时间（毫秒）
     */
    SimpleTcpClient(const std::string& ip, int port, int maxTimeoutMs);

    /**
     * @brief 析构函数，优雅关闭线程
     */
    ~SimpleTcpClient();

    /**
     * @brief 从缓冲区读取数据（主线程调用）
     * @param outData 输出读取到的数据
     * @return 是否成功读取
     */
    bool readData(std::vector<char>& outData);

    // 发送数据
    bool sendData(const std::string& data);

private:
    void monitorConnection();   ///< 监控连接状态，自动重连
    bool connectToServer();     ///< 建立 TCP 连接
    void closeSocket();         ///< 关闭当前 socket
    void receiveLoop();         ///< 接收数据循环

    std::string ip_;
    int port_;
    int maxTimeoutMs_;
    int sockfd_;

    std::atomic<bool> running_;
    std::atomic<bool> connected_;

    std::thread monitorThread_;
    std::thread recvThread_;
    std::mutex connMutex_; // 保护socket读写

    ThreadSafeBuffer buffer_;
};

};