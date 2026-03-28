#ifndef SPELL_HPP_INCLUDED
#define SPELL_HPP_INCLUDED

#include <string>
#include "position.hpp"
#include <vector>

// вперёд-объявления
class Field;
class Hero;
class Enemy;
class Spawner;

enum class TargetMode {
    None,      // без прицела
    Cell,      // одиночная клетка
    Area2x2,   // квадрат
    Adjacent   // соседняя клетка от героя
};

// бусты от заклинаний улучшения
class BoostState {
public:
    int directRadiusPlus{0};
    int aoeSizePlus{0};
    int trapDamagePlus{0};
    int summonCountPlus{0};
    int stacks{0}; // сколько применено(стакаем улучшения)

    void clear() { *this = {}; }
    bool has() const {
        return directRadiusPlus || aoeSizePlus || trapDamagePlus || summonCountPlus || stacks;
    }
};

// единый интерфейс заклинаний
class Spell {
public:
    virtual std::string name() const = 0;
    virtual TargetMode targeting() const = 0;
    virtual int radius() const = 0;

    virtual bool cast(Hero& hero,
                      Field& field,
                      std::vector<Enemy>& enemies,
                      std::vector<Spawner>& spawners,
                      BoostState& boosts,
                      const Position& target) = 0;
};

#endif
