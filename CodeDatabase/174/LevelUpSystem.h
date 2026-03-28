#ifndef LEVELUPSYSTEM_H
#define LEVELUPSYSTEM_H

#include "Player.h"
#include <iostream>

class LevelUpSystem {
public:
    enum class Upgrade {
        HEALTH,
        DAMAGE, 
        SPELL_CAPACITY
    };

    LevelUpSystem(Player& player);
    void performLevelUp(int completedLevel);

private:
    Player& player;
    
    void showLevelUpMenu() const;
    Upgrade getPlayerUpgradeChoice() const;
    void applyUpgrade(Upgrade upgrade);
};

#endif