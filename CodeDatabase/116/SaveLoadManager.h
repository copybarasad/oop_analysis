#pragma once
#include "Serializer.h"
#include "SaveLoadExceptions.h"
#include <filesystem>
#include <vector>
#include <string>

class GameContext;

class SaveLoadManager {
private:
    std::string saveDirectory = "saves";
    std::string defaultFilename = "savegame.json";
    
    std::string getSavePath() const;
    bool ensureSaveDirectory() const;
    
public:
    SaveLoadManager() = default;

    bool saveGame(GameContext& context);
    bool loadGame(GameContext& context);

    bool saveExists() const;

    void setSaveDirectory(const std::string& directory) { saveDirectory = directory; }
    std::string getSaveDirectory() const { return saveDirectory; }
    void setDefaultFilename(const std::string& filename) { defaultFilename = filename; }
    std::string getDefaultFilename() const { return defaultFilename; }
};