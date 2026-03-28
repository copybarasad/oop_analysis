#pragma once
#include "Enemy.hpp"

#include "./Field/Cell.hpp"
#include "./Field/Field.hpp"
#include "Entity.hpp"
#include "./Spells/Fireball.hpp"

class MagicStatue: public Enemy {
private:
    int RADIUS_ATTACK = 3;
    int ATTACK_FREQUENCY = 3;
    InterfaceDirectDamageSpell* attackSpell;


public: 
    MagicStatue(int x, int y, int offsetX, int offsetY, int level = 1);

    ~MagicStatue() {
        delete attackSpell;
    };

    void attack(Field& field, InterfaceGameLogger& logger_consol, int frequency);
    int get_radius_attack() {return RADIUS_ATTACK; }
    void move_towards_target(Field&, InterfaceGameLogger&) override {}

};
