#ifndef PLAYER_H
#define PLAYER_H

#include "game_entity.h"
#include <utility>

enum class CombatMode {
    MELEE,
    RANGED
};

class Player : public GameEntity {
private:
    std::pair<int, int> position;
    CombatMode combatMode;
    int meleeDamage;
    int rangedDamage;
    
public:
    Player(int startX, int startY);
    
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    void move(int dx, int dy);
    
    CombatMode getCombatMode() const;
    void switchCombatMode();
    
    int getMeleeDamage() const;
    int getRangedDamage() const;
    
    void increaseMeleeDamage(int amount);
    void increaseRangedDamage(int amount);
    
    bool isEnemyInMeleeRange(const std::pair<int, int>& enemyPos) const;
    bool isEnemyInRangedRange(const std::pair<int, int>& enemyPos) const;
};

#endif