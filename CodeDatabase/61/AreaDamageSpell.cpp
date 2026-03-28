#include "AreaDamageSpell.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <cmath>
#include <algorithm>
#include <string>

AreaDamageSpell::AreaDamageSpell(int radius, int damage, int areaSize)
    : Spell(radius, damage), enhancedAreaSize(areaSize) {}

SpellResult AreaDamageSpell::use(Game& game, int targetX, int targetY) {
    const Player& player = game.getPlayer();
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Вычисляем расстояние до центра области
    int dx = targetX - playerX;
    int dy = targetY - playerY;
    double distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance > baseRadius) {
        return SpellResult(false, true, "Target is out of range!");
    }
    
    const GameField& field = game.getField();
    if (!field.isValidPosition(targetX, targetY)) {
        return SpellResult(false, true, "Invalid target position!");
    }
    
    // Вычисляем область поражения (центр в targetX, targetY)
    int halfSize = enhancedAreaSize / 2;
    int startX = std::max(0, targetX - halfSize);
    int startY = std::max(0, targetY - halfSize);
    int endX = std::min(field.getWidth() - 1, targetX + halfSize);
    int endY = std::min(field.getHeight() - 1, targetY + halfSize);
    
    int enemiesHit = 0;
    int buildingsHit = 0;
    
    // Наносим урон всем врагам и зданиям в области
    auto& enemies = game.getEnemies();
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        int ex = enemy.getPositionX();
        int ey = enemy.getPositionY();
        if (ex >= startX && ex <= endX && ey >= startY && ey <= endY) {
            enemy.takeDamage(baseDamage);
            enemiesHit++;
            if (!enemy.isAlive()) {
                // Очищаем позицию мертвого врага
                game.getField().clearPosition(enemy.getPositionX(), enemy.getPositionY());
                game.onEnemyKilled();
            }
        }
    }
    
    auto& buildings = game.getBuildings();
    for (auto& building : buildings) {
        int bx = building.getPositionX();
        int by = building.getPositionY();
        if (bx >= startX && bx <= endX && by >= startY && by <= endY) {
            building.takeDamage(baseDamage);
            buildingsHit++;
        }
    }
    
    return SpellResult(true, true, "Area damage spell cast! Hit " + 
                      std::to_string(enemiesHit) + " enemies and " + 
                      std::to_string(buildingsHit) + " buildings.");
}

std::string AreaDamageSpell::getName() const {
    return "Area Damage";
}

std::string AreaDamageSpell::getDescription() const {
    return "Deals " + std::to_string(baseDamage) + " damage in a " + 
           std::to_string(enhancedAreaSize) + "x" + std::to_string(enhancedAreaSize) + 
           " area within " + std::to_string(baseRadius) + " tiles.";
}

void AreaDamageSpell::applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) {
    Spell::applyEnhancement(radiusBonus, damageBonus, areaBonus, summonBonus);
    enhancedAreaSize += areaBonus;
    if (enhancedAreaSize < 2) enhancedAreaSize = 2;
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}

