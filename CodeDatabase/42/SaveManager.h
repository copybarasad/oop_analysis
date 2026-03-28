#pragma once
#include "SaveData.h"
#include "EnemyTower.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message, const std::string& filename = "")
        : std::runtime_error("Save/Load Error [" + filename + "]: " + message) {}
};

class SaveManager {
private:
    const int MAX_SAVE_SLOTS = 3;
    
    void createBackup(const std::string& filename) const;
    void restoreBackup(const std::string& filename) const;
    bool writeToFile(const SaveData& data, const std::string& filename);
    bool readFromFile(SaveData& data, const std::string& filename) const;
    void validateSaveSlot(int slot) const;
    
public:
    SaveManager();
    
    void restoreTowerState(EnemyTower& tower, const SavedTower& savedTower) const;
    
    bool saveGame(const SaveData& data, int slot);
    bool loadGame(SaveData& data, int slot);
    bool deleteSave(int slot);
    
    bool saveExists(int slot) const;
    std::vector<int> getAvailableSaves() const;
    std::string getSaveInfo(int slot) const;
    
    static std::string getSlotFileName(int slot);
    static std::string getSlotBackupFileName(int slot);
};