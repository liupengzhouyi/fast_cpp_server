#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <iostream>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstddef>

#include "MyLog.h"
using namespace MyLog;

namespace tools {

namespace thread_safe_queue {

/// @brief 线程安全队列（支持阻塞、限长、并发安全）
/// @tparam T 数据类型（任意支持拷贝或移动的类型）
template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue(bool usedLog = true)
        : max_size_(1000), shutdown_(false), usedLog_(usedLog) {
            if (usedLog_) {
                MYLOG_INFO("[ThreadSafeQueue] Initialized with max size: {}", max_size_);
                MYLOG_INFO("[ThreadSafeQueue] Logging is enabled.");
            } else {
                MYLOG_INFO("[ThreadSafeQueue] Initialized with max size: {} (logging disabled)", max_size_);
            }
        }

    /// 初始化队列容量
    void init(size_t max_size) {
        std::lock_guard<std::mutex> lock(mutex_);
        max_size_ = max_size;
        if (usedLog_) {
            MYLOG_INFO("[ThreadSafeQueue::init] Queue initialized with max size: {}", max_size_);
        }
    }

    /// 动态设置最大容量（线程安全）
    void setMaxSize(size_t max_size) {
        std::lock_guard<std::mutex> lock(mutex_);
        max_size_ = max_size;
        if (usedLog_) {
            MYLOG_INFO("[ThreadSafeQueue::setMaxSize] Max size set to: {}", max_size_);
        }
    }

   
    /// @brief 向队列中插入元素
    /// @param item 要插入的元素（左值引用）
    /// @param block 是否阻塞等待（默认阻塞）
    /// @return 插入是否成功（在关闭状态或非阻塞满队列时返回 false）
    bool push(const T& item, bool block = true) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!block) {
            // 非阻塞模式，若队列已满则直接返回 false
            if (queue_.size() >= max_size_) {
                if (usedLog_) {
                    MYLOG_WARN("[ThreadSafeQueue::push] Non-blocking mode: queue is full.");
                }
                return false;
            }
        } else {
            // 阻塞等待队列未满，避免使用 lambda，改为手动条件判断
            while (queue_.size() >= max_size_ && !shutdown_) {
                cond_not_full_.wait(lock); // 等待有空间
            }
        }

        // 如果已经 shutdown，拒绝插入
        if (shutdown_) {
            if (usedLog_) {
                MYLOG_WARN("[ThreadSafeQueue::push] Queue is shutdown, cannot push new items.");
            }
            return false;
        }

        // 插入元素
        queue_.push_back(item);
        if (usedLog_) {
            MYLOG_INFO("[ThreadSafeQueue::push] Pushed item successfully, queue size: {}", queue_.size());
        }
        // 通知一个等待中的 pop() 操作
        cond_not_empty_.notify_one();
        return true;
    }


    /// 弹出元素（可阻塞、可指定超时时间）
    /// @param result 返回的结果引用
    /// @param block 是否阻塞等待
    /// @param timeout_ms 超时时间（毫秒），默认无限等待
    /// @return 是否成功获取元素
    bool pop(T& result, bool block = true, int timeout_ms = -1) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 非阻塞模式
        if (!block) {
            if (queue_.empty()) {
                if (usedLog_) {
                    MYLOG_WARN("[ThreadSafeQueue::pop] Non-blocking mode: queue is empty.");
                }
                return false;
            }
        }
        else {
            // 阻塞 + 无限等待
            if (timeout_ms < 0) {
                // 等待直到队列非空或 shutdown
                while (queue_.empty() && !shutdown_) {
                    if (usedLog_) {
                        MYLOG_INFO("[ThreadSafeQueue::pop] queue is empty and not shutdown; Blocking wait (no timeout)...");
                    }
                    cond_not_empty_.wait(lock);
                }
            }
            // 阻塞 + 超时等待
            else {
                auto now = std::chrono::steady_clock::now();
                auto timeout_time = now + std::chrono::milliseconds(timeout_ms);

                while (queue_.empty() && !shutdown_) {
                    if (usedLog_) {
                        MYLOG_INFO("[ThreadSafeQueue::pop] Blocking wait with timeout ({})...", timeout_ms);
                    }

                    // wait_until 返回 false 表示超时
                    if (cond_not_empty_.wait_until(lock, timeout_time) == std::cv_status::timeout) {
                        if (usedLog_) {
                            MYLOG_WARN("[ThreadSafeQueue::pop] Timeout reached while waiting for items.");
                        }
                        return false;
                    }
                }
            }
        }
        

        // 如果队列为空，并且已关闭，说明不会再有数据，返回失败
        if (queue_.empty()) {
            if (usedLog_) {
                MYLOG_WARN("[ThreadSafeQueue::pop] Queue is empty after waiting, and shutdown is true.");
            }
            return false;
        }

        // 从队列中获取数据
        result = std::move(queue_.front());
        queue_.pop_front();

        if (usedLog_) {
            MYLOG_INFO("[ThreadSafeQueue::pop] Popped item successfully, queue size: {}", queue_.size());
        }

        // 通知可能在等待空间的 push 操作
        cond_not_full_.notify_one();
        return true;
    }



    /// 清空队列
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.clear();
        cond_not_full_.notify_all(); // 🔥防止外部在清空后阻塞等待空间：
    }

    /// 当前队列元素数量
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    /// 队列是否为空
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    /// 关闭队列，唤醒所有阻塞线程
    void shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
        cond_not_full_.notify_all();
        cond_not_empty_.notify_all();
    }

private:
    mutable std::mutex mutex_;                ///< 互斥锁
    std::condition_variable cond_not_full_;   ///< 用于push阻塞的条件变量
    std::condition_variable cond_not_empty_;  ///< 用于pop阻塞的条件变量
    std::deque<T> queue_;                     ///< 底层容器
    size_t max_size_;                         ///< 队列最大长度
    bool shutdown_;                           ///< 是否已关闭，唤醒阻塞线程
    bool usedLog_;                            ///< 是否使用log
}; // class ThreadSafeQueue
}; // namespace thread_safe_queue
}; // namespace tools

#endif // THREAD_SAFE_QUEUE_H
