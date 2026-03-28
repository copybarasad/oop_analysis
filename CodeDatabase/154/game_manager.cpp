#include "game_manager.h"
#include "game_exceptions.h"
#include "key_bindings_config.h"
#include "event_system.h"
#include <iostream>
#include "enemy.h"
#include "ally.h"
#include "tower.h"
#include "building.h"
#include "trap_spell.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>

GameManager::GameManager()
    : field_(nullptr),
      player_(nullptr),
      levelManager_(nullptr),
      upgradeSystem_(nullptr),
      saveSystem_(nullptr),
      gameState_(GameState_Enum::MainMenu),
      currentLevel_(1),
      playerWon_(false) {
    saveSystem_ = new SaveSystem();
    // Initialize default key bindings (may be overridden by config)
    loadDefaultBindings();
}

GameManager::~GameManager() {
    cleanup();
}

void GameManager::cleanup() {
    for (Entity* entity : allEntities_) {
        delete entity;
    }
    allEntities_.clear();
    
    if (field_) delete field_;
    if (levelManager_) delete levelManager_;
    if (upgradeSystem_) delete upgradeSystem_;
    if (saveSystem_) delete saveSystem_;
    if (player_) delete player_;
    
    field_ = nullptr;
    player_ = nullptr;
    levelManager_ = nullptr;
    upgradeSystem_ = nullptr;
}

void GameManager::run() {
    while (gameState_ != GameState_Enum::Exit) {
        try {
            switch (gameState_) {
                case GameState_Enum::MainMenu:
                    showMainMenu();
                    break;
                case GameState_Enum::Playing:
                    playLevel();
                    break;
                case GameState_Enum::LevelComplete:
                    handleLevelComplete();
                    break;
                case GameState_Enum::GameOver:
                    showGameOver();
                    break;
                default:
                    gameState_ = GameState_Enum::Exit;
                    break;
            }
        } catch (const GameException& e) {
            std::cerr << "\n[ERROR GAME] " << e.what() << "\n";
            std::cout << "Return to main menu? (y/n): ";
            char ch;
            if (std::cin >> ch && ch == 'y') {
                gameState_ = GameState_Enum::MainMenu;
            } else {
                gameState_ = GameState_Enum::Exit;
            }
        } catch (const std::exception& e) {
            std::cerr << "\n[ERROR SYSTEM] " << e.what() << "\n";
            std::cout << "Return to main menu? (y/n): ";
            char ch;
            if (std::cin >> ch && ch == 'y') {
                gameState_ = GameState_Enum::MainMenu;
            } else {
                gameState_ = GameState_Enum::Exit;
            }
        }
    }
    
    EventSystem::getInstance().logGameEnded();
    std::cout << "\nThanks for playing!\n";
}

void GameManager::showMainMenu() {
    reloadConfig("keybindings.cfg");
    
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "     WELCOME TO THE GAME!\n";
    std::cout << "====================================\n";
    std::cout << "\n1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "\nYour choice: ";
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }
    
    switch (choice) {
        case 1:
            startNewGame();
            gameState_ = GameState_Enum::Playing;
            break;
        case 2: {
            std::cout << "\nEnter savefile name (default 'savegame.sav'): ";
            std::string filename;
            std::cin.ignore();
            std::getline(std::cin, filename);
            if (filename.empty()) {
                filename = "savegame.sav";
            }
            try {
                loadGame(filename);
                gameState_ = GameState_Enum::Playing;
            } catch (const GameException& e) {
                std::cerr << "[ERROR] " << e.what() << "\n";
            }
            break;
        }
        case 3:
            gameState_ = GameState_Enum::Exit;
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
    }
}

void GameManager::startNewGame() {
    try {
        cleanup();
        
        currentLevel_ = 1;
        levelManager_ = new LevelManager(1);
        upgradeSystem_ = new UpgradeSystem();
        
        auto params = levelManager_->getCurrentLevelParams();
        field_ = new Field(params.fieldRows, params.fieldCols);
        
        player_ = new Player(1, 50, 5, 3, 0);
        player_->hand().addRandomSpell();
        allEntities_.push_back(player_);
        
        int r = params.fieldRows / 2;
        int c = params.fieldCols / 2;
        
        if (!field_->placePlayer(player_, r, c)) {
            throw InvalidGameStateException("Failed to place player");
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> rowDist(0, params.fieldRows - 1);
        std::uniform_int_distribution<int> colDist(0, params.fieldCols - 1);
        
        for (int i = 0; i < params.enemyCount; ++i) {
            int rr, cc;
            do {
                rr = rowDist(gen);
                cc = colDist(gen);
            } while ((rr == r && cc == c) || !field_->cellIsEmpty(rr, cc));
            
            Enemy* enemy = new Enemy(
                field_->generateEntityId(),
                params.enemyHealth,
                params.enemyDamage
            );
            allEntities_.push_back(enemy);
            field_->placeEnemy(enemy, rr, cc);
        }
        
        for (int i = 0; i < params.buildingCount; ++i) {
            int rr, cc;
            do {
                rr = rowDist(gen);
                cc = colDist(gen);
            } while ((rr == r && cc == c) || !field_->cellIsEmpty(rr, cc));
            
            Building* building = new Building(
                field_->generateEntityId(),
                params.buildingHealth,
                15
            );
            allEntities_.push_back(building);
            field_->placeBuilding(building, rr, cc);
        }
        
        std::cout << "\n[OK] New game started at level " << currentLevel_ << "!\n";
        EventSystem::getInstance().logGameStarted();
        std::cout << "Field size: " << params.fieldRows << "x" << params.fieldCols << "\n";
    } catch (const GameException&) {
        throw;
    } catch (const std::exception& e) {
        throw InvalidGameStateException(std::string("Error creating new game: ") + e.what());
    }
}

void GameManager::playLevel() {
    if (!field_ || !player_) {
        throw InvalidGameStateException("Field or player not initialized");
    }
    
    std::cout << "\n=== LEVEL " << currentLevel_ << " ===\n";
    std::cout << "Objective: Destroy all enemies and buildings\n\n";
    
    playerWon_ = false;
    
    while (true) {
        try {
            displayGameInfo();
            field_->print();
            
            handlePlayerTurn();
            if (!player_->isAlive()) {
                gameState_ = GameState_Enum::GameOver;
                break;
            }
            
            handleAllyTurns();
            if (!player_->isAlive()) {
                gameState_ = GameState_Enum::GameOver;
                break;
            }
            
            handleEnemyTurns();
            if (!player_->isAlive()) {
                gameState_ = GameState_Enum::GameOver;
                break;
            }
            
            handleBuildingTurns();
            if (!player_->isAlive()) {
                gameState_ = GameState_Enum::GameOver;
                break;
            }
            
            checkLevelConditions();
            if (playerWon_) {
                gameState_ = GameState_Enum::LevelComplete;
                break;
            }
        } catch (const GameException&) {
            throw;
        } catch (const std::exception& e) {
            throw InvalidGameStateException(std::string("Error during turn: ") + e.what());
        }
    }
}

void GameManager::handlePlayerTurn() {
    if (!field_ || !player_) return;
    
    std::cout << "\n--- YOUR TURN ---\n";
    // Build prompt from current bindings (show movement and cast keys)
    char moveUpKey = 'w', moveDownKey = 's', moveLeftKey = 'a', moveRightKey = 'd', castKey = 'c';
    for (const auto& p : keyBindings_) {
        if (p.second == CommandType::MoveUp) moveUpKey = p.first;
        else if (p.second == CommandType::MoveDown) moveDownKey = p.first;
        else if (p.second == CommandType::MoveLeft) moveLeftKey = p.first;
        else if (p.second == CommandType::MoveRight) moveRightKey = p.first;
        else if (p.second == CommandType::CastSpell) castKey = p.first;
    }

    std::cout << "(" << moveUpKey << "/" << moveLeftKey << "/" << moveDownKey << "/" << moveRightKey
              << ") move, (t) toggle mode, (" << castKey << ") spell, (m) menu\n";
    
    char cmd = getPlayerCommand();
    
    if (cmd == 'm') {
        std::cout << "\n1. Continue\n2. Save Game\n3. Main Menu\n";
        int choice;
        if (std::cin >> choice) {
            if (choice == 2) {
                std::cout << "Enter filename (default 'savegame.sav'): ";
                std::string filename;
                std::cin.ignore();
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "savegame.sav";
                try {
                    saveGame(filename);
                    std::cout << "[OK] Game saved\n";
                } catch (const GameException& e) {
                    std::cerr << "[ERROR] " << e.what() << "\n";
                }
            } else if (choice == 3) {
                std::cout << "Are you sure? (y/n): ";
                if (std::cin.get() == 'y') {
                    gameState_ = GameState_Enum::MainMenu;
                }
            }
        }
        return;
    }
    
    if (cmd == 't') {
        field_->togglePlayerCombatMode();
        return;
    }
    
    // 't' and 'm' are special keys (toggle / in-menu). Other actions come from keyBindings_
    auto itCmd = keyBindings_.find(cmd);
    if (cmd == 'c' && itCmd == keyBindings_.end()) {
        std::cout << "Select spell (index) and target (row, col): ";
        int idx, tr, tc;
        if (std::cin >> idx >> tr >> tc) {
            if (field_->playerCastSpell(idx, tr, tc)) {
                std::cout << "[OK] Spell applied\n";
            } else {
                std::cout << "[ERROR] Could not apply spell\n";
            }
        }
        return;
    }
    
    // If command came from bindings map, act accordingly
    if (itCmd != keyBindings_.end()) {
        CommandType type = itCmd->second;

        if (type == CommandType::CastSpell) {
            // Use same cast flow
            std::cout << "Select spell (index) and target (row, col): ";
            int idx, tr, tc;
            if (std::cin >> idx >> tr >> tc) {
                if (field_->playerCastSpell(idx, tr, tc)) {
                    std::cout << "[OK] Spell applied\n";
                } else {
                    std::cout << "[ERROR] Could not apply spell\n";
                }
            }
            return;
        }

        Direction dir = Direction::None;
        if (type == CommandType::MoveUp) dir = Direction::Up;
        else if (type == CommandType::MoveDown) dir = Direction::Down;
        else if (type == CommandType::MoveLeft) dir = Direction::Left;
        else if (type == CommandType::MoveRight) dir = Direction::Right;

        if (dir != Direction::None) {
            field_->movePlayer(dir);
        }

        // Additional actions supported: SaveGame, ShowInfo
        if (type == CommandType::SaveGame) {
            std::cout << "Enter filename (default 'savegame.sav'): ";
            std::string filename;
            std::cin.ignore();
            std::getline(std::cin, filename);
            if (filename.empty()) filename = "savegame.sav";
            try {
                saveGame(filename);
                std::cout << "[OK] Game saved\n";
            } catch (const GameException& e) {
                std::cerr << "[ERROR] " << e.what() << "\n";
            }
        }

        if (type == CommandType::ShowInfo) {
            displayGameInfo();
        }
    }
    
    field_->updateEntities();
}

void GameManager::handleAllyTurns() {
    if (!field_) return;
    std::cout << "\n--- ALLIES TURN ---\n";
    field_->updateEntities();
}

void GameManager::handleEnemyTurns() {
    if (!field_) return;
    std::cout << "\n--- ENEMIES TURN ---\n";
    field_->updateEntities();
}

void GameManager::handleBuildingTurns() {
    if (!field_) return;
    std::cout << "\n--- BUILDINGS TURN ---\n";
    field_->updateEntities();
}

void GameManager::checkLevelConditions() {
    if (!field_) return;
    playerWon_ = false;
}

void GameManager::handleLevelComplete() {
    EventSystem::getInstance().logLevelCompleted(currentLevel_);
    std::cout << "\n====================================\n";
    std::cout << "        LEVEL COMPLETE!\n";
    std::cout << "====================================\n";
    
    if (!player_) {
        gameState_ = GameState_Enum::MainMenu;
        return;
    }
    
    std::cout << "\nUpgrade System:\n";
    upgradeSystem_->showUpgradeMenu(player_);
    
    std::cout << "\nProceed to next level? (y/n): ";
    char ch;
    if (std::cin >> ch && ch == 'y') {
        levelManager_->nextLevel();
        currentLevel_++;
        
        try {
            auto params = levelManager_->getCurrentLevelParams();
            
            if (field_) delete field_;
            for (Entity* entity : allEntities_) {
                if (entity != player_) delete entity;
            }
            allEntities_.clear();
            allEntities_.push_back(player_);
            
            field_ = new Field(params.fieldRows, params.fieldCols);
            
            player_->setHealth(50 + (currentLevel_ - 1) * 15);
            
            int r = params.fieldRows / 2;
            int c = params.fieldCols / 2;
            field_->placePlayer(player_, r, c);
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> rowDist(0, params.fieldRows - 1);
            std::uniform_int_distribution<int> colDist(0, params.fieldCols - 1);
            
            for (int i = 0; i < params.enemyCount; ++i) {
                int rr, cc;
                do {
                    rr = rowDist(gen);
                    cc = colDist(gen);
                } while ((rr == r && cc == c) || !field_->cellIsEmpty(rr, cc));
                
                Enemy* enemy = new Enemy(
                    field_->generateEntityId(),
                    params.enemyHealth,
                    params.enemyDamage
                );
                allEntities_.push_back(enemy);
                field_->placeEnemy(enemy, rr, cc);
            }
            
            for (int i = 0; i < params.buildingCount; ++i) {
                int rr, cc;
                do {
                    rr = rowDist(gen);
                    cc = colDist(gen);
                } while ((rr == r && cc == c) || !field_->cellIsEmpty(rr, cc));
                
                Building* building = new Building(
                    field_->generateEntityId(),
                    params.buildingHealth,
                    15
                );
                allEntities_.push_back(building);
                field_->placeBuilding(building, rr, cc);
            }
            
            gameState_ = GameState_Enum::Playing;
        } catch (const GameException& e) {
            std::cerr << "[ERROR] " << e.what() << "\n";
            gameState_ = GameState_Enum::MainMenu;
        }
    } else {
        gameState_ = GameState_Enum::MainMenu;
    }
}

bool GameManager::showGameOver() {
    std::cout << "\n====================================\n";
    std::cout << "         GAME OVER\n";
    std::cout << "====================================\n";
    
    if (player_) {
        std::cout << "\nFinal Statistics:\n";
        std::cout << "Level: " << currentLevel_ << "\n";
        std::cout << "Score: " << player_->getScore() << "\n";
    }
    
    std::cout << "\n1. Restart\n2. Load Save\n3. Exit\n";
    std::cout << "Your choice: ";
    
    int choice;
    if (std::cin >> choice) {
        switch (choice) {
            case 1: {
                try {
                    startNewGame();
                    gameState_ = GameState_Enum::Playing;
                    return true;
                } catch (const GameException& e) {
                    std::cerr << "[ERROR] Failed to restart: " << e.what() << "\n";
                    gameState_ = GameState_Enum::MainMenu;
                    return true;
                }
            }
            case 2:
                gameState_ = GameState_Enum::MainMenu;
                return true;
            case 3:
                gameState_ = GameState_Enum::Exit;
                return true;
            default:
                gameState_ = GameState_Enum::MainMenu;
                return true;
        }
    }
    return false;
}

void GameManager::saveGame(const std::string& saveFile) {
    try {
        GameState state;
        serializeGameState(state);
        saveSystem_->saveToFile(saveFile, state);
        EventSystem::getInstance().logGameSaved(saveFile);
    } catch (const GameException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveGameException(saveFile, e.what());
    }
}

void GameManager::loadGame(const std::string& saveFile) {
    try {
        GameState state = saveSystem_->loadFromFile(saveFile);
        deserializeGameState(state);
        EventSystem::getInstance().logGameLoaded(saveFile);
    } catch (const GameException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadGameException(saveFile, e.what());
    }
}

void GameManager::serializeGameState(GameState& state) {
    if (!player_ || !field_) {
        throw InvalidGameStateException("Player or field not initialized");
    }
    
    state.currentLevel = currentLevel_;
    state.playerHealthMax = 50 + (currentLevel_ - 1) * 5;
    state.playerHealth = player_->getHealth();
    state.playerMeleeDamage = player_->getMeleeDamage();
    state.playerRangedDamage = player_->getRangedDamage();
    state.playerScore = player_->getScore();
    state.fieldRows = field_->rows();
    state.fieldCols = field_->cols();
    
    for (size_t i = 0; i < player_->hand().size(); ++i) {
        state.playerSpells.push_back(player_->hand().getSpellName(i));
    }
    
    const auto& entities = field_->getAllEntities();
    const auto& positions = field_->getAllPositions();
    
    for (const auto& pair : entities) {
        int id = pair.first;
        Entity* entity = pair.second;

        state.entityIds.push_back(id);
        state.entityHealths.push_back(entity->getHealth());
        state.entityTypes.push_back(static_cast<int>(entity->getType()));
        
        auto pos_it = positions.find(id);
        if (pos_it != positions.end()) {
            state.entityPositionsRow.push_back(pos_it->second.first);
            state.entityPositionsCols.push_back(pos_it->second.second);
        } else {
            state.entityPositionsRow.push_back(-1);
            state.entityPositionsCols.push_back(-1);
        }
    }
}

void GameManager::deserializeGameState(const GameState& state) {
    try {
        cleanup();
        
        currentLevel_ = state.currentLevel;
        levelManager_ = new LevelManager(currentLevel_);
        upgradeSystem_ = new UpgradeSystem();
        
        field_ = new Field(state.fieldRows, state.fieldCols);
        player_ = new Player(1, state.playerHealth, state.playerMeleeDamage, 
                           state.playerRangedDamage, state.playerScore);
        allEntities_.push_back(player_);
        
        for (size_t i = 0; i < state.entityIds.size(); ++i) {
            int id = state.entityIds[i];
            int health = state.entityHealths[i];
            EntityType type = static_cast<EntityType>(state.entityTypes[i]);
            int row = state.entityPositionsRow[i];
            int col = state.entityPositionsCols[i];
            
            if (row < 0 || col < 0) continue;
            
            if (type == EntityType::Player) {
                if (field_->placePlayer(player_, row, col)) {
                    player_->setHealth(health);
                }
            } else if (type == EntityType::Enemy) {
                Enemy* enemy = new Enemy(id, health, 5);
                allEntities_.push_back(enemy);
                field_->placeEnemy(enemy, row, col);
            } else if (type == EntityType::Building) {
                Building* building = new Building(id, health, 15);
                allEntities_.push_back(building);
                field_->placeBuilding(building, row, col);
            } else if (type == EntityType::Ally) {
                Ally* ally = new Ally(id, health, 3);
                allEntities_.push_back(ally);
                field_->placeAlly(ally, row, col);
            }
        }
        
        std::cout << "[OK] Game restored: Level " << currentLevel_ 
                  << ", HP: " << state.playerHealth << "\n";
    } catch (const std::exception& e) {
        throw InvalidGameStateException(std::string("Error restoring state: ") + e.what());
    }
}

char GameManager::getPlayerCommand() {
    char ch;
    if (std::cin >> ch) {
        return ch;
    }
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return ' ';
}

void GameManager::displayGameInfo() {
    if (!player_) return;
    
    std::cout << "\n--- GAME INFO ---\n";
    std::cout << "Level: " << currentLevel_ << "\n";
    std::cout << "HP: " << player_->getHealth() << "\n";
    std::cout << "Score: " << player_->getScore() << "\n";
}

void GameManager::reloadConfig(const std::string& configFile) {
    std::map<char, CommandType> bindings;
    if (KeyBindingsConfig::loadConfigFile(configFile, bindings)) {
        keyBindings_ = std::move(bindings);
        std::cout << "[OK] Key bindings reloaded from " << configFile << "\n";
    } else {
        // keep or reset to defaults if config invalid
        loadDefaultBindings();
        std::cout << "[INFO] Using default key bindings.\n";
    }
}

void GameManager::loadDefaultBindings() {
    keyBindings_.clear();
    keyBindings_['w'] = CommandType::MoveUp;
    keyBindings_['a'] = CommandType::MoveLeft;
    keyBindings_['s'] = CommandType::MoveDown;
    keyBindings_['d'] = CommandType::MoveRight;
    keyBindings_['e'] = CommandType::Attack;
    keyBindings_['c'] = CommandType::CastSpell;
    keyBindings_['q'] = CommandType::ShowInfo;
    keyBindings_['p'] = CommandType::SaveGame;
    keyBindings_['l'] = CommandType::LoadGame;
    keyBindings_['x'] = CommandType::Quit;
}
