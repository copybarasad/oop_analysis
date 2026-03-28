#ifndef DARKTOWER_H
#define DARKTOWER_H

#include "entity.h"
#include "ispellcard.h"
#include "ispellconfig.h"
#include <memory>


class DarkTower : public Entity
{
public:
    DarkTower(int health=CharacterConfig::Health, int xPos=0, int yPos=0, int meleeDamage=SpellConfig::FireBallDamage/2);

    void takeDamage(int damage) override;
    int attack(Entity& other) override;
    void attackSpell(ISpellConfig& config);
    void towerUpdate();
    bool readyToAttack();

private:
    int m_attackInterval;
    std::unique_ptr<ISpellCard> m_card;
};

#endif // DARKTOWER_H
