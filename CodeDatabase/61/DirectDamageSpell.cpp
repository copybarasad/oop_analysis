#include "DirectDamageSpell.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <cmath>
#include <string>

DirectDamageSpell::DirectDamageSpell(int radius, int damage)
    : Spell(radius, damage), enhancedRadius(radius) {}

SpellResult DirectDamageSpell::use(Game& game, int targetX, int targetY) {
    const Player& player = game.getPlayer();
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Вычисляем расстояние до цели
    int dx = targetX - playerX;
    int dy = targetY - playerY;
    double distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance > enhancedRadius) {
        return SpellResult(false, true, "Target is out of range!");
    }
    
    // Проверяем, есть ли враг или здание на целевой клетке
    const GameField& field = game.getField();
    if (!field.isValidPosition(targetX, targetY)) {
        return SpellResult(false, true, "Invalid target position!");
    }
    
    const Cell& targetCell = field.getCell(targetX, targetY);
    bool hasTarget = targetCell.hasEnemyOnCell() || targetCell.hasBuildingOnCell();
    
    if (!hasTarget) {
        return SpellResult(false, true, "No enemy or building at target location!");
    }
    
    // Наносим урон
    bool hitEnemy = false;
    bool hitBuilding = false;
    
    // Находим врага на этой клетке
    auto& enemies = game.getEnemies();
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPositionX() == targetX && enemy.getPositionY() == targetY) {
            enemy.takeDamage(baseDamage);
            hitEnemy = true;
            if (!enemy.isAlive()) {
                // Очищаем позицию мертвого врага
                game.getField().clearPosition(enemy.getPositionX(), enemy.getPositionY());
                game.onEnemyKilled();
            }
            break;
        }
    }
    
    // Находим здание на этой клетке
    auto& buildings = game.getBuildings();
    for (auto& building : buildings) {
        if (building.getPositionX() == targetX && building.getPositionY() == targetY) {
            building.takeDamage(baseDamage);
            hitBuilding = true;
            break;
        }
    }
    
    if (hitEnemy || hitBuilding) {
        return SpellResult(true, true, "Direct damage spell cast! Dealt " + 
                          std::to_string(baseDamage) + " damage.");
    }
    
    return SpellResult(false, true, "Spell failed!");
}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage";
}

std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(baseDamage) + " damage to enemy or building within " + 
           std::to_string(enhancedRadius) + " tiles.";
}

void DirectDamageSpell::applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) {
    Spell::applyEnhancement(radiusBonus, damageBonus, areaBonus, summonBonus);
    enhancedRadius += radiusBonus;
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}

