//
// Created by bob on 10/14/25.
//

#include "Renderer.h"

#include <iostream>

#define CLEAR_COMMAND "clear"
#define PLAYER "P "
#define ENEMY "E "
#define OBSTACLE "O "
#define ENEMY_BUILD "B "
#define SLOW_CELL "~ "
#define EMPTY_CELL ". "
#define HP_TEXT "  HP: "
#define DAMAGE_TEXT "  Damage: "
#define SCORE_TEXT "  Score: "
#define ENEMIES_TEXT "  Enemies: "
#define ENEMIES_BUILD_TEXT "  Builds: "
#define TRAP "T "
#define TOWER "I "

void Renderer::printColored(const std::string &text, const std::string &color) {
    std::cout << color << text << "\033[0m";
}

void Renderer::drawField(const Field &field) {

    system(CLEAR_COMMAND);


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
                case 1: std::cout << HP_TEXT << field.getPlayer()->getHealth(); break;
                case 2: std::cout << DAMAGE_TEXT << field.getPlayer()->getDamage(); break;
                case 3: std::cout << SCORE_TEXT << field.getPlayer()->getScore(); break;
                case 4: std::cout << "  Fight mode: " << ( (field.getPlayer()->getFightMode() == FightMode::CloseCombat)?"Close":"Range"); break;
                case 5: std::cout << ENEMIES_TEXT << field.getEnemies().size(); break;
                case 6: std::cout << ENEMIES_BUILD_TEXT << field.getEnemiesBuild().size(); break;
            }
        }

        std::cout << "\n";
    }

    std::cout << "└";
    for (int i = 0; i < width * 2; i++) std::cout << "─";
    std::cout << "┘\n";

    std::cout.flush();
}
