#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "game_entity.h"
#include <utility>

class Player;
class GameField;


class EnemyTower : public GameEntity {
private:
    std::pair<int, int> position;
    int attackRadius;
    int spellDamage;           
    int cooldownCounter;       
    int cooldownDuration;      
    
public:
    EnemyTower(int x, int y, int attackRadius = 5, int spellDamage = 15, int cooldown = 2);
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool isPlayerInRange(const std::pair<int, int>& playerPos) const;
    bool canAttack() const;
    int getSpellDamage() const;
    int getAttackRadius() const;
    void update();
    void performAttack();
    int getScore() const;
    int getCooldownCounter() const;
    void setCooldownCounter(int c);
};

#endif