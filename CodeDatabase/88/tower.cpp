#include "tower.hpp"

Tower::Tower(int startHealth, int startDamage, int startX, int startY, int cooldown) 
    : Character(startHealth, startDamage, startX, startY), 
      totalCooldown(cooldown), 
      currentCooldown(0) {}

bool Tower::isTimerUp() {
    if (currentCooldown == totalCooldown) {
        currentCooldown = 0;
        return true;
    }
    currentCooldown++;
    return false;
}