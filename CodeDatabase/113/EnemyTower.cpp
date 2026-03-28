#include "EnemyTower.h"
#include "GameField.h"
#include "Player.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(int health, int damage, int attackRange, int x, int y)
    : GameObject(health, health, x, y), m_damage(damage),
    m_attackRange(attackRange), m_canAttackThisTurn(true) {}

void EnemyTower::makeMove(GameField& field, Player& player) {
    if (!isAlive() || !m_canAttackThisTurn) return;

    int distance = std::abs(player.getPositionX() - getPositionX()) +
        std::abs(player.getPositionY() - getPositionY());

    if (distance <= m_attackRange) {
        int actualDamage = m_damage / 2;
        player.takeDamage(actualDamage);
        std::cout << "Enemy tower attacks player for " << actualDamage << " damage!" << std::endl;
        m_canAttackThisTurn = false;
    }
}