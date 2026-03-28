#include "ConsoleRenderer.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    renderStats(game);
    renderSpells(game);
    renderField(game);
}

void ConsoleRenderer::printMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleRenderer::renderStats(const Game& game) {
    const Player& player = game.getPlayer();
    
    std::cout << "\n=== TURN " << game.getCurrentTurn() << " ===" << std::endl;
    std::cout << "Player - HP: " << player.getHitPoints() 
              << " | Score: " << player.getTotalScore() 
              << " | Position: (" << player.getCoordX() << ", " << player.getCoordY() << ")"
              << " | Mode: " << (player.getCurrentCombatStyle() == MELEE_STYLE ? "MELEE" : "RANGED")
              << " | Damage: " << player.getAttackDamage() << std::endl;
    
    int activeEnemies = 0;
    for (const auto& enemy : game.getEnemies()) {
        if (enemy.isAlive()) {
            activeEnemies++;
        }
    }
    std::cout << "Enemies alive: " << activeEnemies << std::endl;
}

void ConsoleRenderer::renderSpells(const Game& game) {
    const auto& spells = game.getPlayer().getSpellHand().getSpells();
    
    std::cout << "\n=== SPELLS ===" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() 
                  << " - " << spells[i]->getDescription()
                  << " [Range: " << spells[i]->getRange() << "]" << std::endl;
    }
    if (spells.empty()) {
        std::cout << "No spells available." << std::endl;
    }
}

void ConsoleRenderer::renderField(const Game& game) {
    const GameField& field = game.getField();
    int width = field.getFieldWidth();
    int height = field.getFieldHeight();
    const Player& player = game.getPlayer();
    const auto& enemies = game.getEnemies();

    std::cout << "\nGAME FIELD" << std::endl;
    
    std::cout << "  ";
    for (int col = 0; col < width; ++col) {
        std::cout << " " << (col % 10);
    }
    std::cout << std::endl;
    
    for (int row = 0; row < height; ++row) {
        std::cout << (row < 10 ? " " : "") << row << " ";      
        for (int col = 0; col < width; ++col) {
            std::cout << field.getCellRepresentation(col, row, player, enemies) << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nP=Player, E=Enemy, #=Obstacle, .=Empty" << std::endl;
}