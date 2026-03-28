#include "AreaDamageSpell.h"
#include "GameState.h"
#include "Enemy.h"
#include "MovementValidator.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(std::string spellName, int spellDamage, int spellManaCost, int spellRange, int spellAreaSize)
    : name(std::move(spellName)), damage(spellDamage), manaCost(spellManaCost), range(spellRange), areaSize(spellAreaSize) {}

bool AreaDamageSpell::cast(const Position& target, GameState& gameState, const Player& caster) {
    if (!canCast(caster.getPosition(), target, gameState)) {
        std::cout << "Цель вне радиуса действия заклинания " << name << "!" << std::endl;
        return false;
    }
    
    std::cout << name << " создает взрывную волну!" << std::endl;
    damageEnemiesInArea(target, gameState);
    return true;
}

void AreaDamageSpell::damageEnemiesInArea(const Position& center, GameState& gameState) {
    auto& enemies = gameState.getEnemies();
    int enemiesHit = 0;
    
    for (int yOffset = -areaSize/2; yOffset <= areaSize/2; ++yOffset) {
        for (int xOffset = -areaSize/2; xOffset <= areaSize/2; ++xOffset) {
            Position checkPos(center.x + xOffset, center.y + yOffset);
            
            if (!MovementValidator::isValidPosition(checkPos, gameState.getGameField())) {
                continue;
            }
            
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && enemy.getPosition() == checkPos) {
                    enemy.takeDamage(damage);
                    enemiesHit++;
                    std::cout << "Враг получает " << damage << " урона от взрывной волны!" << std::endl;
                }
            }
        }
    }
    
    if (enemiesHit == 0) {
        std::cout << "Взрывная волна не задела ни одного врага!" << std::endl;
    }
}