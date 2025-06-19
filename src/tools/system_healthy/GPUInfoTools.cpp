#include "GPUInfoTools.h"

namespace SystemHealthyTools {

GPUInfoTools::GPUInfoTools() {};

GPUInfoTools::~GPUInfoTools() {};

std::vector<SystemHealthy::GPUInfo> GPUInfoTools::CollectGPUInfo() {
    // 实际应调用 nvidia-smi, intel_gpu_top 等工具，示例中返回空
    return {};
}
}