#include "Game4.h"
#include "SaveLoadException.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>

Game4::Game4() {}

void Game4::run() {
    std::cout << "=== Lab3 ===\n";
    std::cout << "Load save at start? Enter filename or empty to start new: ";
    std::string fn;
    std::getline(std::cin, fn);
    if (!fn.empty()) {
        try { cmdLoad(fn); }
        catch (const std::exception &e) {
            std::cout << "Load failed: " << e.what() << "\nStarting new game.\n";
            startNewGame();
        }
    } else {
        startNewGame();
    }
    gameLoop();
}

void Game4::startNewGame() {
    levelNumber_ = 1;
    startLevel();
}


void Game4::startLevel() {
    std::cout << "Starting level " << levelNumber_ << " (field " << baseWidth_ + (levelNumber_-1) << "x" << baseHeight_ + (levelNumber_-1) << ")\n";
    int w = baseWidth_ + (levelNumber_-1);
    int h = baseHeight_ + (levelNumber_-1);
    field_.reset(new GameField(w,h));

    size_t cap = player_ ? player_->hand().capacity() : 10;
    player_.reset(new Player(1, 1, *field_, cap));
    field_->setPlayer(player_.get());

    if (levelNumber_ == 1) {
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();
        player_->hand().addRandomSpell();

    }
    int baseHp = 5 + levelNumber_ * 2;
    field_->addUnit(std::make_unique<Enemy>(w/2, h/2, baseHp, 2 + levelNumber_/1));
    field_->addUnit(std::make_unique<Enemy>(w/2+1, h/2, std::max(1, baseHp-2), 1 + levelNumber_/1));
    field_->addBuilding(std::make_unique<EnemyTower>(w-2, h-2, 20 + levelNumber_*5, 6 + levelNumber_, 3));
}

void Game4::gameLoop() {
    std::cout << "Commands: move X Y | use IDX X Y | save FILE | load FILE | status | next | quit\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "quit" || line == "exit") break;
        try {
            processCommand(line);
        } catch (const SaveLoadException &e) {
            std::cout << "Save/Load error: " << e.what() << "\n";
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }

        if (player_ && player_->hasMoved()) {
            field_->tick();
            player_->nextTurn();
        }

        if (player_ && player_->hp() <= 0) {
            std::cout << "Player has died. Start new game? (y - yes, n - quit): ";
            char c;
            if (!(std::cin >> c)) break;
            std::string rest; std::getline(std::cin, rest);
            if (c == 'y' || c == 'Y') {
                startNewGame();
                continue;
            } else {
                std::cout << "Exiting.\n";
                break;
            }
        }

        if (!anyEnemyLeft()) {
            std::cout << "Level cleared!\n";
            std::cout << "Proceed to next level? (y/n): ";
            char c; std::cin >> c;
            std::string rest; std::getline(std::cin, rest);
            if (c == 'y') {
                nextLevel();
            } else {
                std::cout << "You can inspect status or quit.\n";
            }
        }
    }
}