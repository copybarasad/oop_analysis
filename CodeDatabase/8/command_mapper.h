#pragma once
#include "control_config.h"
#include <string>

class CommandMapper {
private:
    ControlConfig config;
    
public:
    CommandMapper();
    bool initialize(const std::string& configFile = "controls.config");
    std::string mapInputToCommand(const std::string& input) const;
    const ControlConfig& getConfig() const;
    std::string getFormattedCommand(const std::string& commandName) const;
};