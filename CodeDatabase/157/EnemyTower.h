#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Building.h"

class Player;
class GameField;

class EnemyTower : public Building {
public:
    EnemyTower(int x, int y, int hp, int damage, int range, int cooldownTurns = 1);
    virtual ~EnemyTower() = default;
    bool tryAttack(Player& player, GameField& field) override;
    void tick() override;
    int cooldown() const { return cooldownCounter_; }  // Добавлен getter
private:
    int damage_;
    int range_;
    int cooldownTurns_;
    int cooldownCounter_;
};
#endif // ENEMY_TOWER_H
