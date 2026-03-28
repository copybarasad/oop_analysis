#pragma once
#include "PlayingField.h"
#include "Player.h"
#include "DirectDamageSpell.h"
#include <iostream>
#include <memory>

using namespace std;

class EnemyTower {
private:
    int x, y;
    int hp;
    int maxHp;
    int attackRange;
    int cooldown; // 0-башня может атаковать
    int maxCooldown; // макс время перезарядки (в ходах)
    shared_ptr<DirectDamageSpell> spell;
    string name;
    int damage;

public:
    EnemyTower(int posX, int posY, int hp, int maxHp = 100, int range = 3, int cooldownTurns = 2);
    
    int GetX() const;
    int GetY() const;
    int GetAttackRange() const;
    string GetName() const;
    bool CanAttack() const;
    int GetDamage() const;

    void TakeDamage(int damage);
    int GetHp() const;
    int GetMaxHp() const;
    void SetHp(int newHp);
    void SetMaxHp(int newMaxHp);
    void SetDamage(int newDamage);
    
    void SetPosition(int posX, int posY);
    void Update(); 
    bool IsPlayerInRange(const Player& player) const;
    void AttackPlayer(Player& player);
    void DisplayInfo() const;
};