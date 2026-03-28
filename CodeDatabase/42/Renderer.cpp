#include "Renderer.h"
#include "GameConstants.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdlib>
#endif

void Renderer::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Renderer::render(const GameField& field, const Player& player, int turnCount) {
    clearScreen();
    
    std::cout << "LEGEND OF SEGFAULT" << std::endl;
    std::cout << "Turn: " << turnCount << " | Enemies: " << field.getEnemyCount() 
              << " | Towers: " << field.getTowerCount() << std::endl;
    
    std::cout << "Player - HP: " << player.getHealth() << "/" << player.getMaxHealth();
    std::cout << " | Attack: " << (player.getAttackType() == AttackType::MELEE ? "Melee" : "Ranged");
    std::cout << " | Damage: " << player.getDamage();
    std::cout << " | Spells: " << player.getHand().getHandSize() << "/" << player.getHand().getMaxSize() << std::endl;
    
    std::cout << "------------------------" << std::endl;
    
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            Cell cell = field.getCell(Position(x, y));
            std::cout << getCellSymbol(cell) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    renderCombatLog(field.getPreviousTurnLog());
    
    std::cout << "Legend: P - Player, E - Enemy, T - Tower, S - Trap, # - Obstacle, . - Empty" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W/A/S/D - Move UP/LEFT/DOWN/RIGHT" << std::endl;
    std::cout << "  X - Stay in place (skip move)" << std::endl;
    std::cout << "  T - Switch attack type (Melee <-> Ranged)" << std::endl;
    std::cout << "  F - Use ranged attack in direction" << std::endl;
    std::cout << "  R - Open spell menu" << std::endl;
    std::cout << "  K - Save game" << std::endl;
    std::cout << "  L - Load game" << std::endl;
    std::cout << "  Q - Quit game" << std::endl;
}

char Renderer::getCellSymbol(const Cell& cell) {
    switch (cell.getType()) {
        case CellType::EMPTY:    return Constants::EMPTY_CELL;
        case CellType::PLAYER:   return Constants::PLAYER_CELL;
        case CellType::ENEMY:    return Constants::ENEMY_CELL;
        case CellType::OBSTACLE: return Constants::OBSTACLE_CELL;
        case CellType::TOWER:    return Constants::TOWER_CELL;
        case CellType::TRAP:     return Constants::TRAP_CELL;
        default:                 return '?';
    }
}

void Renderer::renderCombatLog(const std::vector<std::string>& log) {
    if (!log.empty()) {
        std::cout << "Combat Log:" << std::endl;
        for (const auto& message : log) {
            std::cout << "> " << message << std::endl;
        }
        std::cout << std::endl;
    }
}

void Renderer::renderGameOver(bool playerWon, int turnCount) {
    clearScreen();
    std::cout << std::endl << "GAME OVER" << std::endl;
    std::cout << "Turns: " << turnCount << std::endl;
    
    if (playerWon) {
        std::cout << "You defeated all enemies!" << std::endl;
    } else {
        std::cout << "You were defeated." << std::endl;
    }
    
    std::cout << std::endl << "Press Enter to exit...";
    std::cin.get();
}

