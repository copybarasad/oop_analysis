#include "enemy_tower.h"
#include "lightning_spell.h"
#include <iostream>
#include "entity.h"
#include "game_field.h"

EnemyTower::EnemyTower(int x, int y, int tower_damage, int tower_radius, int tower_cooldown)
    : position_x(x), position_y(y),health_points(50), damage(tower_damage), radius(tower_radius), 
      cooldown_turns(tower_cooldown), current_cooldown(0)
{
    tower_spell = new LightningSpell(damage, radius);
}

EnemyTower::~EnemyTower() {
    delete tower_spell;
}

int EnemyTower::getPositionX() const {
    return position_x;
}

int EnemyTower::getPositionY() const {
    return position_y;
}

int EnemyTower::getDamage() const {
    return tower_spell->getDamage();
}

int EnemyTower::getRadius() const {
    return radius;
}

int EnemyTower::get_health() const {
    return health_points;
}

bool EnemyTower::is_destroyed() const {
    return health_points <= 0;
}

bool EnemyTower::canAttack() const {
    return current_cooldown == 0;
}

void EnemyTower::attack() {
    current_cooldown = cooldown_turns;
}

void EnemyTower::reduceCooldown() {
    if (current_cooldown > 0) {
        current_cooldown--;
    }
}

void EnemyTower::take_damage(int dmg) {
    health_points -= dmg;
    if (health_points < 0) {
        health_points = 0;
    }
    
    std::cout << "Tower at (" << position_x << ", " << position_y << ") took " << dmg << " damage! Tower HP: " << health_points << std::endl;
    
    if (is_destroyed()) {
        std::cout << "Tower at (" << position_x << ", " << position_y  << ") is destroyed!" << std::endl;
    }
}

void EnemyTower::castTowerSpell(game_field* field, Entity* target, 
                               int tower_x, int tower_y, int target_x, int target_y) {
    if (tower_spell == nullptr || target == nullptr)
        return;

    if (tower_spell->canCast(field, tower_x, tower_y, target_x, target_y)) {
        tower_spell->apply(field, tower_x, tower_y, target_x, target_y, target);
    }
}

