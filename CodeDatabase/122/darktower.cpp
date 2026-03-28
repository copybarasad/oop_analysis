#include "darktower.h"
#include "fireballcard.h"

DarkTower::DarkTower(int health, int xPos, int yPos, int meleeDamage)
    : Entity(health, xPos, yPos,  meleeDamage)
{
    m_card = std::make_unique<FireBallCard>(SpellConfig::FireBallSpellRadius - 1, SpellConfig::FireBallDamage / 2);
}

void DarkTower::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health <= 0)
        kill();
}

int DarkTower::attack(Entity &other)
{
    bool isMeleeRange = isInRange(other, BuildingsConfig::TowerDamageRadius);

    if (isMeleeRange)
    {
        other.takeDamage(m_meleeDamage);
        return m_meleeDamage;
    }
    return 0;
}

void DarkTower::attackSpell(ISpellConfig &config)
{
    m_card->useCard(config);
}

void DarkTower::towerUpdate()
{
    --m_attackInterval;
}

bool DarkTower::readyToAttack()
{
    if (m_attackInterval < 0 && isAlive())
    {
        m_attackInterval = BuildingsConfig::TowerAttackInterval;
        return true;
    }
    return false;
}

