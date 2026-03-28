#include "GameSession.h"
#include "ExceptionHandler.h"
#include <iostream>
#include <algorithm>

GameSession::GameSession() : 
    board(10, 10),
    playerHero(100, 15, 10, Location(0,0)),
    spellShop(playerHero.getSpellHand()),
    turnManager(true),
    saveManager(),
    levelManager(),
    gameRunning(false),
    gameLoaded(false) {
    
    // Инициализируем доску препятствиями
    board.generateRandomObstacles(3, 6);
}

void GameSession::processEnemyTurn() {
    // Логика хода врагов
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            Location heroPos = playerHero.getPosition();
            Location enemyPos = enemy.getPosition();
            
            int diffRow = static_cast<int>(heroPos.row) - static_cast<int>(enemyPos.row);
            int diffCol = static_cast<int>(heroPos.col) - static_cast<int>(enemyPos.col);
            
            if (std::abs(diffCol) > std::abs(diffRow)) {
                enemy.performMove(diffCol > 0 ? MoveDirection::East : MoveDirection::West, board);
            } else {
                enemy.performMove(diffRow > 0 ? MoveDirection::South : MoveDirection::North, board);
            }
        }
    }
    
    // Логика хода башен
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.performTurn(board, playerHero.getPosition());
        }
    }
    
    cleanupDestroyedActors();
}

void GameSession::checkCollisions() {
    // Проверка столкновений с врагами
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == playerHero.getPosition()) {
            playerHero.handleCollision(&enemy);
            if (!enemy.isAlive()) {
                playerHero.getSpellHand().addMana(45);
                playerHero.incrementEnemiesDefeated();
            }
        }
    }
}

void GameSession::cleanupDestroyedActors() {
    std::cout << "DEBUG: Cleaning up destroyed actors\n";
    
    // Удаляем мертвых врагов
    size_t beforeEnemies = enemies.size();
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), 
            [](const Foe& foe) { 
                bool alive = foe.isAlive();
                if (!alive) {
                    std::cout << "DEBUG: Removing dead enemy\n";
                }
                return !alive; 
            }),
        enemies.end()
    );
    size_t afterEnemies = enemies.size();
    std::cout << "DEBUG: Enemies before/after cleanup: " << beforeEnemies << "/" << afterEnemies << "\n";
    
    // Удаляем мертвые башни
    size_t beforeTowers = towers.size();
    towers.erase(
        std::remove_if(towers.begin(), towers.end(), 
            [](const EnemyTower& tower) { 
                bool alive = tower.isAlive();
                if (!alive) {
                    std::cout << "DEBUG: Removing dead tower\n";
                }
                return !alive; 
            }),
        towers.end()
    );
    size_t afterTowers = towers.size();
    std::cout << "DEBUG: Towers before/after cleanup: " << beforeTowers << "/" << afterTowers << "\n";
    
    std::cout << "DEBUG: Cleanup completed\n";
}


bool GameSession::isGameOver() const {
    return isVictory() || isDefeat();
}

bool GameSession::isVictory() const {
    return enemies.empty() && towers.empty();
}

bool GameSession::isDefeat() const {
    return !playerHero.isAlive();
}

void GameSession::saveGame(const std::string& filename) {
    try {
        GameSaveData data;
        
        data.playerHitpoints = playerHero.getHitpoints();
        data.playerMaxHitpoints = playerHero.getMaxHitpoints();
        data.playerPosition = playerHero.getPosition();
        data.meleeMode = playerHero.isMeleeMode();
        data.enemiesDefeated = playerHero.getEnemiesDefeated();
        data.mana = playerHero.getSpellHand().getMana();
        data.currentLevel = levelManager.getCurrentLevel();
        
        data.boardWidth = board.getWidth();
        data.boardHeight = board.getHeight();
        
        for (const auto& enemy : enemies) {
            if (enemy.isAlive()) {
                data.enemies.emplace_back(enemy.getPosition(), enemy.getHitpoints(), enemy.getAttackPower());
            }
        }
        
        for (const auto& tower : towers) {
            if (tower.isAlive()) {
                data.towers.emplace_back(tower.getPosition(), tower.getHitpoints(), tower.getAttackPower(), tower.getAttackRange());
            }
        }
        
        // Сохраняем имена заклинаний
        for (size_t i = 0; i < playerHero.getSpellHand().getSpellCount(); ++i) {
            const Spell* spell = playerHero.getSpellHand().getSpell(i);
            if (spell) {
                data.spellNames.push_back(spell->getName());
            }
        }
        
        if (saveManager.saveGame(filename, data)) {
            std::cout << "Game saved successfully to " << filename << "\n";
            saveManager.setCurrentSave(filename);
        }
    } catch (const std::exception& e) {
        std::cout << "Failed to save game: " << e.what() << "\n";
    }
}

bool GameSession::loadGame(const std::string& filename) {
    try {
        GameSaveData data;
        if (!saveManager.loadGame(filename, data)) {
            return false;
        }
        
        // Очищаем текущее состояние
        enemies.clear();
        towers.clear();
        board.removeAllActors();
        
        // Создаем новую доску
        board = GameBoard(data.boardWidth, data.boardHeight);
        
        // Восстанавливаем героя
        playerHero.setHitpoints(data.playerHitpoints);
        playerHero.setPosition(data.playerPosition);
        
        if (data.meleeMode != playerHero.isMeleeMode()) {
            playerHero.toggleCombatStyle();
        }
        
        playerHero.getSpellHand().setMana(data.mana);
        
        // Очищаем заклинания
        while (playerHero.getSpellHand().getSpellCount() > 0) {
            playerHero.getSpellHand().removeSpell(0);
        }
        
        // Восстанавливаем врагов
        for (const auto& enemyData : data.enemies) {
            enemies.emplace_back(std::get<1>(enemyData), std::get<2>(enemyData), std::get<0>(enemyData));
            board.addActor(&enemies.back());
        }
        
        // Восстанавливаем башни
        for (const auto& towerData : data.towers) {
            towers.emplace_back(std::get<1>(towerData), std::get<2>(towerData), std::get<0>(towerData), std::get<3>(towerData));
            board.addActor(&towers.back());
        }
        
        // Восстанавливаем заклинания
        for (const auto& spellName : data.spellNames) {
            // Создаем базовые заклинания по имени
            if (spellName.find("Fire") != std::string::npos || spellName.find("Lightning") != std::string::npos) {
                playerHero.getSpellHand().addSpell(
                    std::make_unique<DirectDamageSpell>(spellName, 25, 30, 5)
                );
            } else if (spellName.find("Ice") != std::string::npos || spellName.find("Nova") != std::string::npos) {
                playerHero.getSpellHand().addSpell(
                    std::make_unique<AreaDamageSpell>(spellName, 20, 40, 5)
                );
            } else if (spellName.find("Trap") != std::string::npos) {
                playerHero.getSpellHand().addSpell(
                    std::make_unique<TrapSpell>(spellName, 30, 35, 999)
                );
            }
        }
        
        board.addActor(&playerHero);
        
        levelManager.setCurrentLevel(data.currentLevel);
        
        // Генерируем случайные препятствия
        board.generateRandomObstacles(3, 6);
        
        gameLoaded = true;
        return true;
    } catch (const std::exception& e) {
        gameLoaded = false;
        std::cout << "Failed to load game: " << e.what() << "\n";
        return false;
    }
}

void GameSession::startNewGame() {
    // Очищаем текущее состояние
    enemies.clear();
    towers.clear();
    board.removeAllActors();
    
    // Создаем новую доску
    board = GameBoard(10, 10);
    board.generateRandomObstacles(3, 6);
    
    // Сбрасываем героя (не используем placement new)
    // Вместо этого создаем нового героя через присваивание
    Hero tempHero(100, 15, 10, Location(0,0));
    playerHero = std::move(tempHero);
    
    // Сбрасываем магазин (просто создаем новый)
    SpellShop tempShop(playerHero.getSpellHand());
    spellShop = std::move(tempShop);
    
    levelManager.setCurrentLevel(1);
    
    // Создаем врагов
    enemies.emplace_back(30, 8, Location(5, 5));
    enemies.emplace_back(30, 8, Location(7, 7));
    
    // Создаем башни
    towers.emplace_back(50, 15, Location(8, 2), 4);
    towers.emplace_back(50, 15, Location(2, 8), 4);
    
    // Добавляем акторов на доску
    for (auto& enemy : enemies) {
        board.addActor(&enemy);
    }
    
    for (auto& tower : towers) {
        board.addActor(&tower);
    }
    
    board.addActor(&playerHero);
    
    // Даем случайное заклинание
    spellShop.giveRandomStarterSpell();
    
    gameLoaded = true;
    std::cout << "New game started! Level " << levelManager.getCurrentLevel() << "\n";
}

void GameSession::initializeGame() {
    startNewGame();
}