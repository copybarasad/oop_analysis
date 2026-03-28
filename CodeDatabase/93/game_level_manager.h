#ifndef GAME_LEVEL_MANAGER_H
#define GAME_LEVEL_MANAGER_H

#include "game_controller.h"

class GameLevelManager {
public:
    static void advanceToNextLevel(GameController& game);
};

#endif