#include "NetInfoTools.h"
#include <fstream>
#include <sstream>

namespace MySystemHealthy {

NetInfoTools::NetInfoTools() {};

NetInfoTools::~NetInfoTools() {};

SystemHealthy::NetInfo NetInfoTools::CollectNetInfo() {
    SystemHealthy::NetInfo info;
    std::ifstream file("/proc/net/dev");
    std::string line;
    int skip = 2;
    while (skip--) std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string iface;
        ss >> iface;
        uint64_t recv, dummy, trans;
        ss >> recv;
        for (int i = 0; i < 7; ++i) ss >> dummy;
        ss >> trans;

        info.set_recv_bytes(info.recv_bytes() + recv);
        info.set_transmit_bytes(info.transmit_bytes() + trans);
    }
    return info;
}
}
