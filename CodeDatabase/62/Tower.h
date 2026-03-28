#pragma once
#include <iostream>
#include "Building.h"
#include "Entity.h"
#include "Constants.h"

class Spell;
class GameField;

class Tower: public Building {
private:
    std::function<void(int, int, int, float)> applyDamage_;
    Spell* spell_;
    int turns_to_attack;
    int lasted_turns;
    int radius;

    int health_;
    int max_health_;
public:
    Tower(Spell* spell, std::function<void(int, int, int, float)> applyDamage, int turns_to_attack, int radius,
        int health);
    ~Tower() = default;

    int getRadius() const { return radius; }

    void makeTurn(GameField* field);

    int getMaxHealth() const { return max_health_; }
    int getHealth() const { return health_; }

    void applyDamage(int damage) { health_ -= damage; if(health_ < 0) health_ = 0; }

    Spell* getSpell() const { return spell_; }

    int getTurnsToAttack() const { return turns_to_attack; }
    int getLastedTurns() const { return lasted_turns; }

    void setLastedTurns(int turns) { lasted_turns = turns; }
};