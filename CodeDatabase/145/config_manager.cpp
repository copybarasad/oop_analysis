#include "config_manager.h"
#include "config_loader.h"
#include <fstream>
#include <iostream>

void ConfigManager::initializeConfig(const std::string& configFile) {
    if (!configExists(configFile)) {
        std::cout << "Configuration file not found. Creating default configuration...\n";
        createDefaultConfig(configFile);
    }
}

bool ConfigManager::configExists(const std::string& configFile) {
    std::ifstream file(configFile);
    return file.good();
}

void ConfigManager::createDefaultConfig(const std::string& configFile) {
    try {
        GameConfig defaultConfig;
        ConfigLoader::saveConfig(configFile, defaultConfig);
        std::cout << "Default configuration created: " << configFile << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Failed to create default configuration: " << ex.what() << "\n";
    }
}