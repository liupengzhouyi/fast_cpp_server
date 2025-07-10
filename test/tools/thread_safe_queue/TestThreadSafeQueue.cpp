#include "thread_safe_queue.h"
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <future>
#include <iostream>

using namespace std;
using tools::thread_safe_queue::ThreadSafeQueue;

struct MyStruct {
    int id;
    std::string name;

    bool operator==(const MyStruct& other) const {
        return id == other.id && name == other.name;
    }
};

// --------------------- init() ------------------------
TEST(ThreadSafeQueueTest, Init_Int) {
    ThreadSafeQueue<int> q;
    q.init(50);
    EXPECT_EQ(q.size(), 0);
}

TEST(ThreadSafeQueueTest, Init_String) {
    ThreadSafeQueue<std::string> q;
    q.init(10);
    EXPECT_TRUE(q.empty());
}

TEST(ThreadSafeQueueTest, Init_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.init(5);
    EXPECT_EQ(q.size(), 0);
}

// --------------------- setMaxSize() ------------------------
TEST(ThreadSafeQueueTest, SetMaxSize_Int) {
    ThreadSafeQueue<int> q;
    q.setMaxSize(100);
    EXPECT_TRUE(q.push(1));
}

TEST(ThreadSafeQueueTest, SetMaxSize_String) {
    ThreadSafeQueue<std::string> q;
    q.setMaxSize(2);
    EXPECT_TRUE(q.push("abc"));
}

TEST(ThreadSafeQueueTest, SetMaxSize_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.setMaxSize(1);
    EXPECT_TRUE(q.push({1, "test"}));
}

// --------------------- push() ------------------------
TEST(ThreadSafeQueueTest, Push_Int) {
    ThreadSafeQueue<int> q;
    q.init(2);
    EXPECT_TRUE(q.push(10));
}

TEST(ThreadSafeQueueTest, Push_String) {
    ThreadSafeQueue<std::string> q;
    q.init(1);
    EXPECT_TRUE(q.push("hello"));
}

TEST(ThreadSafeQueueTest, Push_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.init(1);
    EXPECT_TRUE(q.push({42, "item"}));
}

// --------------------- pop() ------------------------
TEST(ThreadSafeQueueTest, Pop_Int) {
    ThreadSafeQueue<int> q;
    q.init(5);
    q.push(123);
    int value;
    EXPECT_TRUE(q.pop(value));
    EXPECT_EQ(value, 123);
}

TEST(ThreadSafeQueueTest, Pop_String) {
    ThreadSafeQueue<std::string> q;
    q.init(5);
    q.push("gtest");
    std::string value;
    EXPECT_TRUE(q.pop(value));
    EXPECT_EQ(value, "gtest");
}

TEST(ThreadSafeQueueTest, Pop_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.init(5);
    MyStruct s{5, "x"};
    q.push(s);
    MyStruct out;
    EXPECT_TRUE(q.pop(out));
    EXPECT_EQ(out, s);
}

// --------------------- size() ------------------------
TEST(ThreadSafeQueueTest, Size_Int) {
    ThreadSafeQueue<int> q;
    q.push(1);
    q.push(2);
    EXPECT_EQ(q.size(), 2);
}

TEST(ThreadSafeQueueTest, Size_String) {
    ThreadSafeQueue<std::string> q;
    q.push("a");
    EXPECT_EQ(q.size(), 1);
}

TEST(ThreadSafeQueueTest, Size_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.push({1, "a"});
    q.push({2, "b"});
    EXPECT_EQ(q.size(), 2);
}

// --------------------- clear() ------------------------
TEST(ThreadSafeQueueTest, Clear_Int) {
    ThreadSafeQueue<int> q;
    q.push(1);
    q.push(2);
    q.clear();
    EXPECT_TRUE(q.empty());
}

TEST(ThreadSafeQueueTest, Clear_String) {
    ThreadSafeQueue<std::string> q;
    q.push("x");
    q.clear();
    EXPECT_EQ(q.size(), 0);
}

TEST(ThreadSafeQueueTest, Clear_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.push({1, "a"});
    q.push({2, "b"});
    q.clear();
    EXPECT_TRUE(q.empty());
}

// --------------------- empty() ------------------------
TEST(ThreadSafeQueueTest, Empty_Int) {
    ThreadSafeQueue<int> q;
    EXPECT_TRUE(q.empty());
    q.push(5);
    EXPECT_FALSE(q.empty());
}

TEST(ThreadSafeQueueTest, Empty_String) {
    ThreadSafeQueue<std::string> q;
    EXPECT_TRUE(q.empty());
    q.push("not empty");
    EXPECT_FALSE(q.empty());
}

TEST(ThreadSafeQueueTest, Empty_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    EXPECT_TRUE(q.empty());
    q.push({9, "abc"});
    EXPECT_FALSE(q.empty());
}

// --------------------- shutdown() ------------------------
TEST(ThreadSafeQueueTest, Shutdown_Int) {
    ThreadSafeQueue<int> q;
    q.init(1);
    q.push(1);
    q.shutdown();
    EXPECT_FALSE(q.push(2));
}

TEST(ThreadSafeQueueTest, Shutdown_String) {
    ThreadSafeQueue<std::string> q;
    q.init(1);
    q.push("a");
    q.shutdown();
    std::string s;
    EXPECT_FALSE(q.pop(s));
}

TEST(ThreadSafeQueueTest, Shutdown_MyStruct) {
    ThreadSafeQueue<MyStruct> q;
    q.init(1);
    q.push({1, "x"});
    q.shutdown();
    MyStruct out;
    EXPECT_FALSE(q.pop(out));
}

// --------------------- multi-threaded tests ------------------------


// 显式的push线程函数
void push_worker(ThreadSafeQueue<int>& queue, int thread_id, int items_per_thread, std::atomic<int>& sum_pushed) {
    std::cout << "Start [Push Thread " << thread_id << "] " << std::endl;
    for (int j = 0; j < items_per_thread; ++j) {
        int value = thread_id * items_per_thread + j;
        bool ok = queue.push(value);
        if (ok) {
            sum_pushed += value;
            if (j % 1 == 0) {
                std::cout << "[Push Thread " << thread_id << "] Pushed: " << value << std::endl;
            }
        } else {
            std::cerr << "[Push Thread " << thread_id << "] Push failed at item: " << j << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 模拟处理时间
    }
}

// 显式的pop线程函数
void pop_worker(ThreadSafeQueue<int>& queue, 
                int total_items, 
                std::atomic<int>& pop_count, 
                std::atomic<int>& sum_popped, 
                int thread_id) {

    std::cout << "Start [Pop Thread " << thread_id << "] " << std::endl;
    // 10s后退出
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::seconds(10);
    while (std::chrono::steady_clock::now() < end_time) {
        int value = 0;
        bool ok = queue.pop(value, true, 100); // 超时防死锁
        if (ok) {
            sum_popped += value;
            int current = ++pop_count;
            if (current % 1 == 0) {
                std::cout << "[Pop Thread " << thread_id << "] Popped: " << value << " | Total popped: " << current << std::endl;
            }
            if (current >= total_items) {
                std::cout << "[Pop Thread " << thread_id << "] Reached total items: " << total_items << ", exiting." << std::endl;
                break;
            }
        } else {
            // 可能是shutdown或timeout
            if (pop_count.load() >= total_items) {
                std::cout << "[Pop Thread " << thread_id << "] Reached total items: " << total_items << ", exiting." << std::endl;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 模拟处理时间
    }
}


// GTest 单元测试入口
TEST(ThreadSafeQueueTest, HighConcurrency_Int) {
    const int push_thread_count = 3;
    const int pop_thread_count = 2;
    const int items_per_thread = 10;
    const int total_items = push_thread_count * items_per_thread;

    std::cout << "[Test] HighConcurrency_Int - Start with " << push_thread_count << " pushers and "
              << pop_thread_count << " poppers" << std::endl;

    ThreadSafeQueue<int> queue(false); // 不使用日志
    queue.init(total_items);

    std::atomic<int> pop_count(0);
    std::atomic<int> sum_pushed(0);
    std::atomic<int> sum_popped(0);

    std::vector<std::thread> push_threads;
    for (int i = 0; i < push_thread_count; ++i) {
        push_threads.emplace_back(push_worker, std::ref(queue), i, items_per_thread, std::ref(sum_pushed));
    }

    std::vector<std::thread> pop_threads;
    for (int i = 0; i < pop_thread_count; ++i) {
        pop_threads.emplace_back(pop_worker, std::ref(queue), total_items, std::ref(pop_count), std::ref(sum_popped), i);
    }

    for (auto& t : push_threads) {
        if (t.joinable()) {
            t.join();
        } else {
            std::cerr << "[Test] Push thread not joinable!" << std::endl;
        }
        std::cout << "[Test] Push thread joined" << std::endl;
    }
    std::cout << "[Test] All push threads joined" << std::endl;

    // // 避免 pop 线程无限等待：通知队列关闭
    // queue.shutdown();

    // 等待所有 pop 线程完成
    for (auto& t : pop_threads) {
        if (t.joinable()) {
            t.join();
        } else {
            std::cerr << "[Test] Pop thread not joinable!" << std::endl;
        }
    }
    std::cout << "[Test] All pop threads joined" << std::endl;

    EXPECT_EQ(pop_count.load(), total_items);
    EXPECT_EQ(sum_pushed.load(), sum_popped.load());

    std::cout << "[Test] Final: pushed sum = " << sum_pushed << ", popped sum = " << sum_popped << std::endl;
}