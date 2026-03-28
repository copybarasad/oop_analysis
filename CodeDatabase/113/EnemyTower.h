#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "GameObject.h"
#include "IAttacker.h"

// Предварительное объявление
class GameField;
class Player;

class EnemyTower : public GameObject, public IAttacker {
private:
    int m_damage;
    int m_attackRange;
    bool m_canAttackThisTurn;

public:
    EnemyTower(int health, int damage, int attackRange, int x, int y);
    void resetTurn() { m_canAttackThisTurn = true; }
    int getDamage() const override { return m_damage; }
    AttackType getAttackType() const override { return AttackType::Ranged; }

    int getAttackRange() const { return m_attackRange; }
    void makeMove(GameField& field, Player& player);
};

#endif // ENEMYTOWER_H