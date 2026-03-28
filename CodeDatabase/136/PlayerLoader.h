#ifndef PLAYERLOADER_H
#define PLAYERLOADER_H

#include "Wanderer.h"
#include "GameField.h"
#include "GameController.h"
#include <fstream>

class PlayerLoader {
private:
    std::ifstream& file;
    Wanderer& player;
    GameField& field;
    GameController& gameController;

public:
    PlayerLoader(std::ifstream& file, Wanderer& player, GameField& field, GameController& controller);
    bool loadPlayerData();
    
private:
    bool loadPlayerStats();
    bool validatePlayerData(int health, int maxHealth, int shield, int maxShield, 
                           int mana, int maxMana, int points, int level) const;
};

#endif