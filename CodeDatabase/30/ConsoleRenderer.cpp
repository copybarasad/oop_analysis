#include "include/ConsoleRenderer.h"
#include <iostream>
#include <string>
#include <cstdlib>

#include "include/Manager.h"
#include "../entities/include/Player.h"
#include "../entities/include/Enemy.h"
#include "../entities/include/EnemyBuilding.h"
#include "../entities/include/EnemyTower.h"
#include "../entities/include/Ally.h"
#include "../items/include/Hand.h"
#include "../items/include/Spell.h"

void ConsoleRenderer::render(const GameCycle& game) {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    const auto& manager = game.manager();
    int rows = manager.rows();
    int cols = manager.cols();

    std::cout << "=== LAB GAME (Level " << game.currentLevel() << ") ===\n";

    if (game.playerId() != InvalidEntityId) {
        const auto* player = dynamic_cast<const Player*>(manager.entity(game.playerId()));
        if (player) {
            std::cout << "Player HP: " << player->hp() 
                        << " | Dmg: " << player->damage()
                        << " | Mode: " << (player->attackMode() == AttackMode::Melee ? "Melee" : "Ranged") 
                        << "\n";
            
            std::cout << "Hand: ";
            const auto& hand = player->hand();
            for (size_t i = 0; i < hand.slots().size(); ++i) {
                std::string typeName = "Unknown";
                if (hand.slots()[i]) {
                    switch(hand.slots()[i]->type()) {
                        case SpellType::DirectDamage: typeName = "DirectDmg"; break;
                        case SpellType::AreaDamage: typeName = "AreaDmg"; break;
                        case SpellType::Trap: typeName = "Trap"; break;
                        case SpellType::Summon: typeName = "Summon"; break;
                        case SpellType::Buff: typeName = "Buff"; break;
                    }
                } else {
                    typeName = "Empty";
                }
                std::cout << "[" << i << ":" << typeName << "] ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "    ";
    for (int c = 0; c < cols; ++c) {
        if (c < 10) std::cout << " " << c << " ";
        else std::cout << " " << c;
    }
    std::cout << "\n   +";
    for (int c = 0; c < cols; ++c) std::cout << "---";
    std::cout << "+\n";

    for (int r = 0; r < rows; ++r) {
        if (r < 10) std::cout << " " << r << " |";
        else std::cout << r << " |";

        for (int c = 0; c < cols; ++c) {
            Position pos{r, c};
            auto occupant = manager.occupantAt(pos);
            char symbol = '.';
            
            if (occupant) {
                const auto* ent = manager.entity(*occupant);
                if (ent) {
                    switch (ent->type()) {
                        case EntityType::Player: symbol = 'P'; break;
                        case EntityType::Enemy: symbol = 'E'; break;
                        case EntityType::EnemyBuilding: symbol = 'B'; break;
                        case EntityType::EnemyTower: symbol = 'T'; break;
                        case EntityType::Ally: symbol = 'A'; break;
                    }
                }
            } else if (!manager.field().cellAt(pos).isPassable()) {
                symbol = '#';
            } else if (manager.field().cellAt(pos).hasTrap()) {
                symbol = '^';
            } else if (manager.field().cellAt(pos).isSlowing()) {
                symbol = '~';
            }
            std::cout << " " << symbol << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "   +";
    for (int c = 0; c < cols; ++c) std::cout << "---";
    std::cout << "+\n";
}

void ConsoleRenderer::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int ConsoleRenderer::showMainMenu() {
    clearScreen();
    std::cout << "=== LAB GAME ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "> ";
    
    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice! Please enter 1, 2 or 3.\n> ";
    }
    return choice;
}

void ConsoleRenderer::getFieldSize(int& rows, int& cols) {
    clearScreen();
    std::cout << "=== NEW GAME ===\n";
    std::cout << "Enter field size (rows cols): ";
    while (!(std::cin >> rows >> cols) || rows < 10 || cols < 10 || rows > 25 || cols > 25) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid size! Please enter values between 10 and 25.\n";
        std::cout << "Enter field size (rows cols): ";
    }
}

std::string ConsoleRenderer::getLoadFilename() {
    std::cout << "Enter filename to load: ";
    std::string filename;
    std::cin >> filename;
    return filename;
}
