#include "direct_damage_spell.hpp"
#include "game_state.hpp"
#include "game_object.hpp"
#include "map.hpp"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& name, const std::string& desc, 
                                   int spellRange, int spellDamage)
    : SpellCard(name, desc, spellRange), damage(spellDamage) {
}

void DirectDamageSpell::castAsProjectile(GameState* gameState, int dirX, int dirY) const {
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
    
    int currentX = startX;
    int currentY = startY;
    bool targetHit = false;
    int distance = 0;
    
    for (distance = 0; distance < range; distance++) {
        currentX += dirX;
        currentY += dirY;
        
        std::cout << "  Проверяем клетку (" << currentX << ", " << currentY << ")\n";
        
        if (!map->isValidPosition(currentX, currentY)) {
            std::cout << name << " достиг края карты.\n";
            break;
        }
        
        const Cell& cell = map->getCell(currentX, currentY);
        
        for (const auto& enemy : gameState->getEnemies()) {
            if (enemy && enemy->isAlive() && 
                enemy->getPositionX() == currentX && 
                enemy->getPositionY() == currentY) {
                std::cout << name << " попал во врага в (" << currentX << ", " << currentY 
                          << ") на " << damage << " урона!\n";
                enemy->takeDamage(damage);
                targetHit = true;
                break;
            }
        }
        
        if (!targetHit) {
            for (const auto& building : gameState->getBuildings()) {
                if (building && building->isAlive() && 
                    building->getPositionX() == currentX && 
                    building->getPositionY() == currentY) {
                    std::cout << name << " попал в здание в (" << currentX << ", " << currentY 
                              << ") на " << damage << " урона!\n";
                    building->takeDamage(damage);
                    targetHit = true;
                    break;
                }
            }
        }
        
        if (targetHit) {
            break;
        }
        
        if (!cell.isPassable() && cell.getCellType() != Cell::Type::SLOWING) {
            std::cout << name << " попал в препятствие в (" << currentX << ", " << currentY << ")\n";
            break;
        }
    }
    
    if (!targetHit) {
        std::cout << name << " пролетел " << distance << " клеток и не попал ни в кого.\n";
    }
}

std::unique_ptr<SpellCard> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}