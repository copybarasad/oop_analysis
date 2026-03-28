#pragma once
#include "GameEnt.h"
#include "Player.h"
#include "GameField.h"
#include "TowerAttackSpell.h"
#include <iostream>

class EnemyTower : public GameEnt {
private:
    TowerAttackSpell spell;   // собственное заклинание башни
    int cooldownTurns;        // сколько ходов осталось до восстановления
    const int maxCooldown;    // длительность перезарядки

public:
    EnemyTower(int x, int y, double damage = 2.0, int range = 1, int cooldown = 1);

    GameEnt* clone() const override { return new EnemyTower(*this); }

    void tryAttack(Field& field, Player& player);
    void reduceCooldown();
};
