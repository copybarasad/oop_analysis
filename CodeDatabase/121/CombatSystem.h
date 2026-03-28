#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Player.h"
#include "GameField.h"
#include "TextSystem.h"
#include <vector>
#include <memory>

class Enemy;
class EnemyBuilding;
class EnemyTower;

class CombatSystem {
private:
    std::shared_ptr<TextSystem> textSystem;

public:
    CombatSystem(std::shared_ptr<TextSystem> textSystem);
    
    bool playerAttack(Player& player, GameField& field,
                     std::vector<Enemy>& enemies,
                     std::vector<EnemyBuilding>& buildings,
                     std::vector<EnemyTower>& towers);
    
    void enemyAttack(Enemy& enemy, Player& player);
};

#endif