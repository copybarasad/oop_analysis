#ifndef GAME_OBJECTFACTORY_H
#define GAME_OBJECTFACTORY_H

#include "Object.h"
#include <unordered_map>
#include <functional>

#include "../Entities/Player.h"
#include "../Entities/Ally.h"
#include "../Entities/Enemy.h"
#include  "../Entities/StrongerEnemy.h"

#include "../Environment/AllySpawner.h"
#include "../Environment/Block.h"
#include "../Environment/EnemyBase.h"
#include "../Environment/PlayerTrap.h"
#include "../Environment/SlowTrap.h"
#include "../Environment/SpellTower.h"

class ObjectFactory {
private:
    std::unordered_map<std::string, std::function<Object*()> > creators_ = {
        {"Player", []() -> Object * { return new Player(); }},
        {"Ally", []() -> Object * { return new Ally(); }},
        {"Enemy", []() -> Object * { return new Enemy(); }},
        {"StrongerEnemy", []() -> Object * { return new StrongerEnemy(); }},
        {"AllyBase", []() -> Object * { return new AllyBase(0); }},
        {"Block", []() -> Object * { return new Block(); }},
        {"EnemyBase", []() -> Object * { return new EnemyBase(); }},
        {"PlayerTrap", []() -> Object * { return new PlayerTrap(0); }},
        {"SlowTrap", []() -> Object * { return new SlowTrap(); }},
        {"SpellTower", []() -> Object * { return new SpellTower(); }}
    };

public:
    ObjectFactory() = default;

    Object *create(const std::string &objName);
};


#endif
