#include "directdamage.h"
#include "FightManager.h"

DirectDamage::DirectDamage(int damage, int radius): damage(damage), radius(radius) {}

SpellResult DirectDamage::UseSpell(std::vector<Enemy>& enemies, int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions, int targetX, int targetY) {
    
    SpellResult result;
    bool IsDamage = false;
    
    for(size_t i = 0; i < enemies.size(); i++) {
        int dist = FightManager::SearchDistance(playerX, playerY, enemyPositions[i]);
        if (dist <= radius) {
            enemies[i].Damage(damage);
            IsDamage = true;
            break;
        }
    }

    if (IsDamage) {
        result.success = true;
        result.message = "Вы нанесли урон заклинанием прямого урона";
        result.enemiesHit = 1;
    } else {
        result.success = false;
        result.message = "Врагов рядом нет";
        result.enemiesHit = 0;
    }
    
    return result;
}

std::string DirectDamage::GetName() const {
    return "Заклинание прямого урона";
}



