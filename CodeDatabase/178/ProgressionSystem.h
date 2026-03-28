#ifndef PROGRESSION_SYSTEM_H
#define PROGRESSION_SYSTEM_H

#include "GameController.h"
#include "LevelManager.h"
#include "PlayerProgression.h"

class Game;

class ProgressionSystem {
public:
    void handleLevelTransition(Game& game);
    
private:
    void applyLevelUp(GameController& controller);
    void showUpgradeMenu(GameController& controller, LevelManager& levelManager);
    bool processUpgradeChoice(GameController& controller, LevelManager& levelManager, char choice);
};

#endif