#include "EnemyTower.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(int x, int y, int health, int damage, int attack_radius)
    : Entity(health, damage, x, y),
      attack_radius_(attack_radius),
      can_attack_(true),
      weakened_damage_(damage)
{}

int EnemyTower::GetAttackRadius() const {
    return attack_radius_;
}

bool EnemyTower::GetCanAttack() const { 
    return can_attack_;
}

void EnemyTower::SetCanAttack(bool can_attack) {
    can_attack_ = can_attack;
}
void EnemyTower::SetAttackRadius(int attack_radius) {
    attack_radius_ = attack_radius;
}

void EnemyTower::Tick(Player& player) {

    int dx = std::abs(player.GetX() - GetX());
    int dy = std::abs(player.GetY() - GetY());

    if (dx <= attack_radius_ && dy <= attack_radius_) {
        if (can_attack_) {
            std::cout << "Вражеская башня атакует игрока ослабленным заклинанием!" << "\n";
            player.TakeSpellDamage(weakened_damage_, true);
            can_attack_ = false;
        } else {
            std::cout << "Вражеская башня перезаряжается, поэтому не может атаковать в этот ход." << "\n";
            can_attack_ = true;
        }
    } else {
        can_attack_ = true;
    }
}

std::string EnemyTower::GetDescription() const {
    return "Вражеская башня (неуязвима) на (" + 
        std::to_string(GetX()) + "," + std::to_string(GetY()) + ")";
}

void EnemyTower::Attack(Entity&) {
}

void EnemyTower::TakeSpellDamage(int, bool) {
}