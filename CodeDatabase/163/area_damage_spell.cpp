#include "area_damage_spell.hpp"
#include "game_state.hpp"
#include "game_object.hpp"
#include "map.hpp"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& name, const std::string& desc, 
                               int spellRange, int spellDamage, int size)
    : SpellCard(name, desc, spellRange), damage(spellDamage), areaSize(size) {
}

void AreaDamageSpell::castAsProjectile(GameState* gameState, int dirX, int dirY) const {
    if (!gameState) return;
    
    Player* player = gameState->getPlayer();
    if (!player) return;
    
    Map* map = gameState->getMap();
    if (!map) return;
    
    int startX = player->getPositionX();
    int startY = player->getPositionY();
    
    std::cout << name << " летит из (" << startX << ", " << startY 
              << ") в направлении (" << dirX << ", " << dirY 
              << ") на дальность " << range << "!\n";
    
    int hitX = startX;
    int hitY = startY;
    bool obstacleHit = false;
    int distance = 0;
    
    for (distance = 0; distance < range; distance++) {
        hitX += dirX;
        hitY += dirY;
        
        std::cout << "  Проверяем клетку (" << hitX << ", " << hitY << ")\n";
        
        if (!map->isValidPosition(hitX, hitY)) {
            hitX -= dirX;
            hitY -= dirY;
            std::cout << name << " достиг края карты и взрывается в (" << hitX << ", " << hitY << ")\n";
            obstacleHit = true;
            break;
        }
        
        const Cell& cell = map->getCell(hitX, hitY);
        
        bool hasTarget = false;
        for (const auto& enemy : gameState->getEnemies()) {
            if (enemy && enemy->isAlive() && 
                enemy->getPositionX() == hitX && 
                enemy->getPositionY() == hitY) {
                hasTarget = true;
                break;
            }
        }
        
        if (!hasTarget) {
            for (const auto& building : gameState->getBuildings()) {
                if (building && building->isAlive() && 
                    building->getPositionX() == hitX && 
                    building->getPositionY() == hitY) {
                    hasTarget = true;
                    break;
                }
            }
        }
        
        if (hasTarget) {
            std::cout << name << " попал в цель и взрывается в (" << hitX << ", " << hitY << ")!\n";
            obstacleHit = true;
            break;
        }
        
        if (!cell.isPassable() && cell.getCellType() != Cell::Type::SLOWING) {
            std::cout << name << " попал в препятствие и взрывается в (" << hitX << ", " << hitY << ")!\n";
            obstacleHit = true;
            break;
        }
    }
    
    if (!obstacleHit) {
        std::cout << name << " достиг максимальной дальности и взрывается в (" << hitX << ", " << hitY << ")!\n";
    }
    
    int halfSize = areaSize / 2;
    int startAreaX = hitX - halfSize;
    int startAreaY = hitY - halfSize;
    
    std::cout << "Область взрыва от (" << startAreaX << ", " << startAreaY 
              << ") до (" << startAreaX + areaSize - 1 << ", " << startAreaY + areaSize - 1 << ")\n";
    
    int targetsHit = 0;
    
    for (int x = startAreaX; x < startAreaX + areaSize; x++) {
        for (int y = startAreaY; y < startAreaY + areaSize; y++) {
            if (map->isValidPosition(x, y)) {
                for (const auto& enemy : gameState->getEnemies()) {
                    if (enemy && enemy->isAlive() && 
                        enemy->getPositionX() == x && enemy->getPositionY() == y) {
                        enemy->takeDamage(damage);
                        targetsHit++;
                    }
                }
                
                for (const auto& building : gameState->getBuildings()) {
                    if (building && building->isAlive() && 
                        building->getPositionX() == x && building->getPositionY() == y) {
                        building->takeDamage(damage);
                        targetsHit++;
                    }
                }
            }
        }
    }
    
    std::cout << "Взрыв попал в " << targetsHit << " цель(и).\n";
}

std::unique_ptr<SpellCard> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}