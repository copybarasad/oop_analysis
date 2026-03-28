#include "../include/entity/enemy_tower.h"

EnemyTower::EnemyTower() 
    : Entity(250), 
      spawn_interval_(5), 
      turns_until_spawn_(5),
      damage_(15),
      attack_range_(2),
      attack_cooldown_(3),
      current_attack_cooldown_(0) {}

EnemyTower::EnemyTower(int health, int spawn_interval, int damage, int attack_range, int attack_cooldown) 
    : Entity(health), 
      spawn_interval_(spawn_interval), 
      turns_until_spawn_(spawn_interval),
      damage_(damage),
      attack_range_(attack_range),
      attack_cooldown_(attack_cooldown),
      current_attack_cooldown_(0) {}

int EnemyTower::spawn_interval() const { 
    return this->spawn_interval_; 
}

int EnemyTower::turns_until_spawn() const { 
    return this->turns_until_spawn_; 
}

int EnemyTower::damage() const {
    return this->damage_;
}

int EnemyTower::attack_range() const {
    return this->attack_range_;
}

bool EnemyTower::tick() {
    if (!is_alive()) { return false; }
    
    this->turns_until_spawn_--;
    if (this->turns_until_spawn_ <= 0) {
        this->turns_until_spawn_ = this->spawn_interval_;
        return true;
    }
    return false;
}

void EnemyTower::reset_spawn_timer() {
    this->turns_until_spawn_ = this->spawn_interval_;
}

bool EnemyTower::tick_attack() {
    if (!is_alive()) { return false; }

    this->current_attack_cooldown_--;
    if (this->current_attack_cooldown_ <= 0) {
        this->current_attack_cooldown_ = this->attack_cooldown_;
        return true;
    }

    return false;
}