#include "ConsoleRenderer.h"
#include "../Game.h"
#include <iostream>
#include <cstdlib>

namespace Game {

void ConsoleRenderer::render(const Game& game) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    const auto& logs = game.getTurnLogs();
    if (!logs.empty()) {
        std::cout << "--- Turn Events ---" << std::endl;
        for (const auto& log : logs) {
            std::cout << log << std::endl;
        }
        std::cout << "-------------------" << std::endl;
    }
    
    std::cout << "\n--- LEVEL " << game.getLevelNumber() << " --- Turn " << game.getCurrentTurn() << " ---" << std::endl;
    displayGameStatus(game);
    displayMap(game);
}

void ConsoleRenderer::displayGameStatus(const Game& game) const {
    const Player& player = game.getPlayer();
    std::cout << "Player: HP=" << player.getHealth()
              << " | Dmg=" << player.getCurrentDamage()
              << " | Mode=" << (player.getCombatMode() == CombatMode::Melee ? "Melee" : "Ranged")
              << " | Score=" << player.getScore() << "/" << game.getScoreToWin()
              << " | Pos=(" << player.getPosition().row << ", " << player.getPosition().column << ")";
    if (player.isSlowedForNextTurn()) {
        std::cout << " (SLOWED)";
    }
    std::cout << " | Enhance: " << player.getEnhancementStacks() 
              << " | Spells: " << player.getSpellHand().getSpellCount() << "/" << player.getSpellHand().getMaxSize();
    std::cout << std::endl;
    std::cout << "Enemies remaining: " << game.getEnemies().size() << std::endl;
}

void ConsoleRenderer::displayMap(const Game& game) const {
    const Map& map = game.getMap();
    const Player& player = game.getPlayer();

    std::cout << "Map:" << std::endl;
    for (int r = 0; r < map.getHeight(); ++r) {
        for (int c = 0; c < map.getWidth(); ++c) {
            Utils::Point currentCell(r, c);
            if (player.getPosition() == currentCell) { std::cout << "P "; }
            else if (game.getAllyAtPosition(currentCell)) { std::cout << "A "; }
            else if (game.getTrapAtPosition(currentCell)) { std::cout << "T "; }
            else if (game.getEnemyAtPosition(currentCell)) { std::cout << "E "; }
            else if (game.getTowerAtPosition(currentCell)) { std::cout << "W "; }
            else if (game.getBuildingAtPosition(currentCell)) { std::cout << "B "; }
            else {
                switch (map.getCell(currentCell).getType()) {
                    case CellType::Empty: std::cout << ". "; break;
                    case CellType::Impassable: std::cout << "# "; break;
                    case CellType::Slowing: std::cout << "S "; break;
                }
            }
        }
        std::cout << std::endl;
    }
}

}
