#pragma once

#include <string>

namespace tools {
namespace PingTools {

class PingFuncByCPR {
public:
    static bool PingURL(const std::string& url);
}; // class PingFunc

}; // namespace PingTools
}; // namespace tools
