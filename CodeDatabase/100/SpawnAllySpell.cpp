#include "SpawnAllySpell.h"
#include "../game_objects/Player.h"
#include "../game_main/GameState.h"
#include "../interface/ConsoleRenderer.h"
#include <sstream>

SpawnAllySpell::SpawnAllySpell(int range, int health, int damage, char sym):
                range(range), allyHealth(health), allyDamage(damage), allySymbol(sym), count(1) { updateStats(); }

// Использоват заклинание
bool SpawnAllySpell::use(Player& player, GameState& state, int targetX, int targetY) {
    std::ostringstream ss;

    int centerX = player.getX();
    int centerY = player.getY();
    
    int spawnedCount = 0;
    
    // Все возможные позиции вокруг игрока
    std::vector<std::pair<int, int>> possiblePositions;
    
    for (int x = centerX - range; x <= centerX + range; x++) {
        for (int y = centerY - range; y <= centerY + range; y++) {
            // Пропускаем позицию игрока
            if (x == centerX && y == centerY) continue;
            
            // Проверяем границы поля
            if (x < 0 || x >= state.getField().getWidth() || 
                y < 0 || y >= state.getField().getHeight()) {
                continue;
            }
            
            const Cell& currentCell = state.getField().getCell(x, y);
            
            // Проверяем доступность клетки
            if (!currentCell.getIsPassable() || currentCell.getIsSlowing() || currentCell.getHasTrap()) {
                continue;
            }
            
            // Проверяем, что клетка не занята (проход по всем сущностям)
            bool cellOccupied = false;
            
            for (const auto& character : state.getAllCharacters()) {
                if (character->getX() == x && character->getY() == y && character->isAlive()) {
                    cellOccupied = true;
                    break;
                }
            }
            
            if (!cellOccupied) {
                for (const auto& building : state.getAllBuildings()) {
                    if (building->getX() == x && building->getY() == y && building->isAlive()) {
                        cellOccupied = true;
                        break;
                    }
                }
            }
            
            if (!cellOccupied) {
                for (const auto& tower : state.getAllTowers()) {
                    if (tower->getX() == x && tower->getY() == y && tower->isAlive()) {
                        cellOccupied = true;
                        break;
                    }
                }
            }
            
            if (!cellOccupied) {
                for (const auto& ally : state.getAllAllies()) {
                    if (ally->getX() == x && ally->getY() == y && ally->isAlive()) {
                        cellOccupied = true;
                        break;
                    }
                }
            }
            
            // Если клетка свободна, добавляем 
            if (!cellOccupied) {
                possiblePositions.push_back({x, y});
            }
        }
    }

    // Берём макисмальное кол-во игроков которое реально можно заспавнить
    int maxToSpawn = (count < possiblePositions.size()) ? count : possiblePositions.size();
    
    // Спавним
    for (int i = 0; i < maxToSpawn; i++) {
        int spawnX = possiblePositions[i].first;
        int spawnY = possiblePositions[i].second;
        
        Ally* newAlly = new Ally(allyHealth, allyDamage, spawnX, spawnY, allySymbol);
        state.addAlly(newAlly);
        spawnedCount++;
    }
    
    // Логируем реузльтат заклинания
    if (spawnedCount > 0) {
        if (spawnedCount == 1) {
            ss << Colors::BRIGHT_GREEN << "Ally was spawned!" << Colors::RESET;
        } else {
            ss << Colors::BRIGHT_GREEN << spawnedCount << " allies were spawned around you!" << Colors::RESET;
        }
        state.addMessage(ss.str());
        return true;
    } else {
        ss << Colors::DARK_RED << "Can't spawn ally: no free cells around player" << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }

}

// Методы для улучшения
void SpawnAllySpell::upgrade() {
    level++;
    updateStats();
}

void SpawnAllySpell::updateStats() {
    if (level > 1) count++; // +1 союзник за уровень
}

// Геттеры
std::string SpawnAllySpell::getName() const { return "Spawn Ally Spell"; }

char SpawnAllySpell::getSymbol() const { return 'a'; }

int SpawnAllySpell::getRange() const { return range; }

int SpawnAllySpell::getLevel() const { return level; }

int SpawnAllySpell::getAllyHealth() const { return allyHealth; }

int SpawnAllySpell::getAllyDamage() const { return allyDamage; }