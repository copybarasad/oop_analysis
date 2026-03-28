#pragma once
#include "Entity.h"
#include "DirectSpell.h"

class EnemyTower : public Entity {
private:
    int delay_turns;
    int current_delay = 0;
    DirectSpell spell;
public:
    EnemyTower(std::pair<int, int> position, float health, int maxHealth, int damage, int range, int delay_turns) :
    Entity(position, health, maxHealth, damage), delay_turns(delay_turns), spell(damage, range, Spell::Type::Direct) {}
    EnemyTower() : Entity({0,0},0,0,0), delay_turns(0), spell(0,0,Spell::Type::Direct) {}

    void attack(Game &game, std::pair<int, int> target);
    json to_json() override;
    bool from_json(json& j) override;
};
