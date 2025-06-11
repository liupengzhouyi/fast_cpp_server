#include <iostream>
#include <string>
#include <unistd.h> // for sleep function

#include <gtest/gtest.h>
#include <zmq.h>
#include <thread>
#include <chrono>
#include <cstring>


TEST(ZMQIntegrationTest, BasicTcpCommunication) {
    // 创建 ZeroMQ 上下文
    void* context = zmq_ctx_new();
    ASSERT_NE(context, nullptr);

    // 服务端线程
    std::thread serverThread([&]() {
        void* responder = zmq_socket(context, ZMQ_REP);
        ASSERT_NE(responder, nullptr);

        // 绑定 TCP 地址
        int rc = zmq_bind(responder, "tcp://*:5555");
        ASSERT_EQ(rc, 0);

        char buffer[128] = {0};
        int bytes = zmq_recv(responder, buffer, sizeof(buffer), 0);
        ASSERT_GT(bytes, 0);
        EXPECT_STREQ(buffer, "hello");

        const char* reply = "world";
        rc = zmq_send(responder, reply, strlen(reply), 0);
        ASSERT_EQ(rc, strlen(reply));

        zmq_close(responder);
    });

    // 稍作等待，确保服务端先 bind 完毕
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 客户端
    void* requester = zmq_socket(context, ZMQ_REQ);
    ASSERT_NE(requester, nullptr);

    int rc = zmq_connect(requester, "tcp://127.0.0.1:5555");
    ASSERT_EQ(rc, 0);

    const char* msg = "hello";
    rc = zmq_send(requester, msg, strlen(msg), 0);
    ASSERT_EQ(rc, strlen(msg));

    char buffer[128] = {0};
    int bytes = zmq_recv(requester, buffer, sizeof(buffer), 0);
    ASSERT_GT(bytes, 0);
    EXPECT_STREQ(buffer, "world");

    zmq_close(requester);
    zmq_ctx_term(context);

    serverThread.join();
}
