#include "ArgumentParser.h"
#include <iostream>
#include <sstream>

ArgumentParser::ArgumentParser() {}

void ArgumentParser::addOption(const std::string& shortOpt,
                               const std::string& longOpt,
                               const std::string& description,
                               bool requiresValue) {
    options_.push_back({shortOpt, longOpt, description, requiresValue});
}

std::vector<std::map<std::string, std::string>> ArgumentParser::parse(int argc, char* argv[]) {
    std::vector<std::map<std::string, std::string>> parsedArgs;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        for (const auto& opt : options_) {
            if (arg == opt.shortOpt || arg == opt.longOpt) {
                std::map<std::string, std::string> entry;
                entry["key"] = arg;
                if (opt.requiresValue) {
                    if (i + 1 < argc) {
                        entry["value"] = argv[++i];
                    } else {
                        entry["value"] = ""; // 或者抛异常
                    }
                } else {
                    entry["value"] = "true";
                }
                parsedArgs.push_back(entry);
                break;
            }
        }
    }

    return parsedArgs;
}

void ArgumentParser::printHelp() const {
    std::cout << "Available options:\n";
    for (const auto& opt : options_) {
        std::cout << "  " << opt.shortOpt << ", " << opt.longOpt;
        if (opt.requiresValue) std::cout << " <value>";
        std::cout << "\n    " << opt.description << "\n";
    }
}
