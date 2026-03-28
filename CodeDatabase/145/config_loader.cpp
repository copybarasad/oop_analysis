#include "config_loader.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

GameConfig ConfigLoader::loadConfig(const std::string& filename) {
    GameConfig config;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Config file not found: " << filename << ". Using default configuration.\n";
        return config;
    }
    
    GameConfig tempConfig;
    std::string line;
    int lineNumber = 0;
    bool hasErrors = false;
    
    while (std::getline(file, line)) {
        lineNumber++;

        if (line.empty() || line[0] == '#') continue;
        
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty() || line[0] == '#') continue;
        
        std::string command;
        char key;
        
        if (!parseConfigLine(line, command, key)) {
            std::cerr << "Invalid config line " << lineNumber << ": " << line << "\n";
            hasErrors = true;
            continue;
        }
        
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        
        applyConfigValue(tempConfig, command, key);
    }

    file.close();

    if (hasErrors) {
        std::cerr << "Configuration has errors. Using default configuration.\n";
        return GameConfig();
    }

    if (!tempConfig.validate()) {
        std::cerr << "Validation error: " << tempConfig.getValidationError() 
                 << ". Using default configuration.\n";
        return GameConfig();
    }
    
    std::cout << "Configuration loaded successfully from " << filename << "\n";
    return tempConfig;
}

bool ConfigLoader::saveConfig(const std::string& filename, const GameConfig& config) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw FileIOException(std::string("Cannot create config file: ") + filename);
    }
    
    try {
        file << "# Game Control Configuration\n";
        file << "# Format: command=key\n\n";
        
        file << "move_up=" << config.moveUpKey << "\n";
        file << "move_down=" << config.moveDownKey << "\n";
        file << "move_left=" << config.moveLeftKey << "\n";
        file << "move_right=" << config.moveRightKey << "\n";
        file << "switch_mode=" << config.switchModeKey << "\n";
        file << "ranged_attack=" << config.rangedAttackKey << "\n";
        file << "save_game=" << config.saveGameKey << "\n";
        file << "load_game=" << config.loadGameKey << "\n";
        file << "cast_spell=" << config.castSpellKey << "\n";
        file << "quit=" << config.quitKey << "\n";
        
        file.close();
        return true;
    } catch (const std::exception& ex) {
        throw FileIOException(std::string("Error writing config file: ") + ex.what());
    }
}

bool ConfigLoader::parseConfigLine(const std::string& line, std::string& command, char& key) {
    size_t eqPos = line.find('=');
    if (eqPos == std::string::npos || eqPos == 0 || eqPos == line.length() - 1) {
        return false;
    }
    
    command = line.substr(0, eqPos);
    std::string keyStr = line.substr(eqPos + 1);

    command.erase(command.find_last_not_of(" \t") + 1);
    keyStr.erase(0, keyStr.find_first_not_of(" \t"));
    keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
    
    if (keyStr.length() != 1) {
        return false;
    }
    
    key = keyStr[0];
    return true;
}

void ConfigLoader::applyConfigValue(GameConfig& config, const std::string& command, char key) {
    if (command == "move_up") {
        config.moveUpKey = key;
    } else if (command == "move_down") {
        config.moveDownKey = key;
    } else if (command == "move_left") {
        config.moveLeftKey = key;
    } else if (command == "move_right") {
        config.moveRightKey = key;
    } else if (command == "switch_mode") {
        config.switchModeKey = key;
    } else if (command == "ranged_attack") {
        config.rangedAttackKey = key;
    } else if (command == "save_game") {
        config.saveGameKey = key;
    } else if (command == "load_game") {
        config.loadGameKey = key;
    } else if (command == "cast_spell") {
        config.castSpellKey = key;
    } else if (command == "quit") {
        config.quitKey = key;
    }
}

std::string ConfigLoader::getDefaultConfigContent() {
    return R"(move_up=W
move_down=S
move_left=A
move_right=D
switch_mode=M
ranged_attack=X
save_game=Z
load_game=L
cast_spell=0
quit=Q
)";
}