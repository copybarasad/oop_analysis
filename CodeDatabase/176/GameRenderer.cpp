#include "GameRenderer.h"
#include <iostream>
#include <sstream>

void GameRenderer::render(const GameManager& gameManager, int currentLevel) const {
    const auto& player = gameManager.getPlayer();
    std::cout << renderPlayerInfo(player, currentLevel);
    std::cout << renderSpellInfo(player);
    std::cout << renderGameField(gameManager.getField(), 
                                gameManager.getPlayerPosition(),
                                gameManager);
    std::cout << renderLegend();
}

std::string GameRenderer::renderPlayerInfo(const Player& player, int currentLevel) const {
    std::stringstream ss;
    
    ss << "=== LEVEL " << currentLevel << " ===\n";
    ss << "Player HP: " << player.getHealth()
       << " | Mana: " << player.getMana() << "/" << player.getMaxMana()
       << " | Score: " << player.getScore()
       << " | Enemies Killed: " << player.getEnemiesKilled() << "/" << (currentLevel * 3)
       << " | Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED")
       << " | Damage: " << player.getDamage()
       << " | Slowed: " << (player.getIsSlowed() ? "YES" : "NO") << "\n\n";
    
    return ss.str();
}

std::string GameRenderer::renderSpellInfo(const Player& player) const {
    std::stringstream ss;
    ss << "SPELLS: ";
    
    auto* spellHand = player.getSpellHand();
    if (spellHand) {
        for (size_t i = 0; i < spellHand->getMaxSize(); ++i) {
            auto* spell = spellHand->getSpell(i);
            if (spell) {
                ss << (i + 1) << "=" << spell->getName() 
                   << "(" << spell->getManaCost() << " mana) ";
            } else {
                ss << (i + 1) << "=[EMPTY] ";
            }
        }
    }
    ss << "\n\n";
    
    return ss.str();
}

char GameRenderer::getCellSymbol(const GameCell& cell, 
                                int x, int y,
                                const std::pair<int, int>& playerPos,
                                const GameManager& gameManager) const {
    if (x == playerPos.first && y == playerPos.second) {
        return 'P';
    }
    
    if (gameManager.hasBuildingAt(x, y)) {
        return 'B';
    }
    
    if (gameManager.hasTowerAt(x, y)) {
        return 'T';
    }
    
    const auto& field = gameManager.getField();
    if (field.hasAllyAt(x, y)) {
        return 'A';
    }
    
    if (field.hasTrapAt(x, y)) {
        return 'X';
    }
    
    if (cell.hasEnemy()) {
        return 'E';
    }
    
    switch (cell.getType()) {
        case CellType::EMPTY: return '.';
        case CellType::OBSTACLE: return '#';
        case CellType::SLOW: return '~';
    }
    
    return '?';
}

std::string GameRenderer::renderGameField(const GameField& field, 
                                         const std::pair<int, int>& playerPos,
                                         const GameManager& gameManager) const {
    std::stringstream ss;
    
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            const auto& cell = field.getCell(x, y);
            ss << getCellSymbol(cell, x, y, playerPos, gameManager) << " ";
        }
        ss << "\n";
    }
    
    return ss.str();
}

std::string GameRenderer::renderLegend() const {
    return "\nLEGEND: P=You, E=Enemy, A=Ally, X=Trap, B=Building, T=Tower, #=Wall, ~=Slow trap\n"
           "CONTROLS: WASD=move, M=mode, Q=quit, IJKL=attack, 1-6=spells, P=purchase spell\n\n";
}
