#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H


#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

class SaveManager {
private:
    std::string saveDirectory;

public:
    SaveManager(const std::string& directory = "saves/");

    bool saveGame(const std::string& saveName, const std::map<std::string, std::string>& gameData);
    std::map<std::string, std::string> loadGame(const std::string& saveName);
    bool deleteSave(const std::string& saveName);
    std::vector<std::string> listSaves();
    bool saveExists(const std::string& saveName);

    std::string getSavePath(const std::string& saveName) const;
    void setSaveDirectory(const std::string& directory);

private:
    bool ensureDirectoryExists() const;
    std::string serializeData(const std::map<std::string, std::string>& data) const;
    std::map<std::string, std::string> deserializeData(const std::string& data) const;
};


#endif