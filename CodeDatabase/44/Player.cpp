#include <iostream>

#include "Player.h"
#include "Enemy.h"
#include "Field.h"

constexpr int kMeleeRange = 1;
constexpr int kRangedRange = 3;

Player::Player(int health, int damage, int x, int y)
    : Entity(health, damage, x, y),
      max_health_(health),
      score_(0),
      long_damage_(std::max(1, damage - 2)),
      attack_type_(AttackType::MELEE) {}

int Player::GetScore() const {
    return score_;
}

int Player::GetMaxHealth() const {
    return max_health_;
}

AttackType Player::GetAttackType() const {
    return attack_type_;
}

void Player::SetMaxHealth(int health) {
    max_health_ = health;
}

void Player::SetScore(int score) {
    score_ = score;
}

void Player::SetAttackType(AttackType type) {
    attack_type_ = type;
}

void Player::AddScore(int amount) {
    score_ += amount;
    if (score_ < 0) {
        score_ = 0;
    }
}

std::string Player::GetDescription() const {
    return "HP: " + std::to_string(GetHealth()) +
           " | Score: " + std::to_string(GetScore()) +
           " | Режим атаки: " + (GetAttackType() == AttackType::MELEE ? "ближний" : "дальний");
}

void Player::SwitchAttack() {
    if (attack_type_ == AttackType::MELEE) {
        attack_type_ = AttackType::RANGED;
    } else {
        attack_type_ = AttackType::MELEE;
    }

    std::cout << "Вы переключились на "
              << (attack_type_ == AttackType::MELEE ? "ближний" : "дальний")
              << " бой." << std::endl;
}

void Player::Attack(Entity& target) {
    if (&target == this) {
        throw std::logic_error("Player cannot attack itself");
    }

    int damage_to_deal = (attack_type_ == AttackType::MELEE) ? damage_ : long_damage_;

    std::cout << "Вы нанесли " << damage_to_deal
              << (damage_to_deal > 1 ? " единицы" : " единицу") <<" урона " 
              << (attack_type_ == AttackType::MELEE ? "ближним" : "дальним") 
              << " боем!" << std::endl;

    target.TakeDamage(damage_to_deal);
}

void Player::AttackEnemies(std::vector<Enemy>& enemies){
    bool attack_flag = false;
    for (auto& enemy : enemies) {
        int dist_x = abs(enemy.GetX() - GetX());
        int dist_y = abs(enemy.GetY() - GetY());

        if ((dist_x <= kMeleeRange && dist_y <= kMeleeRange && GetAttackType() == AttackType::MELEE) ||
            (dist_x <= kRangedRange && dist_y <= kRangedRange && GetAttackType() == AttackType::RANGED)) {
            Attack(enemy);
            attack_flag = true;
        }
    }

    if (!attack_flag) {
        std::cout << "Враги находятся слишком далеко!" << std::endl;
    }
}

void Player::TakeSpellDamage(int damage, bool can_slow) {
    TakeDamage(damage);
    if (can_slow) {
        SetSlowed(true);
        std::cout << "Игрок получил " << damage << (damage > 1 ? " единицы" : " единицу") <<  " урона и оглушен на 1 ход!\n";
    } else {
        std::cout << "Игрок получил " << damage << (damage > 1 ? " единицы" : " единицу") << " урона!\n";
    }
}