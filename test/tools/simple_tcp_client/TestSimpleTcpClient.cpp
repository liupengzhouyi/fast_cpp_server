#include <iostream>
#include <string>
#include <unistd.h> // for sleep function

#include "gtest/gtest.h"
#include "SimpleTcpClient.h"
#include "ThreadSafeBuffer.h"


using namespace tools;

TEST(ExampleTest, TestLinkA) {

    std::string ip = "192.168.1.166";
    int port = 5000;
    int maxTimeoutMs = 3000;
    SimpleTcpClient simpleTcpClient = SimpleTcpClient(ip, port, maxTimeoutMs);
    std::vector<char> outData = {};

    while (true)
    {
        sleep(8);
        if (simpleTcpClient.readData(outData)) {
            for (auto c : outData) {
                std::cout << c;
            }
            std::cout << std::endl;
        } else {
            std::cout << "--- ERROR for get data" << std::endl;
        }
        break;
    }
}

TEST(ExampleTest, TestLinkB) {
    std::string ip = "192.168.1.230";
    int port = 3000;
    int maxTimeoutMs = 3000;
    SimpleTcpClient simpleTcpClient = SimpleTcpClient(ip, port, maxTimeoutMs);
    std::vector<char> outData = {};

    while (true)
    {
        sleep(8);
        if (simpleTcpClient.readData(outData)) {
            for (auto c : outData) {
                std::cout << c;
            }
            std::cout << std::endl;
        } else {
            std::cout << "--- ERROR for get data" << std::endl;
        }
        break;
    }
}