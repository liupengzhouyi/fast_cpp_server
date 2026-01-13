#include "SimpleTcpClient.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

#include "MyLog.h"


namespace tools {
/**
 * @brief TCP 客户端实现
 * 负责自动连接、断线重连、接收数据写缓冲区
 */

SimpleTcpClient::SimpleTcpClient(const std::string& ip, int port, int maxTimeoutMs)
    : ip_(ip),
      port_(port),
      maxTimeoutMs_(maxTimeoutMs),
      sockfd_(-1),
      running_(true),
      connected_(false) {

    MYLOG_INFO("SimpleTcpClient created: {}:{}", ip_, port_);
    
    // 启动读取网络请求线程
    recvThread_ = std::thread(&SimpleTcpClient::receiveLoop, this);
    // 启动监控线程
    monitorThread_ = std::thread(&SimpleTcpClient::monitorConnection, this);
}

SimpleTcpClient::~SimpleTcpClient() {
    running_ = false;
    closeSocket();

    if (monitorThread_.joinable()) {
        monitorThread_.join();
    }
    if (recvThread_.joinable()) {
        recvThread_.join();
    }

    // spdlog::info("SimpleTcpClient destroyed");
}

bool SimpleTcpClient::readData(std::vector<char>& outData) {
    return buffer_.pop(outData);
}

void SimpleTcpClient::monitorConnection() {
    while (running_) {
        if (!connected_) {
            MYLOG_WARN("Not connected, try to connect...");
            if (connectToServer()) {
                MYLOG_INFO("Connected to server {}:{}", ip_, port_);
                connected_ = true;
            } else {
                MYLOG_ERROR("Connect failed, retry in 1s...");
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


bool SimpleTcpClient::connectToServer() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) {
        MYLOG_ERROR("Failed to create socket: {}", strerror(errno));
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
    if (inet_pton(AF_INET, ip_.c_str(), &serverAddr.sin_addr) <= 0) {
        MYLOG_ERROR("Invalid IP address: {}", ip_);
        close(sockfd_);
        return false;
    }

    // 设置连接超时时间
    struct timeval timeout;
    timeout.tv_sec = maxTimeoutMs_ / 1000;
    timeout.tv_usec = (maxTimeoutMs_ % 1000) * 1000;
    setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    if (connect(sockfd_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        MYLOG_ERROR("Connect : {}", strerror(errno));
        close(sockfd_);
        return false;
    }

    connected_ = true;
    return true;
}

void SimpleTcpClient::receiveLoop() {
    // 已连接，尝试接收数据
    const size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE);

    while (running_) {
        if (connected_) {
            buffer = {};
            std::lock_guard<std::mutex> lock(connMutex_);
            ssize_t bytesRead = recv(sockfd_, buffer.data(), BUFFER_SIZE, 0);
            if (bytesRead > 0) {
                // 处理数据
                spdlog::info("Received {} bytes", bytesRead);
                MYLOG_INFO("Received {} bytes", bytesRead);
                std::vector<char> data(buffer.begin(), buffer.begin() + bytesRead);
                buffer_.push(data);
            } else if (bytesRead == 0) {
                // 连接关闭
                spdlog::warn("Server closed connection.");
                connected_ = false;
            } else {
                // 错误
                MYLOG_ERROR("recv() error: {}", strerror(errno));
                connected_ = false;
            }
        } else {
            // 未连接，等待1s再检查
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

bool SimpleTcpClient::sendData(const std::string& data) {
    std::lock_guard<std::mutex> lock(connMutex_); // 加锁，防止多线程同时访问 socketFd_

    if (!connected_) {
        MYLOG_ERROR("sendData failed: not connected to server.");
        return false;
    }

    ssize_t bytesSent = send(sockfd_, data.data(), data.size(), 0);
    if (bytesSent < 0) {
        MYLOG_ERROR("sendData failed: {}", strerror(errno));
        return false;
    } else if (static_cast<size_t>(bytesSent) < data.size()) {
        MYLOG_WARN("sendData partial send: {}/{}", bytesSent, data.size());
        // 这里你可以考虑循环发送剩余部分（更高级），或者简单返回 false
        return false;
    }

    MYLOG_INFO("sendData success: {} bytes", bytesSent);
    return true;
}

void SimpleTcpClient::closeSocket() {
    if (sockfd_ >= 0) {
        close(sockfd_);
        sockfd_ = -1;
        spdlog::info("Socket closed");
    }
    connected_ = false;
}


} // namespace tools
