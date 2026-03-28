#ifndef GAMESAVEMANAGER_H
#define GAMESAVEMANAGER_H

#include "GameState.h"
#include <string>
#include <fstream>

// RAII wrapper for file handling
class FileGuard {
private:
    std::fstream file_;
    std::string filename_;

public:
    FileGuard(const std::string& path, std::ios::openmode mode);
    ~FileGuard();
    
    // Disable copy
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;
    
    std::fstream& get() { return file_; }
    bool isOpen() const { return file_.is_open(); }
    const std::string& getFilename() const { return filename_; }
};

class GameSaveManager {
private:
    static constexpr const char* SAVE_FILE = "game_save.dat";
    static constexpr const char* AUTO_SAVE_FILE = "auto_save.dat";
    static constexpr const char* SAVE_VERSION = "1.0";

public:
    GameSaveManager() = default;
    
    // Manual save/load
    void saveGame(const GameState& state);
    GameState loadGame();
    
    // Auto save/load (for level transitions and one-time revival)
    void autoSave(const GameState& state);
    GameState loadAutoSave();
    
    // Check if saves exist
    bool hasSaveFile() const;
    bool hasAutoSave() const;
    
    // Delete save files
    void deleteSaveFile();
    void deleteAutoSave();
    void deleteAllSaves();
};

#endif

