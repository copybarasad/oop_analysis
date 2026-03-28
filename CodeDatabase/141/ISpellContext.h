#ifndef ISPELL_CONTEXT_H
#define ISPELL_CONTEXT_H

#include <string>
#include <vector>
#include "Enhancement.h"

class Field;
class Spell;
class Player;
class Enemy;
class EnemyBuilding;
class Trap;
class Ally;
class EnemyTower;

class ISpellContext {
public:
    virtual ~ISpellContext() = default;

    virtual Field &getField() = 0;

    virtual Player &getPlayer() = 0;

    virtual std::vector<Enemy> &getEnemies() = 0;

    virtual std::vector<EnemyBuilding> &getBuildings() = 0;

    virtual std::vector<Trap> &getTraps() = 0;

    virtual std::vector<Ally> &getAllies() = 0;

    virtual const std::vector<EnemyTower> &getTowers() const = 0;

    virtual void addMessage(const std::string &message) = 0;

    virtual void addScore() = 0;

    virtual void incrementKills(int count) = 0;

    virtual void addEnhancement(const Enhancement &enhancement) = 0;

    virtual void applyEnhancements(Spell &spell) = 0;
};

#endif
