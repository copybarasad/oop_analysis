#pragma once
#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

class ControlConfig {
private:
    std::unordered_map<std::string, std::string> keyToCommand;
    std::unordered_map<std::string, std::string> commandToKey;
    std::unordered_map<std::string, std::string> defaultConfig;

    void setupDefaultConfig() {
        defaultConfig = {
            {"w", "move_up"},
            {"a", "move_left"},
            {"s", "move_down"},
            {"d", "move_right"},

            {"e", "attack"},
            {"q", "cast_spell"},
            {"r", "change_mode"},

            {"0", "exit_to_menu"},
            {"p", "save_game"},
            {"h", "help"},
            {"u", "show_stats"}
        };
    }

    std::string toLower(const std::string& str) const {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

public:
    ControlConfig() {
        setupDefaultConfig();
        resetToDefault();
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Warning: Cannot open config file '" << filename
                << "'. Using default controls." << std::endl;
            return false;
        }

        std::unordered_map<std::string, std::string> tempKeyToCommand;
        std::unordered_map<std::string, std::string> tempCommandToKey;

        std::string line;
        int lineNumber = 0;

        while (std::getline(file, line)) {
            lineNumber++;

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            std::string key, command;

            if (iss >> key >> command) {
                key = toLower(key);

                tempKeyToCommand[key] = command;
                tempCommandToKey[command] = key;
            }
            else {
                std::cerr << "Warning: Invalid format at line " << lineNumber
                    << " in config file." << std::endl;
            }
        }

        file.close();

        if (!validateLoadedConfig(tempKeyToCommand, tempCommandToKey)) {
            std::cerr << "Invalid configuration. Reverting to defaults." << std::endl;
            return false;
        }

        keyToCommand = std::move(tempKeyToCommand);
        commandToKey = std::move(tempCommandToKey);

        std::cout << "Control configuration loaded successfully from '"
            << filename << "'" << std::endl;
        return true;
    }

    bool validateLoadedConfig(const std::unordered_map<std::string, std::string>& ktc,
        const std::unordered_map<std::string, std::string>& ctk) const {
        std::vector<std::string> requiredCommands = {
            "move_up", "move_left", "move_down", "move_right",
            "attack", "cast_spell", "change_mode",
            "exit_to_menu", "save_game", "help", "show_stats"
        };

        for (const auto& cmd : requiredCommands) {
            if (ctk.find(cmd) == ctk.end()) {
                std::cerr << "Error: Required command '" << cmd << "' is not mapped" << std::endl;
                return false;
            }
        }

        for (const auto& [key, command] : ktc) {
            auto range = ktc.equal_range(key);
            int count = std::distance(range.first, range.second);
            if (count > 1) {
                std::cerr << "Error: Key '" << key << "' is mapped to multiple commands" << std::endl;
                return false;
            }
        }

        for (const auto& [command, key] : ctk) {
            auto range = ctk.equal_range(command);
            int count = std::distance(range.first, range.second);
            if (count > 1) {
                std::cerr << "Error: Command '" << command << "' is mapped to multiple keys" << std::endl;
                return false;
            }
        }

        return true;
    }

    void resetToDefault() {
        keyToCommand.clear();
        commandToKey.clear();

        for (const auto& [key, command] : defaultConfig) {
            keyToCommand[key] = command;
            commandToKey[command] = key;
        }

        std::cout << "Control configuration reset to defaults." << std::endl;
    }

    std::string getCommand(const std::string& key) const {
        std::string lowercaseKey = toLower(key);

        auto it = keyToCommand.find(lowercaseKey);
        if (it != keyToCommand.end()) {
            return it->second;
        }

        return ""; 
    }

    std::string getKey(const std::string& command) const {
        auto it = commandToKey.find(command);
        if (it != commandToKey.end()) {
            return it->second;
        }

        return ""; 
    }

    bool hasCommand(const std::string& key) const {
        std::string lowercaseKey = toLower(key);
        return keyToCommand.find(lowercaseKey) != keyToCommand.end();
    }

    void showConfig() const {
        std::cout << "\n=== CURRENT CONTROL CONFIG ===" << std::endl;
        for (const auto& [key, command] : keyToCommand) {
            std::cout << "Key: '" << key << "' -> Command: '" << command << "'" << std::endl;
        }
        std::cout << "===============================" << std::endl;
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot save config to file '" << filename << "'" << std::endl;
            return false;
        }

        file << "# Control configuration file\n";
        file << "# Format: <key> <command>\n";
        file << "# Example: w move_up\n\n";

        file << "# Movement\n";
        file << "w move_up\n";
        file << "a move_left\n";
        file << "s move_down\n";
        file << "d move_right\n\n";

        file << "# Actions\n";
        file << "e attack\n";
        file << "q cast_spell\n";
        file << "r change_mode\n\n";

        file << "# System\n";
        file << "0 exit_to_menu\n";
        file << "p save_game\n";
        file << "h help\n";
        file << "u show_stats\n";

        file.close();
        std::cout << "Default configuration saved to '" << filename << "'" << std::endl;
        return true;
    }

    std::vector<std::string> getAllCommands() const {
        std::vector<std::string> commands;
        for (const auto& [cmd, key] : commandToKey) {
            commands.push_back(cmd);
        }
        return commands;
    }

    std::string getHelpText() const {
        std::stringstream ss;
        ss << "\n=== CONTROLS ===" << std::endl;

        std::vector<std::pair<std::string, std::string>> movement = {
            {"move_up", "Move up"},
            {"move_left", "Move left"},
            {"move_down", "Move down"},
            {"move_right", "Move right"}
        };

        std::vector<std::pair<std::string, std::string>> actions = {
            {"attack", "Attack"},
            {"cast_spell", "Cast spell"},
            {"change_mode", "Change attack mode"}
        };

        std::vector<std::pair<std::string, std::string>> system = {
            {"exit_to_menu", "Exit to menu"},
            {"save_game", "Save game"},
            {"help", "Show help"},
            {"show_stats", "Show player stats"}
        };

        ss << "Movement:" << std::endl;
        for (const auto& [cmd, desc] : movement) {
            std::string key = getKey(cmd);
            if (!key.empty()) {
                ss << "  " << key << " - " << desc << std::endl;
            }
        }

        ss << "\nActions:" << std::endl;
        for (const auto& [cmd, desc] : actions) {
            std::string key = getKey(cmd);
            if (!key.empty()) {
                ss << "  " << key << " - " << desc << std::endl;
            }
        }

        ss << "\nSystem:" << std::endl;
        for (const auto& [cmd, desc] : system) {
            std::string key = getKey(cmd);
            if (!key.empty()) {
                ss << "  " << key << " - " << desc << std::endl;
            }
        }

        ss << "================" << std::endl;
        return ss.str();
    }
};

#endif 