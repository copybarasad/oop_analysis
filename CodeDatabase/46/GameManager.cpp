#include "GameManager.h"
#include <iostream>
#include <cmath>

GameManager::GameManager() 
    : player("Hero", 5, 5), field(15, 15), gameRunning(false) {
    field.generateImpassableCells(15);
}

void GameManager::startGame() {
    initializeGame();
    gameRunning = true;
    std::cout << "Game started! Use WASD to move, 'c' to switch combat mode, 'f' to attack, 'q' to quit." << std::endl;
}

void GameManager::initializeGame() {
    field.placePlayer(5, 5);
    
    enemies.emplace_back(3, 3, 30, 5);
    enemies.emplace_back(12, 7, 30, 5);
    enemies.emplace_back(8, 12, 30, 5);
    
    for (const auto& enemy : enemies) {
        field.placeEnemy(enemy.getPositionX(), enemy.getPositionY());
    }
}

void GameManager::displayGameState() const {
    std::cout << "\n=== Game State ===" << std::endl;
    std::cout << "Player: " << player.getName() << std::endl;
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
    std::cout << "Position: (" << player.getPositionX() << ", " << player.getPositionY() << ")" << std::endl;
    std::cout << "Combat Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << std::endl;
    std::cout << "Damage: " << player.getDamage() << std::endl;
    std::cout << "Attack Range: " << player.getAttackRange() << std::endl;
    std::cout << "Score: " << player.getScore() << std::endl;
    
    int aliveCount = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            aliveCount++;
        }
    }
    std::cout << "Enemies alive: " << aliveCount << "/3" << std::endl;
    
    for (int i = 0; i < enemies.size(); ++i) {
        if (enemies[i].isAlive()) {
            std::cout << "Enemy " << (i+1) << " HP: " << enemies[i].getHealth() << "/30 at (" 
                      << enemies[i].getPositionX() << ", " << enemies[i].getPositionY() << ")" << std::endl;
        }
    }
    
    std::cout << "\n=== GAME FIELD ===" << std::endl;
    displayField();
}

void GameManager::displayField() const {
    int width = field.getWidth();
    int height = field.getHeight();
    
    std::cout << "\n=== GAME FIELD ===" << std::endl;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell& cell = field.getCell(x, y);
            
            if (cell.getHasPlayer()) {
                std::cout << "P ";
            }
            else if (cell.getHasEnemy()) {
                bool enemyAliveHere = false;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
                        enemyAliveHere = true;
                        break;
                    }
                }
                if (enemyAliveHere) {
                    std::cout << "E ";
                } else {
                    std::cout << ". ";
                }
            }
            else if (cell.getType() == CellType::IMPASSABLE) {
                std::cout << "# ";
            }
            else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nLEGEND: P=You E=Enemy #=Wall .=Empty" << std::endl;
    std::cout << "Your position: (" << player.getPositionX() << ", " << player.getPositionY() << ")" << std::endl;
    
    bool hasAliveEnemies = false;
    for (int i = 0; i < enemies.size(); ++i) {
        if (enemies[i].isAlive()) {
            if (!hasAliveEnemies) {
                std::cout << "Enemy positions: ";
                hasAliveEnemies = true;
            }
            std::cout << "(" << enemies[i].getPositionX() << "," << enemies[i].getPositionY() << ") ";
        }
    }
    if (hasAliveEnemies) {
        std::cout << std::endl;
    }
}
void GameManager::processTurn() {
    if (!gameRunning) return;
    
    displayGameState();
    processPlayerTurn();
    
    if (gameRunning) {
        processEnemiesTurn();
        checkGameOver();
    }
}

void GameManager::processPlayerTurn() {
    std::cout << "\nYour turn! Enter command (WASD to move, C to switch mode, F to attack, Q to quit): ";
    char input;
    std::cin >> input;
    
    int newX = player.getPositionX();
    int newY = player.getPositionY();
    
    switch (input) {
        case 'w': case 'W': newY--; break;
        case 'a': case 'A': newX--; break;
        case 's': case 'S': newY++; break;
        case 'd': case 'D': newX++; break;
        case 'c': case 'C':
            player.switchCombatMode();
            std::cout << "Switched to " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") 
                      << " combat mode! Damage: " << player.getDamage() 
                      << ", Range: " << player.getAttackRange() << std::endl;
            return;
        case 'f': case 'F':
            performAttack();
            return;
        case 'q': case 'Q':
            gameRunning = false;
            std::cout << "Game ended by player." << std::endl;
            return;
        default:
            std::cout << "Invalid command!" << std::endl;
            return;
    }
    
    int oldX = player.getPositionX();
    int oldY = player.getPositionY();
    
    bool enemyOnTargetCell = false;
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPositionX() == newX && enemy.getPositionY() == newY) {
            enemyOnTargetCell = true;
            
            if (player.getCombatMode() == CombatMode::MELEE) {
                int oldEnemyHealth = enemy.getHealth();
                enemy.takeDamage(player.getDamage());
                std::cout << "You attacked enemy in melee for " << player.getDamage() << " damage!" << std::endl;
                std::cout << "Enemy health: " << oldEnemyHealth << " -> " << enemy.getHealth() << "/30" << std::endl;
                
                if (!enemy.isAlive()) {
                    std::cout << "Enemy defeated! +10 points" << std::endl;
                    player.addScore(10);
                    field.removeEnemy(enemy.getPositionX(), enemy.getPositionY());
                }
            } else {
                std::cout << "Cannot melee attack in ranged mode! Use attack command (F)." << std::endl;
            }
            break;
        }
    }
    
    if (!enemyOnTargetCell && field.isCellPassable(newX, newY)) {
        field.removePlayer(oldX, oldY);
        field.placePlayer(newX, newY);
        player.setPosition(newX, newY);
        std::cout << "Moved to (" << newX << ", " << newY << ")" << std::endl;
    } else if (!enemyOnTargetCell) {
        std::cout << "Cannot move there!" << std::endl;
    }
}

void GameManager::performAttack() {
    if (player.getCombatMode() == CombatMode::MELEE) {
        std::cout << "You are in melee mode. Move into enemies to attack!" << std::endl;
        return;
    }
    
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    int attackRange = player.getAttackRange();
    bool attacked = false;
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int enemyX = enemy.getPositionX();
            int enemyY = enemy.getPositionY();
            
            int distance = std::abs(enemyX - playerX) + std::abs(enemyY - playerY);
            
            if (distance <= attackRange && distance > 0) {
                int oldEnemyHealth = enemy.getHealth();
                enemy.takeDamage(player.getDamage());
                std::cout << "You ranged attacked enemy at (" << enemyX << ", " << enemyY 
                          << ") for " << player.getDamage() << " damage! Distance: " << distance << std::endl;
                std::cout << "Enemy health: " << oldEnemyHealth << " -> " << enemy.getHealth() << "/30" << std::endl;
                attacked = true;
                
                if (!enemy.isAlive()) {
                    std::cout << "Enemy defeated! +10 points" << std::endl;
                    player.addScore(10);
                    field.removeEnemy(enemy.getPositionX(), enemy.getPositionY());
                }
            }
        }
    }
    
    if (!attacked) {
        std::cout << "No enemies in attack range (" << attackRange << " cells)!" << std::endl;
    }
}

void GameManager::processEnemiesTurn() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int oldX = enemy.getPositionX();
            int oldY = enemy.getPositionY();
            
            field.removeEnemy(oldX, oldY);
            
            enemy.moveTowardsPlayer(player.getPositionX(), player.getPositionY());
            int newX = enemy.getPositionX();
            int newY = enemy.getPositionY();
            
            if (newX == player.getPositionX() && newY == player.getPositionY()) {
                int oldPlayerHealth = player.getHealth();
                player.takeDamage(enemy.getDamage());
                std::cout << "Enemy attacked you for " << enemy.getDamage() << " damage!" << std::endl;
                std::cout << "Your health: " << oldPlayerHealth << " -> " << player.getHealth() << "/100" << std::endl;
                
                enemy.setPosition(oldX, oldY);
                field.placeEnemy(oldX, oldY);
            }
            else if (field.isCellPassable(newX, newY)) {
                field.placeEnemy(newX, newY);
                std::cout << "Enemy moved to (" << newX << ", " << newY << ")" << std::endl;
            } else {
                enemy.setPosition(oldX, oldY);
                field.placeEnemy(oldX, oldY);
                std::cout << "Enemy cannot move to (" << newX << ", " << newY << ")" << std::endl;
            }
        }
    }
}

void GameManager::checkGameOver() {
    if (!player.isAlive()) {
        gameRunning = false;
        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Final score: " << player.getScore() << std::endl;
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
        gameRunning = false;
        std::cout << "\n=== VICTORY! ===" << std::endl;
        std::cout << "You defeated all enemies! Final score: " << player.getScore() << std::endl;
    }
}

bool GameManager::isGameRunning() const {
    return gameRunning;
}