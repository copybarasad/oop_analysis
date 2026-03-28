#ifndef TOWER_HPP
#define TOWER_HPP
#include "tower.hpp"

class MageTower: public Tower{
    int totalCooldown;
    int currentCooldown;
    char icon = 'M';
    int attackRadius;
    int spellDamage = 20;
public:
    MageTower(int startHealth, int startDamage, int startX, int startY, int cooldown, int radius);
    char getIcon()  override { return icon; }

    bool isReady() const { return currentCooldown == 0; }
    void resetCooldown() { currentCooldown = totalCooldown; }
    void tickCooldown() { if (currentCooldown > 0) currentCooldown--; }
    int getAttackRadius() const { return attackRadius; }
    void setAttackRadius(int radius){attackRadius=radius;}
    int getSpellDamage() const { return spellDamage; }
};

#endif