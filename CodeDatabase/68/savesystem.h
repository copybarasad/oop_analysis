#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <string>
#include "gameexceptions.h"

// Forward declaration
class GameManager;

class SaveSystem {
public:
    void saveGameState(const GameManager& game, const std::string& filename);
    void loadGameState(GameManager& game, const std::string& filename);
};

#endif