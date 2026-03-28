#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "game_config.h"
#include <string>

class ConfigManager {
public:
    static void initializeConfig(const std::string& configFile = "controls.cfg");
    static bool configExists(const std::string& configFile);
    static void createDefaultConfig(const std::string& configFile);
};

#endif