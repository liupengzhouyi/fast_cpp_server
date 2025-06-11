#include "ThreadSafeBuffer.h"

namespace tools {

void ThreadSafeBuffer::push(const std::vector<char>& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    buffer_.push(data);
}

bool ThreadSafeBuffer::pop(std::vector<char>& outData) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (buffer_.empty()) {
        return false;
    }
    outData = std::move(buffer_.front());
    buffer_.pop();
    return true;
}

size_t ThreadSafeBuffer::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return buffer_.size();
}

} // namespace tools