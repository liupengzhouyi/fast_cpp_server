#ifndef INIT_TOOLS_H
#define INIT_TOOLS_H
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>


namespace tools {
namespace init_tools {

    bool checkConfigLoadStatus(bool load_ini_config_status, bool load_json_config_status, bool load_yaml_config_status);

    void loadConfigFromArguments(
        const std::vector<std::map<std::string, std::string>>& args, 
        std::vector<std::string>& logInfos, 
        std::string& configFilePath);

    bool initLoadConfig(const std::string& type_,
        const std::string& config_file_path,
        std::vector<std::string>& logInfos);
};
};

#endif // INIT_TOOLS_H