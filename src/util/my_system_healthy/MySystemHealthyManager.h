// SystemHealthyManager.h
#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include "SystemHealthy.pb.h"
#include "MyLog.h"

namespace MySystemHealthy {

class MySystemHealthyManager {

public:
  static MySystemHealthyManager& GetInstance();

  void Init(int update_interval_sec = 5); // 可传入采样周期
  const SystemHealthy::SystemInfo GetSystemInfo();
  void Shutdown();

private:
  MySystemHealthyManager();
  ~MySystemHealthyManager();
  void WorkerLoop();

  std::thread worker_;
  std::mutex mutex_;
  std::atomic<bool> running_;
  SystemHealthy::SystemInfo current_info_;
  int interval_sec_ = 5;
};

}; 
