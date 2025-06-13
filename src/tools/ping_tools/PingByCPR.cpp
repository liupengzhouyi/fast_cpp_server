#include "PingByCPR.h"
#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

namespace tools {
namespace PingTools {

bool PingFuncByCPR::PingURL(const std::string& url) {
    try {
        auto response = cpr::Get(cpr::Url{url}, cpr::Timeout{3000});
        spdlog::info("Ping {} - status code: {}", url, response.status_code);
        return response.status_code == 200;
    } catch (const std::exception& e) {
        spdlog::error("Ping {} failed: {}", url, e.what());
        return false;
    }
}

};  // namespace PingTools
};  // namespace tools