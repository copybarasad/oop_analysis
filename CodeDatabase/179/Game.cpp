#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "GameRenderer.h"
#include "GameException.h"
#include "GameEventNotifier.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <cstdio>  // ДЛЯ std::remove
#include <random>
#include <algorithm>
#include <filesystem>

Game::Game() : gameRunning(false), currentLevel(1), spellHand(5) {
    player = std::make_shared<Player>();
    LOG_INFO("Game initialized with new player");
}

void Game::startNewGame() {
    currentLevel = 1;
    initializeLevel(currentLevel);
    gameRunning = true;
    
    spellHand.addRandomSpell();
    
    LOG_INFO("New game started at level " + std::to_string(currentLevel));
    
    std::cout << "=== Game Started ===\n";
    std::cout << "Level: " << currentLevel << "\n";
    std::cout << "Controls: w(up) s(down) a(left) d(right) m(switch mode) c(cast spell) r(ranged attack) p(show spells) save(save game) load(load game) x(exit)\n";
    
    GameEventNotifier::getInstance().notifyLevelUp(currentLevel);
}

bool Game::isRunning() const {
    return gameRunning;
}

void Game::processCommand(const GameCommand& command) {
    if (!gameRunning) return;
    
    try {
        switch (command.type) {
            case CommandType::MOVE_UP: {
                Position newPos = currentField->getPlayerPosition();
                Position oldPos = newPos; // Сохраняем старую позицию
                newPos.y--;
                if (currentField->movePlayer(newPos)) {
                    std::cout << "Moved up\n";
                    GameEventNotifier::getInstance().notifyPlayerMove(oldPos, newPos);
                } else {
                    std::cout << "Cannot move there!\n";
                }
                break;
            }
            case CommandType::MOVE_DOWN: {
                Position newPos = currentField->getPlayerPosition();
                Position oldPos = newPos;
                newPos.y++;
                if (currentField->movePlayer(newPos)) {
                    std::cout << "Moved down\n";
                    GameEventNotifier::getInstance().notifyPlayerMove(oldPos, newPos);
                } else {
                    std::cout << "Cannot move there!\n";
                }
                break;
            }
            case CommandType::MOVE_LEFT: {
                Position newPos = currentField->getPlayerPosition();
                Position oldPos = newPos;
                newPos.x--;
                if (currentField->movePlayer(newPos)) {
                    std::cout << "Moved left\n";
                    GameEventNotifier::getInstance().notifyPlayerMove(oldPos, newPos);
                } else {
                    std::cout << "Cannot move there!\n";
                }
                break;
            }
            case CommandType::MOVE_RIGHT: {
                Position newPos = currentField->getPlayerPosition();
                Position oldPos = newPos;
                newPos.x++;
                if (currentField->movePlayer(newPos)) {
                    std::cout << "Moved right\n";
                    GameEventNotifier::getInstance().notifyPlayerMove(oldPos, newPos);
                } else {
                    std::cout << "Cannot move there!\n";
                }
                break;
            }
            case CommandType::SWITCH_MODE: {
                player->switchFightMode();
                std::string mode = (player->getFightMode() == FightMode::MELEE ? "MELEE" : "RANGED");
                std::cout << "Switched to " << mode << " mode\n";
                LOG_DEBUG("Player switched to " + mode + " mode");
                break;
            }
            case CommandType::CAST_SPELL: {
                if (command.spellIndex >= spellHand.getSpellCount()) {
                    std::cout << "Invalid spell index!\n";
                    LOG_WARNING("Invalid spell index: " + std::to_string(command.spellIndex));
                    break;
                }
                
                Position target = getSpellTarget();
                auto spell = spellHand.getSpell(command.spellIndex);
                if (spell) {
                    LOG_DEBUG("Attempting to cast spell: " + spell->getName() + " at (" +
                             std::to_string(target.x) + "," + std::to_string(target.y) + ")");
                    
                    if (spellHand.castSpell(command.spellIndex, target, *currentField,
                                           currentField->getPlayerPosition())) {
                        std::cout << "Spell cast successfully!\n";
                        GameEventNotifier::getInstance().notifyPlayerCastSpell(spell->getName(), target);
                    } else {
                        std::cout << "Spell failed!\n";
                        LOG_WARNING("Spell cast failed");
                    }
                }
                break;
            }
            case CommandType::RANGED_ATTACK: {
                if (player->getFightMode() != FightMode::RANGED) {
                    std::cout << "Switch to RANGED mode first!\n";
                    break;
                }
                
                LOG_DEBUG("Ranged attack attempted at (" + std::to_string(command.target.x) +
                         "," + std::to_string(command.target.y) + ")");
                
                if (currentField->rangedAttack(command.target, player)) {
                    std::cout << "Ranged attack successful!\n";
                    GameEventNotifier::getInstance().notifyPlayerAttack(command.target,
                                                                       player->getRangedDamage(),
                                                                       "target");
                } else {
                    std::cout << "Ranged attack failed!\n";
                }
                break;
            }
            case CommandType::SHOW_SPELLS: {
                spellHand.displaySpells();
                break;
            }
            case CommandType::SAVE_GAME: {
                saveGame("savegame.dat");
                break;
            }
            case CommandType::LOAD_GAME: {
                loadGame("savegame.dat");
                break;
            }
            case CommandType::EXIT: {
                gameRunning = false;
                std::cout << "Game exited\n";
                LOG_INFO("Game exited by player");
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
        LOG_ERROR("Error processing command: " + std::string(e.what()));
    }
}

void Game::gameLoop() {
    while (gameRunning) {
        displayGameState();
        playerTurn();
        if (!gameRunning) break;
        
        alliesTurn();
        enemiesTurn();
        buildingsTurn();
        towersTurn();
        currentField->moveAllies();
        currentField->checkTraps();
        
        checkGameState();
        
        player->restoreMana(5);
    }
}

void Game::saveGame(const std::string& filename) {
    auto path = std::filesystem::current_path() / filename;
    std::cout << "💾 Saving to: " << path << std::endl;
    LOG_INFO("Attempting to save game to: " + path.string());
    
    try {
        // Удаляем старый файл если существует
        std::remove(filename.c_str());
        
        // Открываем файл с перезаписью
        std::ofstream file(filename, std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            LOG_ERROR("Cannot open file for writing: " + filename);
            throw SaveGameException("Cannot open file: " + filename);
        }
        
        // Сохраняем основные данные игры
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        file.write(reinterpret_cast<const char*>(&gameRunning), sizeof(gameRunning));
        
        // Сохраняем данные игрока
        if (player) {
            int health = player->getHealth();
            int mana = player->getMana();
            int score = player->getScore();
            FightMode mode = player->getFightMode();
            int maxHealth = player->getMaxHealth();
            int maxMana = player->getMaxMana();
            int meleeDamage = player->getMeleeDamage();
            int rangedDamage = player->getRangedDamage();
            
            file.write(reinterpret_cast<const char*>(&health), sizeof(health));
            file.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
            file.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
            file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
            file.write(reinterpret_cast<const char*>(&maxMana), sizeof(maxMana));
            file.write(reinterpret_cast<const char*>(&meleeDamage), sizeof(meleeDamage));
            file.write(reinterpret_cast<const char*>(&rangedDamage), sizeof(rangedDamage));
        }
        
        // Сохраняем игровое поле
        if (currentField) {
            currentField->save(file);
        }
        
        // Сохраняем заклинания
        spellHand.save(file);
        
        // Проверяем успешность записи
        if (file.fail()) {
            LOG_ERROR("Write error during save");
            throw SaveGameException("Write error during save");
        }
        
        file.close();
        
        std::cout << "Game saved successfully to " << filename << "\n";
        std::cout << "File size created.\n";
        
        LOG_INFO("Game saved successfully to " + filename);
        GameEventNotifier::getInstance().notifyGameSaved(filename);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Save game exception: " + std::string(e.what()));
        throw SaveGameException(e.what());
    }
}

void Game::loadGame(const std::string& filename) {
    LOG_INFO("Attempting to load game from: " + filename);
    
    try {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            LOG_ERROR("File not found: " + filename);
            throw FileNotFoundException(filename);
        }
        
        // Загружаем основные данные
        int savedLevel;
        bool savedGameRunning;
        
        file.read(reinterpret_cast<char*>(&savedLevel), sizeof(savedLevel));
        file.read(reinterpret_cast<char*>(&savedGameRunning), sizeof(savedGameRunning));
        
        if (file.fail()) {
            LOG_ERROR("Cannot read game state from save file");
            throw CorruptedSaveException("Cannot read game state");
        }
        
        currentLevel = savedLevel;
        gameRunning = savedGameRunning;
        
        LOG_INFO("Loaded game state: Level=" + std::to_string(currentLevel) +
                ", Running=" + std::to_string(gameRunning));
        
        // Загружаем данные игрока
        if (player) {
            int health, mana, score, maxHealth, maxMana, meleeDamage, rangedDamage;
            FightMode mode;
            
            file.read(reinterpret_cast<char*>(&health), sizeof(health));
            file.read(reinterpret_cast<char*>(&mana), sizeof(mana));
            file.read(reinterpret_cast<char*>(&score), sizeof(score));
            file.read(reinterpret_cast<char*>(&mode), sizeof(mode));
            file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
            file.read(reinterpret_cast<char*>(&maxMana), sizeof(maxMana));
            file.read(reinterpret_cast<char*>(&meleeDamage), sizeof(meleeDamage));
            file.read(reinterpret_cast<char*>(&rangedDamage), sizeof(rangedDamage));
            
            if (file.fail()) {
                LOG_ERROR("Cannot read player data from save file");
                throw CorruptedSaveException("Cannot read player data");
            }
            
            player->setMaxHealth(maxHealth);  // Сначала максимальное здоровье
            player->setMaxMana(maxMana);      // Потом максимальная мана
            player->setHealth(health);        // Потом текущее здоровье
            player->setMana(mana);            // Потом текущая мана
            player->addScore(score);
            player->setMeleeDamage(meleeDamage);
            player->setRangedDamage(rangedDamage);
            
            if (player->getFightMode() != mode) {
                player->switchFightMode();
            }
            
            LOG_DEBUG("Player loaded: Health=" + std::to_string(health) + "/" + std::to_string(maxHealth) +
                     ", Mana=" + std::to_string(mana) + "/" + std::to_string(maxMana) +
                     ", Score=" + std::to_string(score));
        }
        
        // Создаем временное поле
        currentField = std::make_unique<GameField>(10, 10);
        
        // Загружаем игровое поле
        if (currentField) {
            currentField->load(file);
            
            // Устанавливаем игрока в поле после загрузки
            if (player) {
                Position playerPos = currentField->getPlayerPosition();
                currentField->placePlayer(playerPos, player);
            }
        }
        
        // Загружаем заклинания
        spellHand.load(file);
        
        std::cout << "Game loaded successfully from " << filename << "\n";
        std::cout << "Level: " << currentLevel << " | Score: " << player->getScore() << "\n";
        std::cout << "Health: " << player->getHealth() << "/" << player->getMaxHealth() << "\n";
        std::cout << "Mana: " << player->getMana() << "/" << player->getMaxMana() << "\n";
        std::cout << "Spells: " << spellHand.getSpellCount() << " | Enemies: " << (currentField ? currentField->getEnemies().size() : 0) << "\n";
        
        LOG_INFO("Game loaded successfully from " + filename);
        LOG_INFO("Level: " + std::to_string(currentLevel) + ", Score: " + std::to_string(player->getScore()) +
                ", Spells: " + std::to_string(spellHand.getSpellCount()));
        
        GameEventNotifier::getInstance().notifyGameLoaded(filename);
        GameEventNotifier::getInstance().notifyLevelUp(currentLevel);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Load game exception: " + std::string(e.what()));
        throw LoadGameException(e.what());
    }
}

void Game::playerTurn() {
    if (player->shouldSkipTurn()) {
        std::cout << "\n>>> 💤 You're stuck in swamp! Skipping turn...\n";
        LOG_DEBUG("Player turn skipped due to swamp");
        player->resetTurnSkip();
        return;
    }
    
    std::cout << "\n>>> YOUR TURN: ";
    
    std::string input;
    std::cin >> input;
    
    if (input == "w") processCommand(GameCommand(CommandType::MOVE_UP));
    else if (input == "s") processCommand(GameCommand(CommandType::MOVE_DOWN));
    else if (input == "a") processCommand(GameCommand(CommandType::MOVE_LEFT));
    else if (input == "d") processCommand(GameCommand(CommandType::MOVE_RIGHT));
    else if (input == "m") processCommand(GameCommand(CommandType::SWITCH_MODE));
    else if (input == "c") {
        spellHand.displaySpells();
        if (spellHand.getSpellCount() > 0) {
            std::cout << "Enter spell number: ";
            size_t spellIndex;
            std::cin >> spellIndex;
            if (spellIndex > 0 && spellIndex <= spellHand.getSpellCount()) {
                processCommand(GameCommand(CommandType::CAST_SPELL, spellIndex - 1));
            } else {
                std::cout << "Invalid spell number!\n";
                LOG_WARNING("Invalid spell number entered: " + std::to_string(spellIndex));
            }
        } else {
            std::cout << "You have no spells!\n";
        }
    }
    else if (input == "r") {
        if (player->getFightMode() == FightMode::RANGED) {
            Position target = getSpellTarget();
            processCommand(GameCommand(CommandType::RANGED_ATTACK, target));
        } else {
            std::cout << "Switch to RANGED mode first using 'm'!\n";
        }
    }
    else if (input == "p") processCommand(GameCommand(CommandType::SHOW_SPELLS));
    else if (input == "save") processCommand(GameCommand(CommandType::SAVE_GAME));
    else if (input == "load") processCommand(GameCommand(CommandType::LOAD_GAME));
    else if (input == "x") processCommand(GameCommand(CommandType::EXIT));
    else if (input == "help") {
        std::cout << "\n=== HELP ===\n";
        std::cout << "w/a/s/d - Move\n";
        std::cout << "m - Switch fight mode\n";
        std::cout << "c - Cast spell\n";
        std::cout << "r - Ranged attack (in RANGED mode)\n";
        std::cout << "p - Show spells\n";
        std::cout << "save - Save game\n";
        std::cout << "load - Load game\n";
        std::cout << "x - Exit\n";
        std::cout << "============\n";
    }
    else {
        std::cout << "Unknown command! Use: W,A,S,D,M,C,R,P,SAVE,LOAD,X or type 'help'\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        LOG_DEBUG("Unknown command entered: " + input);
    }
}

void Game::levelUp() {
    std::cout << "\n=== LEVEL UP! ===\n";
    std::cout << "Choose upgrade:\n";
    std::cout << "1. +20 Max Health (Current: " << player->getMaxHealth() << ")\n";
    std::cout << "2. +5 Melee Damage (Current: " << player->getMeleeDamage() << ")\n";
    std::cout << "3. +3 Ranged Damage (Current: " << player->getRangedDamage() << ")\n";
    std::cout << "4. +10 Max Mana (Current: " << player->getMaxMana() << ")\n";
    
    int choice;
    std::cin >> choice;
    
    std::string upgradeType;
    
    switch (choice) {
        case 1:
            player->setMaxHealth(player->getMaxHealth() + 20);
            player->setHealth(player->getMaxHealth()); // Полное исцеление
            upgradeType = "Max Health to " + std::to_string(player->getMaxHealth());
            std::cout << "Max health increased to " << player->getMaxHealth() << "!\n";
            break;
        case 2:
            player->setMeleeDamage(player->getMeleeDamage() + 5);
            upgradeType = "Melee Damage to " + std::to_string(player->getMeleeDamage());
            std::cout << "Melee damage increased to " << player->getMeleeDamage() << "!\n";
            break;
        case 3:
            player->setRangedDamage(player->getRangedDamage() + 3);
            upgradeType = "Ranged Damage to " + std::to_string(player->getRangedDamage());
            std::cout << "Ranged damage increased to " << player->getRangedDamage() << "!\n";
            break;
        case 4:
            player->setMaxMana(player->getMaxMana() + 10);
            player->setMana(player->getMaxMana());
            upgradeType = "Max Mana to " + std::to_string(player->getMaxMana());
            std::cout << "Max mana increased to " << player->getMaxMana() << "!\n";
            break;
        default:
            std::cout << "Invalid choice. Health automatically upgraded.\n";
            player->setMaxHealth(player->getMaxHealth() + 10);
            player->setHealth(player->getMaxHealth());
            upgradeType = "Default Health to " + std::to_string(player->getMaxHealth());
    }
    
    LOG_INFO("Player leveled up: " + upgradeType);
}

void Game::alliesTurn() {
    // Реализация хода союзников
    auto allies = currentField->getAllies();
    if (!allies.empty()) {
        LOG_DEBUG("Allies turn: " + std::to_string(allies.size()) + " allies acting");
    }
}

void Game::enemiesTurn() {
    currentField->moveEnemies();
    currentField->removeDeadEnemies();
    
    auto enemies = currentField->getEnemies();
    if (!enemies.empty()) {
        LOG_DEBUG("Enemies turn: " + std::to_string(enemies.size()) + " enemies moved");
    }
}

void Game::buildingsTurn() {
    auto enemiesToSpawn = currentField->getEnemiesToSpawn();
    for (auto& enemy : enemiesToSpawn) {
        auto buildings = currentField->getBuildings();
        for (auto& buildingPair : buildings) {
            Position buildingPos = buildingPair.first;
            
            for (int attempt = 0; attempt < 8; ++attempt) {
                Position spawnPos = currentField->getRandomAdjacentPosition(buildingPos);
                
                if (currentField->isValidPosition(spawnPos) &&
                    currentField->getCell(spawnPos).isPassableForMovement()) {
                    if (currentField->placeEnemy(spawnPos, enemy)) {
                        std::cout << "Enemy building spawned a new enemy!\n";
                        LOG_DEBUG("Enemy building spawned enemy at (" +
                                 std::to_string(spawnPos.x) + "," + std::to_string(spawnPos.y) + ")");
                        GameEventNotifier::getInstance().notify(GameEvent(
                            GameEventType::ENEMY_SPAWN,
                            "Enemy spawned by building",
                            spawnPos
                        ));
                        break;
                    }
                }
            }
        }
    }
}

void Game::towersTurn() {
    currentField->attackPlayerFromTowers();
}

void Game::checkGameState() {
    if (!player->isAlive()) {
        std::cout << "Game Over! You died.\n";
        LOG_INFO("Game over - player died");
        GameEventNotifier::getInstance().notifyGameOver(false);
        gameRunning = false;
        return;
    }
    
    auto enemies = currentField->getEnemies();
    auto buildings = currentField->getBuildings();
    auto towers = currentField->getTowers();
    
    if (enemies.empty() && buildings.empty() && towers.empty()) {
        std::cout << "Level " << currentLevel << " completed!\n";
        LOG_INFO("Level " + std::to_string(currentLevel) + " completed!");
        
        // Прокачка между уровнями
        levelUp();
        
        currentLevel++;
        player->addScore(100 * currentLevel);
        player->setHealth(player->getMaxHealth());
        player->setMana(player->getMaxMana());
        spellHand.addRandomSpell();
        
        std::cout << "You gained a new spell and recovered health and mana!\n";
        std::cout << "Score: " << player->getScore() << "\n";
        
        LOG_INFO("Player gained new spell and recovered resources");
        LOG_INFO("New score: " + std::to_string(player->getScore()));
        
        std::cout << "Press Enter to continue to next level...";
        std::cin.ignore();
        std::cin.get();
        
        initializeLevel(currentLevel);
        std::cout << "Starting level " << currentLevel << "\n";
        
        GameEventNotifier::getInstance().notifyLevelUp(currentLevel);
    }
}

void Game::displayGameState() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::cout << "========================================\n";
    std::cout << "          FANTASY BATTLE GAME\n";
    std::cout << "========================================\n";
    
    // Используем GameRenderer через интерфейс
    ConsoleRenderer renderer;
    renderer.renderGameState(*currentField, currentLevel, player->getScore());
    renderer.renderField(*currentField);
    
    auto enemies = currentField->getEnemies();
    auto buildings = currentField->getBuildings();
    auto towers = currentField->getTowers();
    
    std::cout << "\nEnemies on map: " << enemies.size()
              << " | Buildings: " << buildings.size()
              << " | Towers: " << towers.size() << "\n";
    
    renderer.renderHelp();
}

Position Game::getSpellTarget() {
    std::cout << "Enter target coordinates (x y): ";
    int x, y;
    std::cin >> x >> y;
    return Position(x, y);
}

void Game::initializeLevel(int level) {
    LOG_INFO("Initializing level " + std::to_string(level));
    
    int size = 10 + level * 2;
    if (size > 25) size = 25;
    
    currentField = std::make_unique<GameField>(size, size);
    player->resetTurnSkip();
    bool playerPlaced = false;
    std::vector<Position> startPositions = {
        {0, 0}, {1, 0}, {0, 1}, {1, 1},
        {size-1, 0}, {size-2, 0}, {size-1, 1}, {size-2, 1},
        {0, size-1}, {1, size-1}, {0, size-2}, {1, size-2},
        {size-1, size-1}, {size-2, size-1}, {size-1, size-2}, {size-2, size-2}
    };
    
    for (const auto& pos : startPositions) {
        if (currentField->placePlayer(pos, player)) {
            playerPlaced = true;
            std::cout << "Player placed at (" << pos.x << ", " << pos.y << ")\n";
            LOG_DEBUG("Player placed at (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")");
            break;
        }
    }
    
    if (!playerPlaced) {
        std::cout << "Searching for free position for player...\n";
        LOG_WARNING("No suitable start position found, searching entire field");
        for (int y = 0; y < size && !playerPlaced; ++y) {
            for (int x = 0; x < size && !playerPlaced; ++x) {
                Position pos(x, y);
                if (currentField->placePlayer(pos, player)) {
                    playerPlaced = true;
                    std::cout << "Player placed at (" << pos.x << ", " << pos.y << ")\n";
                    LOG_DEBUG("Player placed at (" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")");
                }
            }
        }
    }
    
    if (!playerPlaced) {
        std::cout << "ERROR: Cannot place player on level! No free positions.\n";
        LOG_ERROR("Cannot place player - no free positions on level");
        gameRunning = false;
        return;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posDis(3, size - 1);
    std::uniform_int_distribution<> enemyCountDis(level, level + 2);
    
    int enemyCount = enemyCountDis(gen);
    int enemiesPlaced = 0;
    
    LOG_DEBUG("Placing " + std::to_string(enemyCount) + " enemies on level");
    
    for (int i = 0; i < enemyCount * 2; ++i) {
        if (enemiesPlaced >= enemyCount) break;
        
        Position enemyPos(posDis(gen), posDis(gen));
        int enemyHealth = 30 + level * 5;
        int enemyDamage = 10 + level * 2;
        
        if (currentField->placeEnemy(enemyPos, std::make_shared<Enemy>(enemyHealth, enemyDamage))) {
            enemiesPlaced++;
            LOG_DEBUG("Enemy placed at (" + std::to_string(enemyPos.x) + "," + std::to_string(enemyPos.y) +
                     ") with HP=" + std::to_string(enemyHealth));
        }
    }
    
    std::cout << "Placed " << enemiesPlaced << " enemies\n";
    LOG_INFO("Placed " + std::to_string(enemiesPlaced) + " enemies on level");
    
    // Размещаем здания
    if (level >= 2) {
        int buildingsPlaced = 0;
        for (int attempt = 0; attempt < 10 && buildingsPlaced < 1; ++attempt) {
            Position buildingPos(posDis(gen), posDis(gen));
            if (currentField->placeBuilding(buildingPos,
                std::make_shared<EnemyBuilding>(100 + level * 20, 5 - level/2))) {
                buildingsPlaced++;
                LOG_DEBUG("Enemy building placed at (" + std::to_string(buildingPos.x) + "," +
                         std::to_string(buildingPos.y) + ")");
            }
        }
    }
    
    // Размещаем башни
    if (level >= 3) {
        int towersPlaced = 0;
        for (int attempt = 0; attempt < 10 && towersPlaced < 1; ++attempt) {
            Position towerPos(posDis(gen), posDis(gen));
            if (currentField->placeTower(towerPos,
                std::make_shared<EnemyTower>(80 + level * 10, 15 + level * 3))) {
                towersPlaced++;
                LOG_DEBUG("Enemy tower placed at (" + std::to_string(towerPos.x) + "," +
                         std::to_string(towerPos.y) + ")");
            }
        }
    }
}

// Добавляем недостающие методы доступа
GameField* Game::getCurrentField() const {
    return currentField.get();
}

int Game::getCurrentLevel() const {
    return currentLevel;
}

std::shared_ptr<Player> Game::getPlayer() const {
    return player;
}
