#pragma once
#include <set>
#include "EventSystem.h"

class InputConfigLoader {
public:
    using Mapping = std::vector<std::pair<char, std::string>>;

    InputConfigLoader() = default;

    bool LoadConfig(const std::string& filename);

    const Mapping& GetMapping() const { return mapping_; }
    LogTarget GetLogTarget() const { return logTarget; }
    const std::string& GetLogFile() const { return logFile; }

private:
    Mapping mapping_;
    LogTarget logTarget;
    std::string logFile;

    Mapping DefaultMapping() const {
        return {
            {'w', "MoveUp"},
            {'s', "MoveDown"},
            {'a', "MoveLeft"},
            {'d', "MoveRight"},
            {'f', "Attack"},
            {'b', "OpenShop"},
            {'e', "OpenHand"},
            {'m', "SwitchMode"},
            {'v', "Save"},
            {'l', "Load"},
            {'q', "Quit"}
        };
    }
};
