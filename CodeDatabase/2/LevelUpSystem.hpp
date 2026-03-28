#pragma once
#include "Player.hpp"
#include "PlayersHand.hpp"
#include <iostream>

class LevelUpSystem {
private:
    int availablePoints;

public:
    LevelUpSystem();
    void addLevelUpPoints(int points = 2);
    bool improvePlayerHealth(Player& player);
    bool improvePlayerMana(Player& player);
    bool improvePlayerDamage(Player& player);
    int getAvailablePoints() const;
    void reset();
};