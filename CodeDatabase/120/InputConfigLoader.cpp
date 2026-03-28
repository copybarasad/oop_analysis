#include "InputConfigLoader.h"
bool InputConfigLoader::LoadConfig(const std::string& filename) {
    Mapping map;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: failed to open controls file, using default mapping.\n";
        mapping_ = DefaultMapping();
        logTarget = LogTarget::Screen;
        return false;
    }

    std::set<std::string> commandsFound;
    std::set<char> keysFound;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos || pos == 0 || pos + 1 >= line.size()) {
            std::cerr << "Warning: invalid line: " << line << "\n";
            mapping_ = DefaultMapping();
            logTarget = LogTarget::Screen;
            return false;
        }

        std::string keyStr = line.substr(0,pos);
        std::string value = line.substr(pos+1);


        if(keyStr == "LogTarget") {
            if(value == "Screen") logTarget = LogTarget::Screen;
            else if(value == "File") logTarget = LogTarget::File;
            else {
                std::cerr << "Warning: unknown LogTarget value: " << value << ", using Screen\n";
                logTarget = LogTarget::Screen;
            }
            continue;
        }
        if(keyStr == "LogFile") {
            logFile = value;
            continue;
        }


        char key = keyStr[0];
        if (keysFound.count(key) || commandsFound.count(value)) {
            std::cerr << "Warning: duplicate key/command, using default.\n";
            mapping_ = DefaultMapping();
            logTarget = LogTarget::Screen;
            return false;
        }
        keysFound.insert(key);
        commandsFound.insert(value);
        map.emplace_back(key, value);
    }


    for (const auto& [k, cmd] : DefaultMapping()) {
        if (commandsFound.find(cmd) == commandsFound.end()) {
            std::cerr << "Warning: missing command " << cmd << ", using default.\n";
            mapping_ = DefaultMapping();
            logTarget = LogTarget::Screen;
            return false;
        }
    }

    mapping_ = map;
    return true;
}
