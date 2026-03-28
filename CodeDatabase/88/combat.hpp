#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "../map/gamemap.hpp"
#include "../entities/player.hpp"
#include "../entities/enemy.hpp"
#include "../entities/character.hpp"


class Combat {
private:
    GameMap& gameMap;
    
public:
    Combat(GameMap& map);
    
    bool performMeleeAttack(Character* attacker, Character* target);
    bool performRangedAttack(Player* player, Character* target);
    Character* findRangedTarget(Player* player, int currentX, int currentY, int directionX, int directionY);
    void handleDefeatedEnemy(Character* enemy, bool isRanged = false);
};

#endif