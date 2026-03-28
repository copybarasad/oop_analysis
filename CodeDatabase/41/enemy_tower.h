#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "building.h"
#include "spell_direct_damage.h"
#include "constants.h"

class Enemy_tower : public Building{
private:
    int attack_range;
    int attack_damage;
    int attack_cooldown;
    int current_cooldown;
    bool can_attack;

public:
    Enemy_tower(const std::string& building_name, int pos_x, int pos_y, Game_field* game_field,
                int health = ENEMY_TOWER_HEALTH, int range = ENEMY_TOWER_ATTACK_RANGE,
                int damage = ENEMY_TOWER_DAMAGE, int cooldown = ENEMY_TOWER_ATTACK_COOLDOWN);
    void update() override;
    bool can_attack_player() const;
    void perform_attack();
};

#endif