#include "SaveManager.h"
#include <filesystem>
#include <algorithm>

#include "../common/Logger.h"

#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(dir) _mkdir(dir.c_str())
#else
#include <sys/stat.h>
#define CREATE_DIR(dir) mkdir(dir.c_str(), 0755)
#endif

SaveManager::SaveManager(const std::string& directory) : saveDirectory(directory) {
    ensureDirectoryExists();
}

bool SaveManager::ensureDirectoryExists() const {
    std::error_code ec;
    if (!std::filesystem::exists(saveDirectory, ec)) {
        return std::filesystem::create_directories(saveDirectory, ec);
    }
    return true;
}

std::string SaveManager::getSavePath(const std::string& saveName) const {
    return saveDirectory + saveName + ".sav";
}

bool SaveManager::saveGame(const std::string& saveName, const std::map<std::string, std::string>& gameData) {
    if (saveName.empty()) return false;
    
    if (!ensureDirectoryExists()) {
        Logger::getInstance().add("Error cant make dir");
        return false;
    }
    
    std::string filePath = getSavePath(saveName);
    std::ofstream file(filePath);
    
    if (!file.is_open()) {
        Logger::getInstance().add("Error: file cant be created " + filePath);
        return false;
    }

    std::string serializedData = serializeData(gameData);
    file << serializedData;
    file.close();
    
    Logger::getInstance().add("Game saved: " + saveName);
    return true;
}

std::map<std::string, std::string> SaveManager::loadGame(const std::string& saveName) {
    std::map<std::string, std::string> gameData;
    
    if (!saveExists(saveName)) {
        Logger::getInstance().add("Error: Save " + saveName + " not found");
        return gameData;
    }
    
    std::string filePath = getSavePath(saveName);
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        Logger::getInstance().add( "Error");
        return gameData;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string serializedData = buffer.str();
    file.close();
    
    gameData = deserializeData(serializedData);
    Logger::getInstance().add("Game loaded: " + saveName);
    
    return gameData;
}

bool SaveManager::deleteSave(const std::string& saveName) {
    if (!saveExists(saveName)) {
        return false;
    }
    
    std::string filePath = getSavePath(saveName);
    return std::remove(filePath.c_str()) == 0;
}

std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> saves;
    
    if (!ensureDirectoryExists()) {
        return saves;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".sav") {
                std::string filename = entry.path().stem().string();
                saves.push_back(filename);
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    
    // Сортируем по алфавиту
    std::sort(saves.begin(), saves.end());
    return saves;
}

bool SaveManager::saveExists(const std::string& saveName) {
    std::string filePath = getSavePath(saveName);
    return std::filesystem::exists(filePath);
}

void SaveManager::setSaveDirectory(const std::string& directory) {
    saveDirectory = directory;
    if (!saveDirectory.empty() && saveDirectory.back() != '/') {
        saveDirectory += '/';
    }
    ensureDirectoryExists();
}

std::string SaveManager::serializeData(const std::map<std::string, std::string>& data) const {
    std::stringstream ss;
    for (const auto& [key, value] : data) {
        std::string escapedKey = key;
        std::string escapedValue = value;

        std::replace(escapedKey.begin(), escapedKey.end(), '\n', '\\');
        std::replace(escapedValue.begin(), escapedValue.end(), '\n', '\\');
        
        ss << escapedKey << "=" << escapedValue << "\n";
    }
    return ss.str();
}

std::map<std::string, std::string> SaveManager::deserializeData(const std::string& data) const {
    std::map<std::string, std::string> result;
    std::stringstream ss(data);
    std::string line;
    
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            std::replace(key.begin(), key.end(), '\\', '\n');
            std::replace(value.begin(), value.end(), '\\', '\n');
            
            result[key] = value;
        }
    }
    
    return result;
}