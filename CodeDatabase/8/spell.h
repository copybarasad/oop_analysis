#pragma once
#include <string>

class Field;
class Player;
class Enemy;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) = 0;
    virtual bool do_spell(int targetX, int targetY, Player& target, std::string& message) {
        return false;
    }
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual int getRange() const = 0;
};