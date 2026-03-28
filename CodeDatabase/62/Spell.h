#pragma once

#include <iostream>
#include "Constants.h"

#include "GameField.h"

enum class SpellType {
    InstantDamage = 0,
    Health = 1,
    SplashDamage = 2,
    Trap = 3,
    Boost = 4,
    Ally = 5
};

inline std::string spellTypeToString(SpellType type) {
    switch (type) {
        case SpellType::InstantDamage: return INSTANT_DAMAGE_SPELL;
        case SpellType::Health:        return HEALTH_SPELL;
        case SpellType::SplashDamage:  return SPLASH_DAMAGE_SPELL;
        case SpellType::Trap:          return TRAP_SPELL;
        case SpellType::Boost:         return BOOST_SPELL;
        case SpellType::Ally:          return ALLY_SPELL;
        default:                       return "";
    }
}
class Spell {
protected:
    int radius;
    int value;
    int dist;
    SpellType name;
public:
    Spell(int radius, int value, int dist, SpellType name): radius(radius), value(value), dist(dist), name(name) {};
    ~Spell() = default;
    
    const SpellType getName() const { return name; }
    int getRadius() const { return radius; }
    int getValue() const { return value; }
    int getDist() const { return dist; }
    virtual bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) = 0;
    virtual bool use(GameField& gameField, Tower* tower) = 0;

    virtual Spell* clone() const = 0;
};
