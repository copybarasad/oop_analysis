#include "gamemanager.h"
#include "savesystem.h"
#include "gameexceptions.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>  // ADD THIS FOR std::remove_if

// Constructor - Initializes the game manager
GameManager::GameManager() 
    : currentLevelNumber(1), gameRunning(false) 
{
    saveSystem = std::make_unique<SaveSystem>();
    levelSystem = std::make_unique<LevelSystem>();
}

void GameManager::startGame()
{
    std::cout << "=== WELCOME TO THE SPELL BATTLE GAME ===\n";
    std::cout << "Would you like to start a new game or open a loaded game? (enter 'new' or 'load'): ";
    
    std::string choice;
    std::cin >> choice;
    
    if (choice == "load") {
        std::cout << "Enter save filename: ";
        std::string filename;
        std::cin >> filename;
        loadGame(filename);
    } else {
        initializeFirstLevel();
    }
    
    gameRunning = true;
    mainGameLoop();
}

void GameManager::mainGameLoop()
{
    while (gameRunning) {
        displayGameState();
        
        processPlayerTurn();
        if (!gameRunning) break;
        
        processAllyTurns();
        processEnemyTurns();     // MOVING ENEMIES
        processTowerTurns();     // TOWERS
        processEnemyBaseTurns();
        
        if (checkLevelComplete()) {
            advanceToNextLevel();
        } else if (checkGameOver()) {
            handleGameOver();
        }
    }
}

void GameManager::initializeFirstLevel()
{
    currentLevel = std::make_unique<GameField>(10, 10);
    player = std::make_unique<Player>(100, 15, Position(5, 5));
    
    player->addPoints(20);
    
    currentLevel->placePlayer(player.get());
    
    // Add initial towers for level 1
    currentLevel->addTower(std::make_unique<EnemyTower>(Position(2, 2)));
    currentLevel->addTower(std::make_unique<EnemyTower>(Position(7, 7)));
    
    currentLevelNumber = 1;


    currentLevel->addEnemy(MovingEnemy(Position(1, 1), 30, 5));
    currentLevel->addEnemy(MovingEnemy(Position(8, 8), 30, 5));

    movingEnemies.clear();
    
    enemyBases.emplace_back(Position(0, 0), 100);

    std::cout << "Level 1 initialized with 2 enemies and 1 enemy base!\n";
}

void GameManager::cleanupCurrentLevel()
{
    // Clear all enemies for next level
    movingEnemies.clear();
    currentLevel.reset();
    std::cout << "Level cleanup completed\n";
}

void GameManager::processPlayerTurn()
{
    std::cout << "\n--- PLAYER TURN " << currentLevelNumber << " ---\n";
    std::cout << "Health: " << player->getHealth();
    std::cout << " | Mana: " << player->getMana() << "/" << player->getMaxMana();
    std::cout << " | Points: " << player->getPoints() << "\n";
    
    handlePlayerInput();
}

void GameManager::processAllyTurns() {
    std::cout << "--- ALLY PHASE ---\n";
    
    if (allies.empty()) {
        std::cout << "No allies to move.\n";
        return;
    }
    
    // Remove dead allies
    allies.erase(std::remove_if(allies.begin(), allies.end(),
        [](const AllyUnit& ally) { return !ally.isAlive(); }),
        allies.end());
    
    // Process living allies
    for (auto& ally : allies) {
        ally.takeTurn(player.get(), currentLevel.get());
    }
    
    std::cout << allies.size() << " allies moved.\n";
}

void GameManager::processEnemyTurns()
{
    
    std::cout << "--- ENEMY PHASE ---\n";
    auto& enemies = currentLevel->getEnemies();
    if (movingEnemies.empty()) {
        std::cout << "No enemies to move.\n";
        return;
    }
    
    
    // Remove dead enemies
    movingEnemies.erase(
        std::remove_if(movingEnemies.begin(), movingEnemies.end(),
            [](const MovingEnemy& enemy) { 
                return !enemy.isAlive(); 
            }),
        movingEnemies.end()
    );
    
    // Process living enemies
    for (auto& enemy : movingEnemies) {
        enemy.takeTurn(player.get(), currentLevel.get());
    }
}

void GameManager::processEnemyBaseTurns() {
    std::cout << "--- ENEMY BASE PHASE ---\n";
    
    if (enemyBases.empty()) {
        std::cout << "No enemy bases.\n";
        return;
    }
    
    for (auto& base : enemyBases) {
        base.takeTurn(currentLevel.get(), movingEnemies);
    }
}

void GameManager::processTowerTurns()
{
    std::cout << "--- TOWER PHASE ---\n";
    
    auto& towers = currentLevel->getTowers();
    if (towers.empty()) {
        std::cout << "No towers to attack.\n";
        return;
    }
    
    int towersAttacked = 0;
    for (auto& tower : towers) {
        if (tower->isPlayerInRange(player->getPosition())) {
            if (tower->attackPlayer(player.get(), currentLevel.get())) {
                towersAttacked++;
            }
        }
    }
    
    if (towersAttacked == 0) {
        std::cout << "No towers in range to attack.\n";
    }
    
    currentLevel->updateTowerCooldowns();
}

bool GameManager::checkLevelComplete() const
{
    return currentLevel->getTowers().empty() && currentLevel->getAliveEnemyCount() == 0;
}

bool GameManager::checkGameOver() const
{
    return !player->isAlive();
}

void GameManager::handleGameOver()
{
    std::cout << "\n=======================\n";
    std::cout << "      GAME OVER!\n";
    std::cout << "You reached Level " << currentLevelNumber << "\n";
    std::cout << "=======================\n";
    
    std::cout << "Would you like to restart or quit? (enter 'restart' or 'quit'): ";
    
    std::string choice;
    std::cin >> choice;
    
    if (choice == "restart") {
        currentLevelNumber = 1;
        gameRunning = false;
        cleanupCurrentLevel();
        player.reset(new Player(100, 15, Position(5, 5)));  // Reset player
        std::cout << "\nRestarting game...\n";
        startGame();
    } else {
        gameRunning = false;
        std::cout << "Thanks for playing!\n";
    }
}

void GameManager::advanceToNextLevel()
{
    std::cout << "\n=================================\n";
    std::cout << " LEVEL " << currentLevelNumber << " COMPLETED!\n";
    std::cout << "=================================\n";
    
    // 8-POINT REQUIREMENT: Restore health and remove 50% of spells
    player->setHealth(100);
    if (player->getHand()) {
        int spellsToRemove = (player->getHand()->getSpellCount() + 1) / 2;
        spellsToRemove = std::min(spellsToRemove, player->getHand()->getSpellCount() - 1);
        for (int i = 0; i < spellsToRemove; i++) {
            player->getHand()->removeSpell(0);
        }
        std::cout << "Health restored! " << spellsToRemove << " spells removed.\n";
    }
    
    // 10-POINT REQUIREMENT: Player progression system
    std::cout << "\n=== LEVEL UP! Choose upgrade (1-4) ===\n";
    std::cout << "1. +25 Max Health (Current: " << player->getHealth() << ")\n";
    std::cout << "2. +20 Max Mana (Current: " << player->getMaxMana() << ")\n"; 
    std::cout << "3. Improve ALL spell damage by +5\n";
    std::cout << "4. Improve ALL spell range by +1\n";
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch(choice) {
        case 1:
            player->setHealth(player->getHealth() + 25);
            std::cout << "✓ Health increased to " << player->getHealth() << "!\n";
            break;
        case 2:
            player->setMaxMana(player->getMaxMana() + 20);
            player->gainMana(20);
            std::cout << "✓ Mana capacity increased to " << player->getMaxMana() << "!\n";
            break;
        case 3:
            if (player->getHand() && player->getHand()->getSpellCount() > 0) {
                for (auto& spell : player->getHand()->getAllSpells()) {
                    spell->upgradeDamage(5);
                }
                std::cout << "✓ All spells damage increased by 5!\n";
            } else {
                std::cout << "No spells to upgrade!\n";
            }
            break;
        case 4:
            if (player->getHand() && player->getHand()->getSpellCount() > 0) {
                for (auto& spell : player->getHand()->getAllSpells()) {
                    spell->upgradeRange(1);
                }
                std::cout << "✓ All spells range increased by 1!\n";
            } else {
                std::cout << "No spells to upgrade!\n";
            }
            break;
        default:
            std::cout << "Invalid choice! No upgrade applied.\n";
            break;
    }
    
    currentLevelNumber++;
    cleanupCurrentLevel();
    
    // Create larger field with more towers
    int newSize = 10 + currentLevelNumber;
    newSize = (newSize < 10) ? 10 : (newSize > 25) ? 25 : newSize;  // Clamp 10-25
    
    currentLevel = std::make_unique<GameField>(newSize, newSize);
    
    Position center(newSize/2, newSize/2);
    currentLevel->placePlayer(player.get());
    player->setPosition(center);
    
    // Add more towers for increased difficulty
    int towersToAdd = 1 + currentLevelNumber;
    for (int i = 0; i < towersToAdd; i++) {
        int offset = (i + 1) * 2;
        currentLevel->addTower(std::make_unique<EnemyTower>(
            Position(offset, offset)));
        if (i % 2 == 0) {
            currentLevel->addTower(std::make_unique<EnemyTower>(
                Position(newSize - offset, newSize - offset)));
        }
    }
    
    // Add more enemies for next level
    int enemiesToAdd = currentLevelNumber;
    for (int i = 0; i < enemiesToAdd; i++) {
        movingEnemies.emplace_back(
            Position(i * 2 + 1, i * 2 + 1), 
            30 + (currentLevelNumber * 5),  // More health each level
            10 + (currentLevelNumber * 2)   // More damage each level
        );
    }
    
    std::cout << "\n=================================\n";
    std::cout << " Welcome to Level " << currentLevelNumber << "!\n";
    std::cout << " Field size: " << newSize << "x" << newSize << "\n";
    std::cout << " Towers: " << currentLevel->getTowers().size() << "\n";
    std::cout << " Enemies: " << movingEnemies.size() << "\n";
    std::cout << "=================================\n";
}

void GameManager::saveGame(const std::string& filename)
{
    try {
        saveSystem->saveGameState(*this, filename);
        std::cout << "✓ Game saved successfully to '" << filename << "'\n";
    } catch (const SaveGameException& e) {
        std::cout << "✗ Save failed: " << e.what() << "\n";
    }
}

void GameManager::loadGame(const std::string& filename)
{
    try {
        saveSystem->loadGameState(*this, filename);
        std::cout << "✓ Game loaded successfully from '" << filename << "'\n";
        gameRunning = true;
    } catch (const LoadGameException& e) {
        std::cout << "✗ Load failed: " << e.what() << "\n";
        std::cout << "Starting new game instead...\n";
        initializeFirstLevel();
        gameRunning = true;
    }
}

void GameManager::displayGameState() const
{
    std::cout << "\n=== CURRENT GAME STATE ===\n";
    currentLevel->display(movingEnemies);
    
    std::cout << "\nLevel: " << currentLevelNumber << "\n";
    std::cout << "Player - Health: " << player->getHealth();
    std::cout << ", Mana: " << player->getMana() << "/" << player->getMaxMana();
    std::cout << ", Points: " << player->getPoints() << "\n";
    std::cout << "Position: (" << player->getPosition().x 
              << ", " << player->getPosition().y << ")\n";
    std::cout << "Towers remaining: " << currentLevel->getTowers().size() << "\n";
    std::cout << "Enemies remaining: " << currentLevel->getEnemies().size() << "\n";
}

void GameManager::handlePlayerInput()
{
    std::cout << "\nCommands: move, cast, buy, mana, save, load, quit\n";
    std::cout << "Enter command: ";
    
    std::string command;
    std::cin >> command;
    
    if (command == "move") {
        std::cout << "Enter new position (x y): ";
        int x, y;
        if (std::cin >> x >> y) {
            if (x >= 0 && x < currentLevel->getWidth() && 
                y >= 0 && y < currentLevel->getHeight()) {
                currentLevel->movePlayer(Position(x, y));
            } else {
                std::cout << "Invalid coordinates! Field is " 
                          << currentLevel->getWidth() << "x" 
                          << currentLevel->getHeight() << "\n";
            }
        } else {
            std::cout << "Invalid input!\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    } 
    else if (command == "cast") {
        if (player->getHand() && player->getHand()->getSpellCount() > 0) {
            player->getHand()->displayHand();
            std::cout << "Enter spell index: ";
            int spellIndex;
            
            if (std::cin >> spellIndex) {
                std::cout << "Enter target position (x y): ";
                int x, y;
                if (std::cin >> x >> y) {
                    player->castSpell(spellIndex, currentLevel.get(), Position(x, y));
                } else {
                    std::cout << "Invalid coordinates!\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
            } else {
                std::cout << "Invalid spell index!\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        } else {
            std::cout << "No spells available!\n";
        }
    }
    else if (command == "buy") {
        if (player->getHand()) {
            if (player->getHand()->buySpell(player.get())) {
                std::cout << "Spell purchased!\n";
            }
        }
    }
    else if (command == "mana") {
        player->gainMana(20);
        std::cout << "Gained 20 mana!\n";
    }
    else if (command == "save") {
        std::cout << "Enter save filename: ";
        std::string filename;
        std::cin >> filename;
        saveGame(filename);
    }
    else if (command == "load") {
        std::cout << "Enter load filename: ";
        std::string filename;
        std::cin >> filename;
        loadGame(filename);
    }
    else if (command == "quit") {
        std::cout << "Are you sure? (yes/no): ";
        std::string confirm;
        std::cin >> confirm;
        std::cin.ignore(10000, '\n');
        if (confirm == "yes") {
            gameRunning = false;
            std::cout << "Thanks for playing!\n";
            return; //to exit immediatly
        } else {
            std::cout << "Continue playing..\n";
            return;
        }
    }
    else {
        std::cout << "Unknown command! Try: move, cast, buy, mana, save, load, quit\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}