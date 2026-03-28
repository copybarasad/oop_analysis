#include "AreaDamageSpell.h"
#include "Field.h"
#include "GameVisualize.h"
#include "Player.h"

#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize,
                               const std::string& name, 
                               const std::string& description,
                               int manaCost)
    : name(name), description(description), manaCost(manaCost),
      damage(damage), range(range), areaSize(areaSize), enhanced(false) {}

bool AreaDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    if (player.getMana() < manaCost) {
        visualizer.visualizeInsufficientResource("mana", manaCost, player.getMana());
        return false;
    }
    
    auto playerPos = field.getPlayerPosition();
    int playerX = playerPos.first;
    int playerY = playerPos.second;
    
    // Проверяем радиус (манхэттенское расстояние)
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        visualizer.visualizeSpellTargetIssue("out_of_range", distance, range);
        return false;
    }
    
    // Проверяем, что цель в пределах поля
    if (targetX < 0 || targetX >= field.getWidth() || targetY < 0 || targetY >= field.getHeight()) {
        visualizer.visualizeSpellTargetIssue("out_of_bounds");
        return false;
    }
    
    int actualAreaSize = enhanced ? areaSize + 1 : areaSize;
    int actualDamage = enhanced ? damage + 5 : damage;
    
    bool hitSomething = false;
    
    // Наносим урон по области
    for (int dx = 0; dx < actualAreaSize; ++dx) {
        for (int dy = 0; dy < actualAreaSize; ++dy) {
            int currentX = targetX + dx;
            int currentY = targetY + dy;
            
            // Проверяем границы поля
            if (currentX >= 0 && currentX < field.getWidth() && 
                currentY >= 0 && currentY < field.getHeight()) {
                
                // Наносим урон врагам в этой клетке
                if (field.hasEnemy(currentX, currentY)) {
                    auto& enemies = field.getEnemies();
                    auto& enemyPositions = field.getEnemyPositions();
                    
                    for (size_t i = 0; i < enemyPositions.size(); ++i) {
                        if (enemyPositions[i].first == currentX && enemyPositions[i].second == currentY) {
                            enemies[i].takeDamage(actualDamage);
                            hitSomething = true;
                            visualizer.visualizeSpellEffect("area", "hit", currentX, currentY, actualDamage);
                        }
                    }
                }
            }
        }
    }
    
    // ВСЕГДА тратим ману при успешном касте в радиусе (даже если никого не задели)
    player.setMana(player.getMana() - manaCost);
    
    if (hitSomething) {
        // Используем renderOperationResult с деталями о касте
        visualizer.visualizeOperationResult("spell", "success", 
            "Area at (" + std::to_string(targetX) + "," + std::to_string(targetY) + 
            ") size " + std::to_string(actualAreaSize) + 
            ". Remaining mana: " + std::to_string(player.getMana()));
    } else {
        visualizer.visualizeSpellEffect("area", "miss", targetX, targetY);
    }
    return true;
}

void AreaDamageSpell::enhance(int damageBonus, int areaBonus) {
    damage += damageBonus;
    areaSize += areaBonus;
    enhanced = true;
}

void AreaDamageSpell::increaseDamage(int amount) {
    damage += amount;
}

void AreaDamageSpell::increaseArea(int amount) {
    areaSize += amount;
}