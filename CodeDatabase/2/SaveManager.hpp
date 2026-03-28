#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct SaveSlot {
    std::string filename;
    std::string displayName;
    bool exists;
    int level;

};

class SaveManager {
private:
    std::vector<SaveSlot> saveSlots;
    static const int MAX_SAVE_SLOTS = 2;

public:
    SaveManager();
    void refreshSaveSlots();
    bool saveExists(const std::string& filename) const;
    bool saveGame(int slot, const class GameCycle& game) const;
    bool loadGame(int slot, class GameCycle& game) const;
    bool deleteSave(int slot);
    const std::vector<SaveSlot>& getSaveSlots() const { return saveSlots; }
    std::string getSaveFilename(int slot) const;
};