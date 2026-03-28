#include "DirectDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include "GameVisualize.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range,
                                   const std::string& name,
                                   const std::string& description, 
                                   int manaCost)
    : name(name), description(description), manaCost(manaCost),
      damage(damage), range(range), enhanced(false) {}

bool DirectDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    // Проверяем, достаточно ли маны у игрока
    if (player.getMana() < manaCost) {
        visualizer.visualizeInsufficientResource("mana", manaCost, player.getMana());
        return false;
    }
    
    // Получаем позицию игрока
    auto playerPos = field.getPlayerPosition();
    int playerX = playerPos.first;
    int playerY = playerPos.second;
    
    // Проверяем, находится ли цель в радиусе действия (манхэттенское расстояние)
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    int actualRange = enhanced ? range + 2 : range;
    
    if (distance > actualRange) {
        visualizer.visualizeSpellTargetIssue("out_of_range", distance, actualRange);
        return false;
    }
    
    // Проверяем, что цель в пределах поля
    if (targetX < 0 || targetX >= field.getWidth() || targetY < 0 || targetY >= field.getHeight()) {
        visualizer.visualizeSpellTargetIssue("out_of_bounds");
        return false;
    }
    
    // Применяем улучшение если есть
    int actualDamage = enhanced ? damage + 10 : damage;
    
    // Наносим урон врагу или зданию
    bool success = false;
    
    // Проверяем, есть ли враг на целевой клетке
    if (field.hasEnemy(targetX, targetY)) {
        // Находим врага и наносим урон
        auto& enemies = field.getEnemies();
        auto& enemyPositions = field.getEnemyPositions();
        
        for (size_t i = 0; i < enemyPositions.size(); ++i) {
            if (enemyPositions[i].first == targetX && enemyPositions[i].second == targetY) {
                enemies[i].takeDamage(actualDamage);
                success = true;
                visualizer.visualizeSpellEffect("direct", "hit", targetX, targetY, actualDamage);
                break;
            }
        }
    } else {
        visualizer.visualizeSpellEffect("direct", "miss", targetX, targetY);
    }
    
    // Тратим ману только если заклинание было успешно применено
    if (success) {
        player.setMana(player.getMana() - manaCost);
    }
    
    return success;
}

void DirectDamageSpell::setDamage(int newDamage) {
    damage = newDamage;
}

void DirectDamageSpell::setRange(int newRange) {
    range = newRange;
}

void DirectDamageSpell::enhance(int damageBonus, int rangeBonus) {
    damage += damageBonus;
    range += rangeBonus;
    enhanced = true;
}

void DirectDamageSpell::increaseDamage(int amount) {
    damage += amount;
}

void DirectDamageSpell::increaseRange(int amount) {
    range += amount;
}
