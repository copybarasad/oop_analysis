#include "RenderSystem.h"
#include <iostream>
#include <algorithm>

RenderSystem::RenderSystem(const GameField& field, const Player& player,
                         const std::vector<Enemy>& enemies, const std::vector<EnemyBuilding>& buildings,
                         const std::vector<Trap>& traps, const std::vector<Ally>& allies,
                         const std::vector<EnemyTower>& towers)
    : field(field), player(player), enemies(enemies), buildings(buildings),
      traps(traps), allies(allies), towers(towers) {}

void RenderSystem::renderGame() const {
    std::cout << "\n   Grid (0.." << field.getWidth()-1 << ") across X, 0.." << field.getHeight()-1 << " across Y\n";
    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); ++x) std::cout << std::setw(2) << x << ' ';
    std::cout << '\n';

    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << std::setw(2) << y << ' ';
        for (int x = 0; x < field.getWidth(); ++x) {
            Position currentPos(x,y);
            char cellChar = '.';
            
            if (currentPos == player.getPosition()) {
                cellChar = 'P';
            } else {
                bool enemyFound = false;
                for (const auto &enemy: enemies) {
                    if (enemy.isAlive() && enemy.getPosition() == currentPos) { 
                        cellChar = 'E'; 
                        enemyFound = true; 
                        break; 
                    }
                }
                
                if (!enemyFound) {
                    for (const auto &b: buildings) {
                        if (b.getPosition() == currentPos) { 
                            cellChar = 'B'; 
                            break; 
                        }
                    }
                    
                    for (const auto &t: towers) {
                        if (t.getPosition() == currentPos && t.isAlive()) { 
                            cellChar = 'T'; 
                            break; 
                        }
                    }
                    
                    if (cellChar == '.') {
                        const GameCell &cell = field.getCell(currentPos);
                        switch (cell.getType()) {
                            case CellType::OBSTACLE: cellChar = '#'; break;
                            case CellType::SLOWING: cellChar = '~'; break;
                            default: cellChar = '.'; break;
                        }
                    }
                }
            }
            
            for (const auto &tr: traps) if (tr.getPosition() == currentPos) cellChar = '^';
            for (const auto &al: allies) if (al.getPosition() == currentPos) cellChar = 'A';
            
            std::cout << std::setw(2) << cellChar << ' ';
        }
        std::cout << '\n';
    }
    
    std::cout << "\nPlayer HP: " << player.getHealth();
    if (!towers.empty()) {
        const EnemyTower &tower = towers.front();
        std::cout << " | Tower HP: " << (tower.isAlive() ? tower.getHealth() : 0);
    }

    std::cout << " | Score: " << player.getScore()
              << " | Coins: " << player.getCoins()
              << " | Combat: " << (player.getCombatType() == CombatType::MELEE ? "Melee" : "Ranged")
              << " | Damage: " << player.getDamage()
              << " | Range: " << player.getAttackRange();
              
    if (!player.getCanMove()) std::cout << " | SLOWED";
    
    std::cout << "\nEnemies alive: " << std::count_if(enemies.begin(), enemies.end(), 
                  [](const Enemy &e){ return e.isAlive(); })
              << " | Allies: " << allies.size() << " | Towers: " << towers.size() << "\n";
              
    std::cout << "Commands: WASD-move, T+dir - attack, X-area attack, C-switch combat, H-help, Q-quit\n";
    std::cout << "Spell commands: L - list hand, U - use spell (index then target x y), B - buy random spell (cost 30), G - upgrade spell (cost 25)\n";
}