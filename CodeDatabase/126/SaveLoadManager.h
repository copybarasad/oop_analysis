#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include <string>
#include "../gamelogic/GameState.h"
#include "../exceptions/GameException.h"

class SaveLoadManager {
    GameState& gameState;
    
public:
    SaveLoadManager(GameState& state);
    
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
};

#endif