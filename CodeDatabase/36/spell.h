#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <utility>
#include <vector>

class Field;
class Player;
class Enemy;
class EnemyTower;
class EnemyBase;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool Cast(Field& field, Player& player, const std::pair<int,int>& target,
                     std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases) = 0;
    virtual std::string Name() const = 0;
    virtual std::string Description() const = 0;
    virtual void Enhance() {}
    virtual bool CanBeEnhanced() const { return false; }
};

#endif