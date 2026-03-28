#ifndef CHARACTER_FACTORY_H
#define CHARACTER_FACTORY_H

#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include <memory>

class CharacterFactory {
public:
    static std::unique_ptr<Player> createPlayer(const Position &startPosition, int health, int meleeDmg, int rangedDmg);
    static std::unique_ptr<Enemy> createEnemy(const Position &startPosition, int health, int damage);
    static std::unique_ptr<Ally> createAlly(const Position &pos, int health, int damage);
};

#endif