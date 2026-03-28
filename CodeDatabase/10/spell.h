#pragma once
#include <string>

class Field;
class Player;
class Enemy;

class Spell {
public:
    virtual ~Spell() = default;
    virtual int cast_spell(int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player) = 0;
    virtual int cast_spell(int, int, int, int, Field&) {
        return 0;
    }
    virtual int getCost() const = 0;
    virtual int getRange() const = 0;
    virtual std::string getName() const = 0;
};
