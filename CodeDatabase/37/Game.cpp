#include "Game.h"
#include <iostream>
#include <vector>

Game::Game() : player(100, 20, 15), field(15, 15), gameRunning(true), combatJustHappened(false) {
    // Create enemies at specific positions
    enemies.emplace_back(50, 10, Position(5, 5));
    enemies.emplace_back(50, 10, Position(10, 10));
    enemies.emplace_back(50, 10, Position(7, 12));
    
    std::cout << "=== LAB WORK 4 - GAME ===\n";
}

void Game::displayGameField() const {
    std::cout << "\n=== GAME FIELD ===\n";
    
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            Position currentPos(x, y);
            char cellChar = '.';
            
            // Check what's at this position
            if (player.getPosition() == currentPos) {
                cellChar = 'P';
            } else {
                bool enemyFound = false;
                for (size_t i = 0; i < enemies.size(); i++) {
                    if (enemies[i].isAlive() && enemies[i].getPosition() == currentPos) {
                        cellChar = 'E';
                        enemyFound = true;
                        break;
                    }
                }
                
                if (!enemyFound) {
                    if (!field.isCellPassable(currentPos)) {
                        cellChar = '#';
                    } else {
                        // Check if this cell is adjacent to combat
                        bool nearCombat = false;
                        Position playerPos = player.getPosition();
                        std::vector<Position> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
                        
                        for (const auto& dir : directions) {
                            Position checkPos = playerPos + dir;
                            if (checkPos == currentPos) {
                                for (const auto& enemy : enemies) {
                                    if (enemy.isAlive() && enemy.getPosition() == currentPos) {
                                        nearCombat = true;
                                        break;
                                    }
                                }
                            }
                        }
                        
                        if (nearCombat && combatJustHappened) {
                            cellChar = '!';
                        } else {
                            cellChar = '.';
                        }
                    }
                }
            }
            
            std::cout << cellChar << " ";
        }
        std::cout << "\n";
    }
    std::cout << "==================\n";
}

void Game::displayStatus() const {
    std::cout << "\n=== PLAYER STATUS ===\n";
    std::cout << "Health: " << player.getHealth() << "/100\n";
    std::cout << "Damage: " << player.getDamage() << " (" 
              << (player.getCombatMode() ? "RANGED" : "MELEE") << ")\n";
    std::cout << "Position: (" << player.getPosition().x << ", " << player.getPosition().y << ")\n";
    
    std::cout << "\n=== ENEMIES ===\n";
    int aliveCount = 0;
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].isAlive()) {
            std::cout << "Enemy " << i << ": HP=" << enemies[i].getHealth() 
                      << " at (" << enemies[i].getPosition().x << ", " 
                      << enemies[i].getPosition().y << ")\n";
            aliveCount++;
        }
    }
    if (aliveCount == 0) {
        std::cout << "All enemies defeated!\n";
    }
    
    if (combatJustHappened) {
        std::cout << "*** COMBAT! You and enemy are fighting! ***\n";
    }
}


// void Game::processInput(char input) {
//     switch (input) {
//         case 'w': case 'W': movePlayer(Direction::UP); break;
//         case 's': case 'S': movePlayer(Direction::DOWN); break;
//         case 'a': case 'A': movePlayer(Direction::LEFT); break;
//         case 'd': case 'D': movePlayer(Direction::RIGHT); break;
//         case 'c': case 'C': switchPlayerMode(); break;
//         case 'q': case 'Q': quitGame(); break;
//         default:
//             std::cout << "Unknown command!\n";
//             return;
//     }
// }

void Game::movePlayer(Direction dir) {
    Position newPos = player.getPosition();
    
    switch (dir) {
        case Direction::UP: newPos.y--; break;
        case Direction::DOWN: newPos.y++; break;
        case Direction::LEFT: newPos.x--; break;
        case Direction::RIGHT: newPos.x++; break;
    }
    
    combatJustHappened = false;
    
    // Check if movement is possible
    if (isValidMove(newPos)) {
        performPlayerMove(newPos);
        std::cout << "Moved to (" << newPos.x << ", " << newPos.y << ")\n";
        
        checkCombat();
        
        if (!combatJustHappened) {
            processEnemyTurns();
            checkCombat();
        }
        
        checkGameState();
    } else {
        std::cout << "Cannot move there! Cell is blocked or occupied.\n";
    }
}

// NEW: Direct mode switching method for commands
void Game::switchPlayerMode() {
    player.switchCombatMode();
    std::cout << "Combat mode switched to " 
              << (player.getCombatMode() ? "RANGED" : "MELEE") << "!\n";
}

// NEW: Direct quit method for commands
void Game::quitGame() {
    gameRunning = false;
    std::cout << "Quitting game...\n";
}

// Helper methods
bool Game::isValidMove(const Position& newPos) const {
    return field.isCellPassable(newPos) && !isEnemyAtPosition(newPos);
}

bool Game::isEnemyAtPosition(const Position& pos) const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == pos) {
            return true;
        }
    }
    return false;
}

void Game::performPlayerMove(const Position& newPos) {
    player.setPosition(newPos);
}

void Game::processEnemyTurns() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            Position oldPos = enemy.getPosition();
            Position playerPos = player.getPosition();
            
            bool isAdjacent = false;
            std::vector<Position> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            
            for (const auto& dir : directions) {
                Position checkPos = playerPos + dir;
                if (checkPos == oldPos) {
                    isAdjacent = true;
                    break;
                }
            }
            
            if (!isAdjacent) {
                enemy.moveTowards(player.getPosition(), field);
                if (oldPos != enemy.getPosition()) {
                    std::cout << "Enemy moved to (" << enemy.getPosition().x 
                              << ", " << enemy.getPosition().y << ")\n";
                }
            }
        }
    }
}

void Game::checkCombat() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            Position enemyPos = enemy.getPosition();
            Position playerPos = player.getPosition();
            
            bool isAdjacent = false;
            std::vector<Position> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            
            for (const auto& dir : directions) {
                Position checkPos = playerPos + dir;
                if (checkPos == enemyPos) {
                    isAdjacent = true;
                    break;
                }
            }
            
            if (isAdjacent) {
                combatJustHappened = true;
                
                int enemyDamage = enemy.getDamage();
                enemy.attack(player);
                std::cout << "Enemy attacks you! You lose " << enemyDamage 
                          << " HP. Remaining: " << player.getHealth() << "\n";
                
                if (player.isAlive()) {
                    int playerDamage = player.getDamage();
                    enemy.takeDamage(playerDamage);
                    std::cout << "You attack enemy! Enemy loses " << playerDamage 
                              << " HP. Enemy HP: " << enemy.getHealth() << "\n";
                    
                    if (!enemy.isAlive()) {
                        std::cout << "You defeated the enemy!\n";
                    }
                }
                
                break;
            }
        }
    }
}

void Game::checkGameState() {
    if (!player.isAlive()) {
        std::cout << "\n*** GAME OVER! You were defeated. ***\n";
        gameRunning = false;
        return;
    }

    bool allEnemiesDefeated = true;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            allEnemiesDefeated = false;
            break;
        }
    }

    if (allEnemiesDefeated) {
        std::cout << "\n*** VICTORY! All enemies defeated! ***\n";
        gameRunning = false;
    }
}

void Game::showControls() const {
    std::cout << "\n=== CONTROLS ===\n";
    std::cout << "W - Move Up    A - Move Left\n";
    std::cout << "S - Move Down  D - Move Right\n";
    std::cout << "C - Switch Combat Mode (Melee/Ranged)\n";
    std::cout << "Q - Quit Game\n";
    std::cout << "LEGEND: P=Player, E=Enemy, !=Combat area, #=Wall, .=Empty\n";
}