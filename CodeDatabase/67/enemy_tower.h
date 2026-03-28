#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "lightning_spell.h"
#include "game_field.h"
#include "entity.h"

class EnemyTower {
private:
    int position_x;
    int position_y;
    int damage;
    int radius;
    int cooldown_turns;
    int current_cooldown;
    int health_points;
    LightningSpell* tower_spell;
    
public:
    EnemyTower(int x, int y, int tower_damage, int tower_radius, int tower_cooldown);
    ~EnemyTower();
    int getPositionX() const;
    int getPositionY() const;
    int getDamage() const;
    int getRadius() const;
    int get_health() const;
    bool canAttack() const;
    bool is_destroyed() const;
    void attack();
    void reduceCooldown();
    void take_damage(int dmg);

    void castTowerSpell(game_field* field, Entity* target, int tower_x, int tower_y, int target_x, int target_y);
};

#endif
