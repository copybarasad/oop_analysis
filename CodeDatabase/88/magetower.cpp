#include "magetower.hpp"

MageTower::MageTower(int startHealth, int startDamage, int startX, int startY, int cooldown, int radius) 
    : Tower(startHealth, startDamage, startX, startY, cooldown), 
      totalCooldown(cooldown), currentCooldown(0), attackRadius(radius) {}

