#include "ConsoleRenderer.h"
#include "Game.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBarrack.h"
#include "EnemyTower.h"
#include "AllyUnit.h"
#include "GameMenu.h"
#include <iostream>
#include <iomanip>
using namespace std;

void ConsoleRenderer::renderGame(const Game& game) {
    clearScreen();
    
    const auto& field = game.getField();
    const auto& player = game.getPlayer();
    const auto& enemies = game.getEnemies();
    const auto& buildings = game.getBuildings();
    const auto& allies = game.getAllies();
    
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            aliveEnemies++;
        }
    }

    int aliveBuildings = 0;
    for (const auto& building : buildings) {
        if (building && building->isAlive()) {
            aliveBuildings++;
        }
    }

    int aliveAllies = 0;
    for (const auto& ally : allies) {
        if (ally && ally->isAlive()) {
            aliveAllies++;
        }
    }

    cout << "=== BATTLE FIELD ===" << endl;
    cout << "Level: " << game.getCurrentLevel() << endl;
    cout << "Enemies alive: " << aliveEnemies << endl;
    cout << "Enemy buildings: " << aliveBuildings << endl;
    cout << "Allies alive: " << aliveAllies << endl;
    cout << "Player: HP = " << player.getHealthPoints()
         << ", DMG = " << player.getDamage()
         << ", Score = " << player.getScorePoints()
         << ", Style = ";

    if (player.getFightingStyle() == 'm') {
        cout << "Melee";
    } else {
        cout << "Range";
    }

    cout << ", Range = " << player.getAttackRange() << endl << endl;

    if (player.shouldSkipMove()) {
        cout << "SLOWED: You cannot move next turn!" << endl << endl;
    }

    int fieldSize = field.getSize();
    
    cout << "   ";
    for (int x = 0; x < fieldSize; x++) {
        if (x < 10) {
            cout << " " << x << " ";
        } else {
            cout << x << " ";
        }
    }
    cout << endl;

    cout << "  +";
    for (int x = 0; x < fieldSize; x++) {
        cout << "---";
    }
    cout << "-+" << endl;

    for (int y = 0; y < fieldSize; y++) {
        if (y < 10) {
            cout << " " << y << "|";
        } else {
            cout << y << "|";
        }

        for (int x = 0; x < fieldSize; x++) {
            CellType cellType = field.getCellType(x, y);

            switch (cellType) {
                case CellType::PLAYER:
                    cout << " P ";
                    break;
                case CellType::ENEMY:
                    cout << " E ";
                    break;
                case CellType::ALLY:
                    cout << " A ";
                    break;
                case CellType::ENEMY_BARRACK:
                    cout << " B ";
                    break;
                case CellType::ENEMY_TOWER:
                    cout << " T ";
                    break;
                case CellType::OBSTACLE:
                    cout << " # ";
                    break;
                case CellType::SLOW:
                    cout << " ~ ";
                    break;
                case CellType::EMPTY:
                    cout << " . ";
                    break;
                case CellType::TRAP:
                    cout << " $ ";
                    break;
                default:
                    cout << " ? ";
                    break;
            }
        }
        cout << "|" << endl; 
    }

    cout << "  +";
    for (int x = 0; x < fieldSize; x++) {
        cout << "---";
    }
    cout << "-+" << endl;

    cout << endl << "Legend: P-Player, E-Enemy, A-Ally" << endl;
    cout << "T-Tower,  B-Barrack, $-Trap, #-Obstacle, ~-Slow zone" << endl;
    cout << "Controls: WASD-move, F-attack, M-use spell, C-change style," << endl;
    cout << "O-save game, L-load game, Q-quit" << endl;
    cout << "Coordinates: columns (X-coord, top), rows (Y-coord, left)" << endl;
}

void ConsoleRenderer::renderGameEnd(const Game::GameEndData& gameEndData) {
    std::cout << "\n====== GAME OVER ======\n\n";
    
    if (gameEndData.playerWon) {
        std::cout << "VICTORY! Levels completed: " << gameEndData.totalLevels << "\n";
        std::cout << "Final score: " << gameEndData.score << "\n";
        std::cout << "Total kills: " << gameEndData.kills << "\n";
    } else if (gameEndData.playerAlive) {
        std::cout << "Game quit.\n";
        std::cout << "Score: " << gameEndData.score << "\n";
        std::cout << "Kills: " << gameEndData.kills << "\n";
        std::cout << "Level reached: " << gameEndData.level << "\n";
    } else {
        std::cout << "DEFEAT!\n";
        std::cout << "Score: " << gameEndData.score << "\n";
        std::cout << "Kills: " << gameEndData.kills << "\n";
        std::cout << "Level reached: " << gameEndData.level << "\n";
    }
    
    std::cout << "\nPress Enter to return to main menu...";
}

void ConsoleRenderer::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}