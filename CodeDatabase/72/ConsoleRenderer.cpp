#include "ConsoleRenderer.h"

#include <unistd.h>

#define CLEAR_COMMAND "clear"

void ConsoleRenderer::printColored(const std::string &text, const std::string &color) {
    std::cout << color << text << "\033[0m";
}

void ConsoleRenderer::drawField(const Field& field) {
    system("clear");

    int width = field.getWidth();
    int height = field.getHeight();
    auto playerPos = field.getPlayer()->getPosition();

    std::cout << "┌";
    for (int i = 0; i < width * 2; i++) std::cout << "─";
    std::cout << "┐\n";

    for (int y = 0; y < height; y++) {
        std::cout << "│";
        for (int x = 0; x < width; x++) {
            Position pos{x, y};
            bool drawn = false;

            if (pos == playerPos) {
                printColored(PLAYER, "\033[32m");
                drawn = true;
            }

            for (const auto & trap: field.getTraps()) {
                if (pos == trap.getPosition() && pos != playerPos) {
                    printColored(TRAP, "\033[35m");
                    drawn = true;
                    break;
                }
            }

            for (const auto &enemy : field.getEnemies()) {
                if (pos == enemy.getPosition()) {
                    printColored(ENEMY, "\033[31m");
                    drawn = true;
                    break;
                }
            }

            for (const auto &build : field.getEnemiesBuild()) {
                if (pos == build.getPosition()) {
                    printColored(ENEMY_BUILD, "\033[35m");
                    drawn = true;
                    break;
                }
            }

            for (const auto & tower: field.getEnemyTowers()) {
                if (pos == tower.getPosition()) {
                    printColored(TOWER, "\033[35m");
                    drawn = true;
                    break;
                }
            }

            if (!drawn) {
                switch (field.getCell(pos).getState()) {
                    case State::Free:     printColored(EMPTY_CELL, "\033[32m"); break;
                    case State::Obstacle: std::cout << OBSTACLE; break;
                    case State::Slow:     std::cout << SLOW_CELL; break;
                }
            }
        }

        std::cout << "│";

        if (y < 7) {
            switch (y) {
                case 0: std::cout << "  \033[36mPlayer Info\033[0m"; break;
                case 1: std::cout << "  HP: " << field.getPlayer()->getHealth(); break;
                case 2: std::cout << "  Damage: " << field.getPlayer()->getDamage(); break;
                case 3: std::cout << "  Score: " << field.getPlayer()->getScore(); break;
                case 4: std::cout << "  Fight mode: " << ((field.getPlayer()->getFightMode() == FightMode::CloseCombat)?"Close":"Range"); break;
                case 5: std::cout << "  Enemies: " << field.getEnemies().size(); break;
                case 6: std::cout << "  Builds: " << field.getEnemiesBuild().size(); break;
            }
        }

        std::cout << "\n";
    }

    std::cout << "└";
    for (int i = 0; i < width * 2; i++) std::cout << "─";
    std::cout << "┘\n";

    std::cout.flush();
}

void ConsoleRenderer::drawMessage(const std::string &message) {
    std::cout << "\n" << message << "\n";
}

void ConsoleRenderer::drawMainMenu(const std::vector<std::string>& menuItems, int selectedIndex) {
    system(CLEAR_COMMAND);

    std::cout << "\n=== RPG GAME ===\n\n";

    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::cout << i + 1 << ". " << menuItems[i] << "\n";
    }

    std::cout << "\nВыберите номер: ";
}

void ConsoleRenderer::drawLoadMenu(const std::vector<std::string>& saveFiles, int selectedIndex) {
    system(CLEAR_COMMAND);

    std::cout << "\n=== ВЫБЕРИТЕ СОХРАНЕНИЕ ===\n\n";

    for (size_t i = 0; i < saveFiles.size(); ++i) {
        std::cout << i + 1 << ". " << saveFiles[i] << "\n";
    }

    std::cout << "\n0. Назад в меню\n";
    std::cout << "\nВыберите номер: ";
}

void ConsoleRenderer::drawMessage(const std::string& title, const std::string& message) {
    system(CLEAR_COMMAND);

    std::cout << "\n=== " << title << " ===\n\n";
    std::cout << message << "\n\n";
}

void ConsoleRenderer::drawSaveMessage(const std::string& filename) {
    system(CLEAR_COMMAND);

    std::cout << "\n=== ИГРА СОХРАНЕНА ===\n\n";
    std::cout << "Файл: " << filename << "\n\n";
}


void ConsoleRenderer::drawGameOver(bool won) {
    system(CLEAR_COMMAND);

    std::cout << "\n";
    if (won) {
        std::cout << "=== ПОБЕДА! ===\n\n";
    } else {
        std::cout << "=== GAME OVER ===\n\n";
    }
}