#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "game_config.h"
#include <string>

class ConfigLoader {
public:
    static GameConfig loadConfig(const std::string& filename);
    static bool saveConfig(const std::string& filename, const GameConfig& config);
    
private:
    static bool parseConfigLine(const std::string& line, std::string& command, char& key);
    static void applyConfigValue(GameConfig& config, const std::string& command, char key);
    static std::string getDefaultConfigContent();
};

#endif