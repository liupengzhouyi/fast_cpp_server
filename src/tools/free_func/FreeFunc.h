#ifndef FREE_FUNC_H
#define FREE_FUNC_H
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "MyLog.h"

namespace tools {
namespace free_func {


void logWelcomeMessage();

bool loadLogConfigFromINIConfig(std::vector<std::string>& logInfos,
                                std::string& logDirPath,
                                std::string& logFilePath,
                                std::string& appName);

bool checkConfigLoadStatus(bool load_ini_config_status, bool load_json_config_status, bool load_yaml_config_status);

bool showMyConfig(const std::string& type_);
};
};

#endif // FREE_FUNC_H