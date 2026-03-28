#pragma once
#include "Entity.h"
#include "Player.h"

class EnemyTower : public Entity {
public:
    EnemyTower(int x, int y, int health = 0, int damage = 2, int attack_radius = 1);

    int GetAttackRadius() const;
    bool GetCanAttack() const;

    void SetCanAttack(bool can_attack);
    void SetAttackRadius(int attack_radius);

    void Tick(Player& player);
    std::string GetDescription() const; 

    void Attack(Entity&) override;
    void TakeSpellDamage(int damage, bool can_slow = false) override;

private:
    int attack_radius_;
    bool can_attack_;
    int weakened_damage_;
};
