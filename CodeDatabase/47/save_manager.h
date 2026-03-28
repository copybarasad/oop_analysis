#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "game_state.h"
#include <string>
#include <memory>

class SaveManager {
public:
    static const std::string SAVE_FILE_PATH;
    
    static bool saveGame(const GameState& gameState, const std::string& filename = SAVE_FILE_PATH);
    static std::unique_ptr<GameState> loadGame(const std::string& filename = SAVE_FILE_PATH);
    static bool saveExists(const std::string& filename = SAVE_FILE_PATH);
    
private:
    static std::string getSaveFilePath(const std::string& filename);
};

#endif