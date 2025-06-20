#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <vector>
#include <map>

class ArgumentParser {
public:
    ArgumentParser();

    void addOption(const std::string& shortOpt,
                   const std::string& longOpt,
                   const std::string& description,
                   bool requiresValue = false);

    std::vector<std::map<std::string, std::string>> parse(int argc, char* argv[]);

    void printHelp() const;

private:
    struct Option {
        std::string shortOpt;
        std::string longOpt;
        std::string description;
        bool requiresValue;
    };

    std::vector<Option> options_;
};

#endif // ARGUMENT_PARSER_H
