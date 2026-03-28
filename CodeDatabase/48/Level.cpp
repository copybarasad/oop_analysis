#include "lib/Level.h"
#include "lib/SaveSystem.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

// Объявление внешней функции
extern int getInputInt(const std::string& prompt, int min, int max);

Level::Level(int number, GameField* field, Player* player) 
    : levelNumber(number), gameField(field), player(player), levelCompleted(false), levelFailed(false) {
    std::srand(std::time(0));
}

void Level::initialize() {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "          LEVEL " << levelNumber << "\n";
    std::cout << std::string(50, '=') << "\n";
    
    createEnemies();
    createTowers();
    displayLevelInfo();
}

void Level::start() {
    initialize();
    
    int turn = 1;
    while (isRunning() && turn <= 100) {
        std::cout << "\n" << std::string(40, '-') << "\n";
        std::cout << "TURN " << turn << " - LEVEL " << levelNumber << "\n";
        std::cout << std::string(40, '-') << "\n";
        
        gameField->display();
        player->displayStatus();
        displayLevelInfo();
        
        std::cout << "\n--- PLAYER TURN ---\n";
        std::cout << "Enter command (WASD to move, H to heal, C to cast spell, T to test spells, K - Quick Save, L - Load Game, X to exit): ";
        
        char input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        processPlayerTurn(input);
        
        if (!isRunning()) break;
        
        std::cout << "\n--- ENEMIES TURN ---\n";
        processEnemiesTurn();
        
        std::cout << "\n--- TOWERS TURN ---\n";
        processTowersTurn();
        
        checkLevelCompletion();
        
        turn++;
    }
    
    if (isCompleted()) {
        std::cout << "\n LEVEL " << levelNumber << " COMPLETED! \n";
        player->increaseScore(50 * levelNumber);
    } else if (isFailed()) {
        std::cout << "\n LEVEL " << levelNumber << " FAILED! \n";
    }
}

void Level::processPlayerTurn(char input) {
    switch (input) {
        case 'w': case 'W': 
            player->move(0, -1);
            handleAutoAttack();
            break;
        case 's': case 'S': 
            player->move(0, 1);
            handleAutoAttack();
            break;
        case 'a': case 'A': 
            player->move(-1, 0);
            handleAutoAttack();
            break;
        case 'd': case 'D': 
            player->move(1, 0);
            handleAutoAttack();
            break;
        case 'h': case 'H':
            player->heal(20);
            break;
        case 'c': case 'C':
            player->getSpellHand().displayHand();
            if (player->getSpellHand().getSpellCount() > 0) {
                int spellIndex = getInputInt("Select spell (1-" + 
                    std::to_string(player->getSpellHand().getSpellCount()) + "): ", 
                    1, player->getSpellHand().getSpellCount()) - 1;
                int targetX = getInputInt("Enter target X: ", 0, gameField->getWidth() - 1);
                int targetY = getInputInt("Enter target Y: ", 0, gameField->getHeight() - 1);
                player->castSpell(spellIndex, targetX, targetY);
            }
            break;
        case 't': case 'T':
            break;
        case 'k': case 'K':  // Quick Save
            saveGame();
            break;
        case 'l': case 'L':  // Load Game Menu
            showSaveLoadMenu();
            break;
        case 'x': case 'X':
            levelFailed = true;
            std::cout << "Level exited by player.\n";
            break;
        default:
            std::cout << "Invalid command!\n";
            break;
    }
}

void Level::processEnemiesTurn() {
    int enemiesMoved = 0;
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.moveTowardsPlayer(player->getX(), player->getY(), *player);
            enemiesMoved++;
        }
    }
    std::cout << enemiesMoved << " enemies moved.\n";
}

void Level::processTowersTurn() {
    int towersActed = 0;
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.update(*player);
            if (tower.canAttackThisTurn()) {
                towersActed++;
            }
            tower.resetAttack();
        }
    }
    std::cout << towersActed << " towers acted.\n";
}

void Level::checkLevelCompletion() {
    if (isWinConditionMet()) {
        levelCompleted = true;
    } else if (isLoseConditionMet()) {
        levelFailed = true;
    }
}

bool Level::isCompleted() const {
    return levelCompleted;
}

bool Level::isFailed() const {
    return levelFailed;
}

bool Level::isRunning() const {
    return !levelCompleted && !levelFailed && player->isPlayerAlive();
}

void Level::displayLevelInfo() const {
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) aliveEnemies++;
    }
    
    int aliveTowers = 0;
    for (const auto& tower : towers) {
        if (tower.isAlive()) aliveTowers++;
    }
    
    std::cout << "Level " << levelNumber << " Progress:\n";
    std::cout << "Enemies: " << (enemies.size() - aliveEnemies) << "/" << enemies.size() << " defeated\n";
    std::cout << "Towers: " << (towers.size() - aliveTowers) << "/" << towers.size() << " destroyed\n";
    
    if (player->getHealth() < 30) {
        std::cout << " WARNING: Low health! Use H to heal.\n";
    }
}

void Level::createEnemies() {
    int width = gameField->getWidth();
    int height = gameField->getHeight();
    
    // Упрощенная логика - создаем врагов в углах
    std::vector<std::pair<int, int>> cornerPositions = {
        {width-1, 0},      // правый верхний
        {0, height-1},     // левый нижний
        {width-1, height-1} // правый нижний
    };
    
    int enemyCount = std::min(2 + levelNumber, 3); // Максимум 3 врага
    
    for (int i = 0; i < enemyCount; ++i) {
        if (i < static_cast<int>(cornerPositions.size())) {
            int x = cornerPositions[i].first;
            int y = cornerPositions[i].second;
            
            if (gameField->isCellPassable(x, y) && 
                !(x == player->getX() && y == player->getY())) {
                
                int health = 25 + levelNumber * 5;
                int damage = 4 + levelNumber;
                enemies.push_back(Enemy(gameField, x, y, health, damage));
                std::cout << "Enemy created at (" << x << ", " << y << ") with health " << health << "\n";
            }
        }
    }
}

void Level::createTowers() {
    if (levelNumber < 2) {
        std::cout << "No towers in level 1\n";
        return;
    }
    
    int width = gameField->getWidth();
    int height = gameField->getHeight();
    
    // Создаем башни по центру сторон
    std::vector<std::pair<int, int>> towerPositions = {
        {width/2, 0},           // верхний центр
        {width/2, height-1},    // нижний центр  
        {0, height/2},          // левый центр
        {width-1, height/2}     // правый центр
    };
    
    int towerCount = std::min(levelNumber - 1, 2); // Максимум 2 башни
    
    for (int i = 0; i < towerCount; ++i) {
        if (i < static_cast<int>(towerPositions.size())) {
            int x = towerPositions[i].first;
            int y = towerPositions[i].second;
            
            if (gameField->isCellPassable(x, y) && 
                (abs(x - player->getX()) > 2 || abs(y - player->getY()) > 2)) {
                
                int health = 50 + levelNumber * 15;
                int damage = 6 + levelNumber;
                int range = 4;
                towers.push_back(EnemyTower(gameField, x, y, health, damage, range));
                std::cout << "Tower created at (" << x << ", " << y << ") with health " << health << "\n";
            }
        }
    }
    
    if (towers.empty()) {
        std::cout << "Warning: No towers could be placed!\n";
    }
}

void Level::handleAutoAttack() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int distX = std::abs(enemy.getX() - player->getX());
            int distY = std::abs(enemy.getY() - player->getY());
            
            // Атакуем если враг на соседней клетке (не по диагонали)
            if ((distX == 1 && distY == 0) || (distX == 0 && distY == 1)) {
                player->attackEnemy(enemy);
                return; // Атакуем только одного врага за ход
            }
        }
    }
}

bool Level::isWinConditionMet() const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) return false;
    }
    for (const auto& tower : towers) {
        if (tower.isAlive()) return false;
    }
    return enemies.size() > 0;
}

bool Level::isLoseConditionMet() const {
    return !player->isPlayerAlive();
}

// ========== СИСТЕМА СОХРАНЕНИЯ/ЗАГРУЗКИ ДЛЯ LEVEL ==========

void Level::saveGame() {
    GameSaveData saveData;
    
    // Сохраняем данные игрока
    saveData.playerName = player->getName();
    saveData.playerHealth = player->getHealth();
    saveData.playerMaxHealth = player->getMaxHealth();
    saveData.playerDamage = player->getDamage();
    saveData.playerScore = player->getScore();
    saveData.playerLevel = player->getLevel();
    saveData.playerX = player->getX();
    saveData.playerY = player->getY();
    saveData.playerMana = player->getMana();
    saveData.playerMaxMana = player->getMaxMana();
    
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
    
    // Сохраняем состояние уровня
    saveData.currentTurn = 1;
    saveData.currentLevel = levelNumber;
    saveData.gameRunning = isRunning();
    
    std::string slotName = getSaveSlotName();
    if (!slotName.empty()) {
        if (SaveSystem::saveGame(saveData, slotName)) {
            std::cout << "Level " << levelNumber << " saved successfully to slot: " << slotName << "!\n";
        }
    }
}

void Level::loadGame() {
    // В Level загрузка не должна завершать уровень
    std::cout << "Loading is only available from the main menu.\n";
    std::cout << "Please use 'X' to exit to main menu and load your game there.\n";
}

void Level::showSaveLoadMenu() {
    std::cout << "\n=== SAVE/LOAD MENU ===\n";
    std::cout << "1. Save Game\n";
    std::cout << "2. Load Game Info\n";
    std::cout << "3. Delete Save\n";
    std::cout << "4. Back to Game\n";
    
    int choice = getInputInt("Select option (1-4): ", 1, 4);
    
    switch (choice) {
        case 1:
            saveGame();
            break;
        case 2:
            showSaveInfo();
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

void Level::showSaveInfo() {
    auto saves = SaveSystem::getAvailableSaves();
    if (saves.empty()) {
        std::cout << "No save files found!\n";
        return;
    }
    
    std::cout << "Available saves:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i + 1 << ". " << saves[i] << "\n";
    }
    std::cout << "Use 'X' to exit to main menu and load a saved game.\n";
}

std::string Level::getSaveSlotName() {
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