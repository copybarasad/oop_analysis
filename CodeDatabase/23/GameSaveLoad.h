#ifndef GAME_SAVE_LOAD_H
#define GAME_SAVE_LOAD_H

#include "GameState.h"
#include "GameExceptions.h"
#include <string>
#include <fstream>

class GameSaveLoad {
public:
    static const std::string SAVE_FILE_EXTENSION;
    static const std::string DEFAULT_SAVE_FILE;
    
    static void saveGame(const GameState& state, const std::string& filename);
    static GameState loadGame(const std::string& filename);
    static bool saveFileExists(const std::string& filename);
    
private:
    static void validateGameState(const GameState& state, const std::string& filename);
};

#endif 

