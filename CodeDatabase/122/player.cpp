#include "player.h"
#include "enemy.h"

#include <iostream>

Player::Player(int health, int mana, int xPos, int yPos, int meleeDamage, int rangedDamage)
    : Entity(health, xPos, yPos, meleeDamage), m_rangedDamage(rangedDamage), m_typeBattle(MELEE), m_hands(new HandsPlayer(mana)) {}

void Player::move(const Cell &cell, int dx, int dy)
{
    m_xPos += dx;
    m_yPos += dy;

    if (cell.getType() == CellType::SLOWED) {
        m_effect = DELAYED;
    }
}

int Player::attack(Entity& other)
{
    bool isMeleeRange = isInRange(other, 1);
    bool isRangedRange = isInRange(other, 2);

    if (isMeleeRange)
    {
        other.takeDamage(m_meleeDamage);
        m_hands->getMana() += CharacterConfig::MeleeAttackPoints;
        return m_meleeDamage;
    }

    else if (isRangedRange && m_typeBattle == RANGED) {
        Enemy* enemy = dynamic_cast<Enemy*>(&other);
        if (enemy) {
            enemy->trigger();
        }
        other.takeDamage(m_rangedDamage);
        m_hands->getMana() += CharacterConfig::RangedAttackPoints;
        return m_rangedDamage;
    }
    return 0;
}

void Player::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health <= 0)
        kill();
}

void Player::changeTypeBattle()
{
    m_typeBattle = (m_typeBattle == MELEE) ? RANGED : MELEE;
}
