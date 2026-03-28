#include "Game.h"
#include "GameState.h"
#include "SaveLoadManager.h"
#include "LevelManager.h"
#include "UpgradeSystem.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "Ally.h"
#include <iostream>
#include <stdexcept>
#include <random>
#include <windows.h>
#include <limits>

Game::Game() : status_(GameStatus::RUNNING), currentLevel_(1), isPaused_(false) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

Game::~Game() = default;

void Game::run() {
}

void Game::startNewGame() {
    state_ = LevelManager::createLevel(1);
    currentLevel_ = 1;
    status_ = GameStatus::RUNNING;
    
    GameEvent event;
    event.type = EventType::GAME_LOADED;
    event.actor = "System";
    event.details = "Новая игра начата, уровень " + std::to_string(currentLevel_);
    notifyEvent(event);
}

bool Game::loadGame(const std::string& filename) {
    try {
        auto loadedState = SaveLoadManager::loadGame(filename);
        if (!loadedState) {
            return false;
        }
        
        state_ = std::move(loadedState);
        currentLevel_ = state_->getLevel();
        status_ = GameStatus::RUNNING;
        
        GameEvent event;
        event.type = EventType::GAME_LOADED;
        event.actor = "System";
        event.details = "Игра загружена из " + filename;
        notifyEvent(event);
        
        return true;
    } 
    catch (const FileNotFoundException& e) {
        throw;
    }
    catch (const CorruptedSaveException& e) {
        throw;
    }
    catch (const std::exception& e) {
        throw;
    }
}

bool Game::saveGame(const std::string& filename) const {
    try {
        if (!state_) {
            return false;
        }
        
        SaveLoadManager::saveGame(*state_, filename);
        
        GameEvent event;
        event.type = EventType::GAME_SAVED;
        event.actor = "System";
        event.details = "Игра сохранена в " + filename;
        const_cast<Game*>(this)->notifyEvent(event);
        
        return true;
    } 
    catch (const std::exception& e) {
        return false;
    }
}

bool Game::movePlayer(int dx, int dy) {
    if (status_ != GameStatus::RUNNING) return false;
    if (!state_ || !state_->getField() || !state_->getPlayer()) return false;
    
    auto field = state_->getField();
    auto player = state_->getPlayer();
    
    if (player->isSlowed()) {
        player->setSlowed(false);
        return false;
    }
    
    bool actionPerformed = field->movePlayer(dx, dy);
    
    if (actionPerformed) {
        GameEvent event;
        event.type = EventType::PLAYER_MOVE;
        event.actor = "Player";
        event.x = dx;
        event.y = dy;
        notifyEvent(event);
        
        field->moveEnemies();
        checkWinConditions();
    }
    
    return actionPerformed;
}

bool Game::useSpell(int spellIndex, int targetX, int targetY) {
    if (status_ != GameStatus::RUNNING) return false;
    if (!state_ || !state_->getField() || !state_->getPlayer()) return false;
    
    auto player = state_->getPlayer();
    auto field = state_->getField();
    
    if (spellIndex < 0 || spellIndex >= player->getHand().size()) {
        return false;
    }
    
    bool success = player->getHand().useSpell(spellIndex, *field, *player, targetX, targetY);
    if (success) {
        GameEvent event;
        event.type = EventType::PLAYER_CAST_SPELL;
        event.actor = "Player";
        event.x = targetX;
        event.y = targetY;
        event.details = "Заклинание #" + std::to_string(spellIndex);
        notifyEvent(event);
        
        field->moveEnemies();
        checkWinConditions();
    }
    return success;
}

void Game::switchAttackMode() {
    if (status_ != GameStatus::RUNNING) return;
    if (!state_ || !state_->getPlayer()) return;
    
    auto player = state_->getPlayer();
    auto currentMode = player->getAttackMode();
    auto newMode = (currentMode == AttackMode::MELEE) ? AttackMode::RANGED : AttackMode::MELEE;
    player->setAttackMode(newMode);
}

void Game::endTurn() {
    if (status_ != GameStatus::RUNNING) return;
    if (!state_ || !state_->getField()) return;
    
    processAlliesTurn();
    processEnemiesTurn();
    processStructuresTurn();
    
    state_->incrementTurn();
    checkWinConditions();
}

void Game::advanceToNextLevel() {
    if (!state_ || !state_->getPlayer()) return;
    
    auto currentPlayer = state_->getPlayer();
    currentLevel_++;
    state_ = LevelManager::createLevel(currentLevel_, currentPlayer);
    status_ = GameStatus::RUNNING;
    
    GameEvent event;
    event.type = EventType::LEVEL_COMPLETE;
    event.actor = "Player";
    event.details = "Переход на уровень " + std::to_string(currentLevel_);
    notifyEvent(event);
}

void Game::applyUpgrade(const std::string& upgradeType) {
    if (!state_ || !state_->getPlayer()) return;
    
    UpgradeSystem::applyUpgrade(state_->getPlayer(), upgradeType);
    
    GameEvent event;
    event.type = EventType::UPGRADE_APPLIED;
    event.actor = "Player";
    event.details = "Улучшение: " + upgradeType;
    notifyEvent(event);
}

void Game::initializeLevel(int level) {
    state_ = LevelManager::createLevel(level);
}

void Game::processAlliesTurn() {
    if (!state_ || !state_->getField()) return;
    
    auto field = state_->getField();
    auto allies = field->findAllies();
    
    for (const auto& [x, y] : allies) {
        auto enemies = field->findEnemies();
        
        if (!enemies.empty()) {
            auto [ex, ey] = enemies[0];
            int dx = (ex > x) ? 1 : (ex < x) ? -1 : 0;
            int dy = (ey > y) ? 1 : (ey < y) ? -1 : 0;
            
            int nx = x + dx, ny = y + dy;
            if (field->inBounds(nx, ny) && field->getCell(nx, ny).isEmpty() && field->getCell(nx, ny).isPassable()) {
                auto ally = field->getCell(x, y).getAlly();
                field->getCell(x, y).setAlly(nullptr);
                field->getCell(nx, ny).setAlly(ally);
                
                auto& targetCell = field->getCell(nx, ny);
                if (auto enemy = targetCell.getEnemy()) {
                    if (enemy->isAlive()) {
                        enemy->takeDamage(ally->getDamage());
                        if (!enemy->isAlive()) {
                            targetCell.setEnemy(nullptr);
                        }
                    }
                }
            }
        }
    }
}

void Game::processEnemiesTurn() {
    if (!state_ || !state_->getField()) return;
    
    auto field = state_->getField();
    auto enemies = field->findEnemies();
    
    if (!enemies.empty()) {
        field->moveEnemies();
    }
}

void Game::processStructuresTurn() {
    if (!state_ || !state_->getField() || !state_->getPlayer()) return;
    
    auto field = state_->getField();
    auto player = state_->getPlayer();
    auto bases = field->findBases();
    
    if (!bases.empty()) {
        field->updateBases();
    }
    
    auto towers = field->findTowers();
    if (!towers.empty()) {
        for (const auto& [x, y] : towers) {
            auto tower = field->getCell(x, y).getTower();
            if (tower) {
                tower->tick();
                tower->attackPlayerIfInRange(*field, *player);
            }
        }
    }
}

void Game::checkWinConditions() {
    if (!state_ || !state_->getField() || !state_->getPlayer()) return;
    
    auto player = state_->getPlayer();
    auto field = state_->getField();
    
    if (!player->isAlive()) {
        status_ = GameStatus::PLAYER_LOST;
        return;
    }
    
    auto enemies = field->findEnemies();
    auto bases = field->findBases();
    auto towers = field->findTowers();
    
    bool playerExists = false;
    try {
        field->findPlayer();
        playerExists = true;
    } catch (const std::exception&) {
        playerExists = false;
    }
    
    if (playerExists && enemies.empty() && bases.empty() && towers.empty()) {
        status_ = GameStatus::LEVEL_COMPLETE;
        handleLevelCompletion();
    }
}

void Game::handleLevelCompletion() {
    if (!state_ || !state_->getPlayer()) return;
    
    auto player = state_->getPlayer();
    
    int levelReward = 100 + (currentLevel_ * 20);
    player->addPoints(levelReward);
    
    int oldHp = player->getHp();
    int healAmount = player->getMaxHp() - player->getHp();
    if (healAmount > 0) {
        player->heal(healAmount);
    }
    
    player->getHand().addRandomSpell();
    
    bool upgradeSelected = false;
    while (!upgradeSelected) {
        UpgradeSystem::displayUpgradeMenu(player);
        
        int choice;
        std::cin >> choice;
        
        auto upgrades = UpgradeSystem::getAvailableUpgrades(player);
        if (choice >= 1 && choice <= upgrades.size()) {
            UpgradeSystem::applyUpgrade(player, upgrades[choice - 1].name);
            upgradeSelected = true;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    if (player->getHand().size() > 0) {
        player->getHand().removeRandomSpell();
    }
    
    advanceToNextLevel();
}

const GameState* Game::getState() const { 
    return state_.get(); 
}

GameState* Game::getState() { 
    return state_.get(); 
}