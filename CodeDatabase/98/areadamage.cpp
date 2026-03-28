#include "areadamage.h"
#include "FightManager.h"

AreaDamage::AreaDamage(int damage, int radius): damage(damage), radius(radius) {}

SpellResult AreaDamage::UseSpell(std::vector<Enemy>& enemies, int playerX, int playerY,  const std::vector<std::pair<int,int>>& enemyPositions, int targetX, int targetY) { 
    
    SpellResult result;
    std::pair<int, int> ForDamageSpell = std::make_pair(targetX, targetY);
    double dist = FightManager::SearchDistance(playerX, playerY, ForDamageSpell);
    
    if (dist > radius) {
        result.success = false;
        result.message = "Ты слишком далеко! Расстояние: " + std::to_string(dist) + 
                        ", максимально: " + std::to_string(radius);
        return result;
    }

    int enemiesHit = 0;
    for(size_t i = 0; i < enemies.size(); i++) {
        int ex = enemyPositions[i].first;
        int ey = enemyPositions[i].second;
        
        if(ex >= targetX && ex < targetX + 2 && ey >= targetY && ey < targetY + 2) {
            enemies[i].Damage(damage);
            enemiesHit++;
        }
    }

    result.success = true;
    result.enemiesHit = enemiesHit;
    if (enemiesHit > 0) {
        result.message = "Ты попала по " + std::to_string(enemiesHit) + " врагам";
    } else {
        result.message = "Ты не попала по врагам";
    }
    
    return result;
}

std::string AreaDamage::GetName() const {
    return "Заклинание урона по площади";
}

