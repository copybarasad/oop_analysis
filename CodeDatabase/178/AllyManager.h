#ifndef ALLY_MANAGER_H
#define ALLY_MANAGER_H

#include "Ally.h"
#include "Enemy.h"
#include "GameField.h"
#include "Player.h"
#include <vector>

class AllyManager {
private:
    GameField& field;
    
public:
    AllyManager(GameField& gameField);
    void updateAllies(std::vector<Ally> &allies, std::vector<Enemy> &enemies, Player &player);
    void addAlly(std::vector<Ally> &allies, const Ally &ally);
};

#endif