#include "ConsoleRenderer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void ConsoleRenderer::render(const GameEngine& game, int currentLevel) const {    
    std::cout << "=== LEVEL " << currentLevel << " ===\n\n";
    renderGameField(game);
    std::cout << "\n";
    renderGameInfo(game, currentLevel);
    renderEnemiesInfo(game);
    std::cout << "\n";
    renderLegend();
    std::cout << "\n";
    renderControls();
    std::cout << "\n";
}

void ConsoleRenderer::renderGameField(const GameEngine& game) const {
    Position playerPos = game.getPlayerPosition();
     int width = game.getWidth();
    int height = game.getHeight();
    
    std::cout << "    ";
    for (int x = 0; x < width; ++x) {
        std::cout << std::setw(2) << x << " ";
    }
    std::cout << "\n";
    
    std::cout << "   #";
    for (int x = 0; x < width * 3 - 1; ++x) {
        std::cout << "#";
    }
    std::cout << "#\n";
    
    for (int y = 0; y < height; ++y) {
        std::cout << std::setw(2) << y << " #";
        
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            std::cout << " " << getCellSymbol(game, pos) << " ";
        }
        std::cout << "#\n";
    }
    
    std::cout << "   #";
    for (int x = 0; x < width * 3 - 1; ++x) {
        std::cout << "#";
    }
    std::cout << "#\n";
}

std::string ConsoleRenderer::getCellSymbol(const GameEngine& game, const Position& pos) const {
    if (pos == game.getPlayerPosition()) {
        return "@";
    }
    
    auto trapPositions = game.getTrapPositions();
    for (size_t i = 0; i < trapPositions.size(); ++i) {
        if (trapPositions[i] == pos) {
            return "*"; 
        }
    }
    
    auto enemyPositions = game.getEnemyPositions();
    auto enemyHealths = game.getEnemyHealths();
    
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyPositions[i] == pos && enemyHealths[i] > 0) {
            if (game.isEnemyTower(i)) {
                return "T";
            } else {
                return "E";
            }
        }
    }
    
    switch (game.getCellType(pos)) {
        case WALL: return "#";
        case TOWER_BASE: return "B";
        default: return ".";
    }
}

void ConsoleRenderer::renderGameInfo(const GameEngine& game, int currentLevel) const {
    std::cout << "PLAYER:\n";
    std::cout << "  HP: " << game.getPlayerHealth() 
              << " | Mana: " << game.getPlayerMana()
              << " | Mode: " << (game.getPlayerCombatMode() == MELEE ? "MELEE" : "RANGED")
              << " | Score: " << game.getPlayerScore() << "\n";
    
    std::cout << "  Position: (" << game.getPlayerPosition().getX() << ", " << game.getPlayerPosition().getY() << ")\n";
    std::cout << "  Damage: " << game.getPlayerDamage() << "\n";
    
    const auto& spellHand = game.getPlayerSpellHand();
    if (spellHand.getSpellCount() > 0) {
        std::cout << "  Spells: ";
        for (int i = 0; i < spellHand.getSpellCount(); ++i) {
            std::cout << i << ": " << spellHand.getSpellName(i) << " (Cost: " << spellHand.getSpellManaCost(i) << ") | ";
        }
        std::cout << "\n";
    }
    
    std::cout << "--------------------------------------------------\n";
}

void ConsoleRenderer::renderEnemiesInfo(const GameEngine& game) const {
    auto enemyPositions = game.getEnemyPositions();
    auto enemyHealths = game.getEnemyHealths();
    auto enemyDamages = game.getEnemyDamages();
    
    int aliveCount = game.getAliveEnemyCount();
    
    std::cout << "ENEMIES (Alive: " << aliveCount << "):\n";
    
    if (enemyPositions.empty()) {
        std::cout << "  No enemies\n";
        return;
    }
    
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyHealths[i] <= 0) {
            continue; 
        }
        
        std::cout << "  Enemy #" << i << ":\n";
        std::cout << "    Type: " << (game.isEnemyTower(i) ? "TOWER" : "NORMAL") << "\n";
        std::cout << "    HP: " << enemyHealths[i] << "\n";
        std::cout << "    Damage: " << enemyDamages[i] << "\n";
        std::cout << "    Position: (" << enemyPositions[i].getX() 
                  << ", " << enemyPositions[i].getY() << ")\n";
        
        if (game.isEnemyTower(i)) {
            std::cout << "    Range: " << game.getEnemyAttackRange(i) << "\n";
        }
        
        Position playerPos = game.getPlayerPosition();
        int distance = std::abs(enemyPositions[i].getX() - playerPos.getX()) + 
                       std::abs(enemyPositions[i].getY() - playerPos.getY());
        std::cout << "    Distance to player: " << distance << "\n";
        
        if (game.isEnemyTower(i)) {
            if (distance <= game.getEnemyAttackRange(i)) {
                std::cout << "    [CAN ATTACK PLAYER!]\n";
            } else {
                std::cout << "    [Out of range]\n";
            }
        } else if (distance <= 1) {
            std::cout << "    [CAN ATTACK PLAYER!]\n";
        }
        
        std::cout << "\n";
    }
    
    std::cout << "--------------------------------------------------\n";
}



void ConsoleRenderer::renderLegend() const {
    std::cout << "LEGEND:\n";
    std::cout << "  @ - Player\n";
    std::cout << "  E - Normal Enemy\n";
    std::cout << "  T - Tower\n";
    std::cout << "  # - Wall\n";
    std::cout << "  * - Trap\n";
    std::cout << "  B - Tower Base\n";
    std::cout << "  . - Empty cell\n";
}

void ConsoleRenderer::renderControls() const {
    std::cout << "CONTROLS:\n";
    std::cout << "  Movement: w/up, s/down, a/left, d/right\n";
    std::cout << "  Combat: m - switch mode, b - buy spell (50 score)\n";
    std::cout << "  Spells: cast - cast spell (enter index, then coordinates)\n";
    std::cout << "  Game: save, load, quit/exit\n";
}