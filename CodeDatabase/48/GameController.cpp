#include "lib/GameController.h"
#include "lib/DirectDamageSpell.h"
#include "lib/AreaDamageSpell.h"
#include "lib/TrapSpell.h"
#include "lib/SaveSystem.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

// Вспомогательная функция для ввода
extern int getInputInt(const std::string& prompt, int min, int max);

GameController::GameController(GameField* field) 
    : gameField(field), player(field, "Hero", 0, 0), currentTurn(1), gameRunning(true) {
}

void GameController::initializeGame() {
    createEnemies();
    createTowers();
    
    std::cout << "Game initialized!\n";
    std::cout << "Enemies count: " << enemies.size() << "\n";
    std::cout << "Towers count: " << towers.size() << "\n";
}

void GameController::createEnemies() {
    int width = gameField->getWidth();
    int height = gameField->getHeight();
    
    // Create enemies in corners
    if (gameField->isCellPassable(width-1, 0)) {
        enemies.push_back(Enemy(gameField, width-1, 0, 30, 3));
    }
    
    if (gameField->isCellPassable(0, height-1)) {
        enemies.push_back(Enemy(gameField, 0, height-1, 40, 4));
    }
    
    if (gameField->isCellPassable(width-1, height-1)) {
        enemies.push_back(Enemy(gameField, width-1, height-1, 50, 5));
    }
}

void GameController::createTowers() {
    int width = gameField->getWidth();
    int height = gameField->getHeight();
    
    if (width >= 3 && height >= 3) {
        if (gameField->isCellPassable(width/2, 0)) {
            towers.push_back(EnemyTower(gameField, width/2, 0, 80, 6, 4));
        }
        if (gameField->isCellPassable(width/2, height-1)) {
            towers.push_back(EnemyTower(gameField, width/2, height-1, 80, 6, 4));
        }
        if (width >= 5 && gameField->isCellPassable(width/4, height/2)) {
            towers.push_back(EnemyTower(gameField, width/4, height/2, 100, 8, 5));
        }
    }
}

void GameController::startGameLoop() {
    while (gameRunning && currentTurn <= 100 && player.isPlayerAlive()) {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "TURN " << currentTurn << "\n";
        std::cout << std::string(50, '=') << "\n";
        
        displayGameState();
        
        std::cout << "YOUR TURN\n";
        std::cout << "Enter command (WASD to move, H to heal, C to cast spell, K to save, L to load, T to test spells, X to exit): ";
        
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        processPlayerTurn(input);
        
        if (gameRunning && player.isPlayerAlive()) {
            processEnemyTurn();
            processTowerTurn();
            checkWinCondition();
        }
        
        currentTurn++;
    }
    
    if (!player.isPlayerAlive()) {
        std::cout << "\nGAME OVER! Player was defeated.\n";
    }
    
    displayFinalStatistics();
}

void GameController::processPlayerTurn(char input) {
    // Преобразуем в верхний регистр для удобства
    char upperInput = std::toupper(input);
    
    switch (upperInput) {
        case 'W': 
            player.move(0, -1);
            handleAutoAttack();
            break;
        case 'S': 
            player.move(0, 1);
            handleAutoAttack();
            break;
        case 'A': 
            player.move(-1, 0);
            handleAutoAttack();
            break;
        case 'D':
            player.move(1, 0);
            handleAutoAttack();
            break;
        case 'H':
            player.heal(20);
            break;
        case 'C':
            handleSpellCasting();
            break;
        case 'T':
            giveAllSpells();
            break;
        case 'K':  // Quick Save
            saveGame();
            break;
        case 'L':  // Load Game
            showSaveLoadMenu();
            break;
        case 'X':
            gameRunning = false;
            std::cout << "Game exited by player.\n";
            break;
        default:
            std::cout << "Invalid command! Use W, A, S, D to move, H to heal, C to cast spell, K to save, L to load, T to test spells, X to exit.\n";
            break;
    }
}

void GameController::handleSpellCasting() {
    player.getSpellHand().displayHand();
    if (player.getSpellHand().getSpellCount() > 0) {
        int spellIndex = getInputInt("Select spell (1-" + 
                                   std::to_string(player.getSpellHand().getSpellCount()) + "): ", 
                                   1, player.getSpellHand().getSpellCount()) - 1;
        int targetX = getInputInt("Enter target X: ", 0, gameField->getWidth() - 1);
        int targetY = getInputInt("Enter target Y: ", 0, gameField->getHeight() - 1);
        player.castSpell(spellIndex, targetX, targetY);
    } else {
        std::cout << "No spells available!\n";
    }
}

void GameController::handleAutoAttack() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int distX = std::abs(enemy.getX() - player.getX());
            int distY = std::abs(enemy.getY() - player.getY());
            if (distX <= 1 && distY <= 1 && (distX == 1 || distY == 1)) {
                std::cout << "Auto-attacking adjacent enemy!\n";
                player.attackEnemy(enemy);
                break;
            }
        }
    }
}

void GameController::processEnemyTurn() {
    std::cout << "\nENEMIES TURN:\n";
    int enemiesMoved = 0;
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.moveTowardsPlayer(player.getX(), player.getY(), player);
            enemiesMoved++;
        }
    }
    std::cout << enemiesMoved << " enemies acted this turn.\n";
}

void GameController::processTowerTurn() {
    std::cout << "\nENEMY TOWERS:\n";
    int towersActed = 0;
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.update(player);
            if (tower.canAttackThisTurn()) {
                towersActed++;
            }
            tower.resetAttack();
        }
    }
    std::cout << towersActed << " towers acted this turn.\n";
}

void GameController::checkWinCondition() {
    bool allEnemiesDead = true;
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            allEnemiesDead = false;
            break;
        }
    }
    
    bool allTowersDestroyed = true;
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            allTowersDestroyed = false;
            break;
        }
    }
    
    if (allEnemiesDead && allTowersDestroyed) {
        std::cout << "\n🎉 VICTORY! All enemies and towers defeated! 🎉\n";
        player.increaseScore(100);
        gameRunning = false;
    }
}

void GameController::displayGameState() const {
    gameField->display();
    player.displayStatus();
    
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) aliveEnemies++;
    }
    std::cout << "Enemies remaining: " << aliveEnemies << "/" << enemies.size() << "\n";
    
    int aliveTowers = 0;
    for (const auto& tower : towers) {
        if (tower.isAlive()) aliveTowers++;
    }
    std::cout << "Towers remaining: " << aliveTowers << "/" << towers.size() << "\n";
    
    if (player.getHealth() < 30 && player.getHealth() > 0) {
        std::cout << "WARNING: Low health! Use H to heal.\n";
    }
    
    if (player.getMana() < 10) {
        std::cout << "WARNING: Low mana! Move around to restore mana.\n";
    }
}

void GameController::displayFinalStatistics() const {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "FINAL STATISTICS\n";
    std::cout << std::string(50, '=') << "\n";
    player.displayStatus();
    
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) aliveEnemies++;
    }
    std::cout << "Enemies defeated: " << (enemies.size() - aliveEnemies) << "/" << enemies.size() << "\n";
    
    int aliveTowers = 0;
    for (const auto& tower : towers) {
        if (tower.isAlive()) aliveTowers++;
    }
    std::cout << "Towers destroyed: " << (towers.size() - aliveTowers) << "/" << towers.size() << "\n";
    
    std::cout << "Turns survived: " << (currentTurn - 1) << "\n";
    std::cout << "Spells learned: " << player.getSpellHand().getSpellCount() << "\n";
}

void GameController::giveAllSpells() {
    std::cout << "\nTEST MODE: Adding all spell cards to your hand!\n";
    
    DirectDamageSpell fireBolt("Fire Bolt", 25, 12, 4);
    AreaDamageSpell fireball("Fireball", 15, 18, 5, 2);
    TrapSpell bearTrap("Bear Trap", 30, 10, 3, 6);
    
    if (player.getSpellHand().addSpell(&fireBolt)) {
        std::cout << "✓ Added: " << fireBolt.getName() << "\n";
    }
    
    if (player.getSpellHand().addSpell(&fireball)) {
        std::cout << "✓ Added: " << fireball.getName() << "\n";
    }
    
    if (player.getSpellHand().addSpell(&bearTrap)) {
        std::cout << "✓ Added: " << bearTrap.getName() << "\n";
    }
    
    player.restoreMana(1000);
    std::cout << "All spells added! Use 'C' to cast them.\n";
    player.getSpellHand().displayHand();
}

bool GameController::isGameOver() const {
    return !gameRunning || !player.isPlayerAlive() || currentTurn > 100;
}

// ========== СИСТЕМА СОХРАНЕНИЯ/ЗАГРУЗКИ ==========

void GameController::saveGame() {
    GameSaveData saveData;
    
    // Сохраняем данные игрока
    saveData.playerName = player.getName();
    saveData.playerHealth = player.getHealth();
    saveData.playerMaxHealth = player.getMaxHealth();
    saveData.playerDamage = player.getDamage();
    saveData.playerScore = player.getScore();
    saveData.playerLevel = player.getLevel();
    saveData.playerX = player.getX();
    saveData.playerY = player.getY();
    saveData.playerMana = player.getMana();
    saveData.playerMaxMana = player.getMaxMana();
    
    // Сохраняем данные поля
    saveData.fieldWidth = gameField->getWidth();
    saveData.fieldHeight = gameField->getHeight();
    
    saveData.fieldPassable.resize(saveData.fieldHeight);
    saveData.fieldHasEnemy.resize(saveData.fieldHeight);
    saveData.fieldHasTower.resize(saveData.fieldHeight);
    
    for (int y = 0; y < saveData.fieldHeight; ++y) {
        saveData.fieldPassable[y].resize(saveData.fieldWidth);
        saveData.fieldHasEnemy[y].resize(saveData.fieldWidth);
        saveData.fieldHasTower[y].resize(saveData.fieldWidth);
        
        for (int x = 0; x < saveData.fieldWidth; ++x) {
            const auto& cell = gameField->getCell(x, y);
            saveData.fieldPassable[y][x] = cell.getIsPassable();
            saveData.fieldHasEnemy[y][x] = cell.getHasEnemy();
            saveData.fieldHasTower[y][x] = cell.getHasTower();
        }
    }
    
    // Сохраняем врагов
    for (const auto& enemy : enemies) {
        GameSaveData::EnemyData enemyData;
        enemyData.health = enemy.getHealth();
        enemyData.maxHealth = enemy.getMaxHealth();
        enemyData.damage = enemy.getDamage();
        enemyData.x = enemy.getX();
        enemyData.y = enemy.getY();
        enemyData.alive = enemy.isAlive();
        saveData.enemies.push_back(enemyData);
    }
    
    // Сохраняем башни
    for (const auto& tower : towers) {
        GameSaveData::TowerData towerData;
        towerData.health = tower.getHealth();
        towerData.maxHealth = tower.getMaxHealth();
        towerData.x = tower.getX();
        towerData.y = tower.getY();
        towerData.attackRange = tower.getAttackRange();
        towerData.attackDamage = tower.getAttackDamage();
        towerData.alive = tower.isAlive();
        saveData.towers.push_back(towerData);
    }
    
    // Сохраняем состояние игры
    saveData.currentTurn = currentTurn;
    saveData.currentLevel = 1; // Базовый уровень для GameController
    saveData.gameRunning = gameRunning;
    
    std::string slotName = getSaveSlotName();
    if (!slotName.empty()) {
        SaveSystem::saveGame(saveData, slotName);
    }
}

void GameController::loadGame() {
    auto saves = SaveSystem::getAvailableSaves();
    if (saves.empty()) {
        std::cout << "No save files found!\n";
        return;
    }
    
    std::cout << "Available saves:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i + 1 << ". " << saves[i] << "\n";
    }
    
    int choice = getInputInt("Select save to load (1-" + std::to_string(saves.size()) + "): ", 1, saves.size());
    
    GameSaveData saveData;
    if (SaveSystem::loadGame(saveData, saves[choice - 1])) {
        loadFromSave(saveData);
        std::cout << "Game loaded successfully! Continuing from turn " << currentTurn << "\n";
    } else {
        std::cout << "Failed to load game!\n";
    }
}

void GameController::showSaveLoadMenu() {
    std::cout << "\n=== SAVE/LOAD MENU ===\n";
    std::cout << "1. Save Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Delete Save\n";
    std::cout << "4. Back to Game\n";
    
    int choice = getInputInt("Select option (1-4): ", 1, 4);
    
    switch (choice) {
        case 1:
            saveGame();
            break;
        case 2:
            loadGame();
            break;
        case 3: {
            auto saves = SaveSystem::getAvailableSaves();
            if (saves.empty()) {
                std::cout << "No save files found!\n";
            } else {
                std::cout << "Available saves:\n";
                for (size_t i = 0; i < saves.size(); ++i) {
                    std::cout << i + 1 << ". " << saves[i] << "\n";
                }
                int saveChoice = getInputInt("Select save to delete (0 to cancel): ", 0, saves.size());
                if (saveChoice > 0) {
                    if (SaveSystem::deleteSave(saves[saveChoice - 1])) {
                        std::cout << "Save deleted successfully!\n";
                    } else {
                        std::cout << "Error deleting save!\n";
                    }
                }
            }
            break;
        }
        case 4:
            break;
    }
}

std::string GameController::getSaveSlotName() {
    auto saves = SaveSystem::getAvailableSaves();
    
    if (!saves.empty()) {
        std::cout << "Available save slots:\n";
        for (size_t i = 0; i < saves.size(); ++i) {
            std::cout << i + 1 << ". " << saves[i] << "\n";
        }
        std::cout << saves.size() + 1 << ". New slot\n";
        
        int choice = getInputInt("Select slot (1-" + std::to_string(saves.size() + 1) + "): ", 1, saves.size() + 1);
        
        if (choice <= static_cast<int>(saves.size())) {
            return saves[choice - 1];
        }
    }
    
    std::string newName;
    std::cout << "Enter new save slot name: ";
    std::getline(std::cin, newName);
    return newName.empty() ? "quicksave" : newName;
}

void GameController::loadFromSave(const GameSaveData& saveData) {
    // Восстановить игрока
    player = Player(gameField, saveData.playerName, saveData.playerX, saveData.playerY);
    
    // Восстановить параметры игрока
    player.takeDamage(player.getHealth() - saveData.playerHealth); // Корректируем здоровье
    
    // Восстановить врагов
    enemies.clear();
    for (const auto& enemyData : saveData.enemies) {
        Enemy enemy(gameField, enemyData.x, enemyData.y, enemyData.maxHealth, enemyData.damage);
        if (!enemyData.alive) {
            enemy.takeDamage(enemyData.maxHealth); // Убить врага
        } else {
            enemy.takeDamage(enemyData.maxHealth - enemyData.health); // Нанести урон
        }
        enemies.push_back(enemy);
    }
    
    // Восстановить башни
    towers.clear();
    for (const auto& towerData : saveData.towers) {
        EnemyTower tower(gameField, towerData.x, towerData.y, towerData.maxHealth, 
                        towerData.attackDamage, towerData.attackRange);
        if (!towerData.alive) {
            tower.takeDamage(towerData.maxHealth); // Уничтожить башню
        } else {
            tower.takeDamage(towerData.maxHealth - towerData.health); // Нанести урон
        }
        towers.push_back(tower);
    }
    
    // Восстановить состояние игры
    currentTurn = saveData.currentTurn;
    gameRunning = saveData.gameRunning;
    
    // Восстановить состояние поля
    for (int y = 0; y < saveData.fieldHeight; ++y) {
        for (int x = 0; x < saveData.fieldWidth; ++x) {
            auto& cell = gameField->getCell(x, y);
            cell.setIsPassable(saveData.fieldPassable[y][x]);
            cell.setHasEnemy(saveData.fieldHasEnemy[y][x]);
            cell.setHasTower(saveData.fieldHasTower[y][x]);
        }
    }
}