#pragma once
#include "Entity.h"
#include <vector>

class Enemy;

enum class AttackType {
    MELEE,
    RANGED
};

class Player : public Entity {
public:
    Player(int health, int damage, int x, int y);

    int GetScore() const;
    AttackType GetAttackType() const;
    std::string GetDescription() const override;
    int GetMaxHealth() const;

    void SetScore(int score);
    void SetAttackType(AttackType type);
    void SetMaxHealth(int health);

    void AddScore(int amount);
    void SwitchAttack();
    void Attack(Entity& target) override;
    void AttackEnemies(std::vector<Enemy>& enemies);

    void TakeSpellDamage(int damage, bool can_slow = false) override;

private:
    int max_health_;
    int score_;
    int long_damage_;
    AttackType attack_type_;
};
