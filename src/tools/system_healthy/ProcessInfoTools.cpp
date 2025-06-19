#include "ProcessInfoTools.h"

namespace SystemHealthyTools {

ProcessInfoTools::ProcessInfoTools() {};

ProcessInfoTools::~ProcessInfoTools() {};

std::vector<SystemHealthy::ProcessInfo> ProcessInfoTools::CollectProcessInfo() {
    // 可通过遍历 /proc 目录下的 pid 获取状态，示例中返回空
    return {};
}
}
