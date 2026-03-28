#include "include/GameCycle.h"
#include "include/Manager.h"
#include "include/EntityManager.h"
#include "include/MovementManager.h"
#include "include/CombatManager.h"
#include "include/Field.h"
#include "include/Balance.h"
#include "include/GameException.h"
#include "../entities/include/Player.h"
#include "../entities/include/Enemy.h"
#include "../entities/include/EnemyBuilding.h"
#include "../entities/include/EnemyTower.h"
#include "../items/include/Hand.h"
#include <cassert>
#include <iostream>
#include <fstream>

GameCycle::GameCycle()
{
    initialRows_ = Balance::fieldRows(1);
    initialCols_ = Balance::fieldCols(1);
    initLevel(initialRows_, initialCols_);
}

void GameCycle::setDimensions(int rows, int cols) {
    initialRows_ = rows;
    initialCols_ = cols;
}

void GameCycle::startGame() {
    state_ = GameState::Running;
    level_ = 1;
    initLevel(initialRows_, initialCols_);
}

bool GameCycle::step(const PlayerCommand& cmd) {
    if (state_ != GameState::Running) return true;
    
    if (isWin()) {
        state_ = GameState::WaitingForUpgrade;
        return true;
    }
    if (isLose()) {
        state_ = GameState::GameOver;
        return false;
    }

    playerTurn_(cmd);
    
    if (isWin()) {
        state_ = GameState::WaitingForUpgrade;
        return true;
    }

    alliesTurn_();
    enemiesTurn_();
    buildingsTurn_();

    if (isWin()) {
        state_ = GameState::WaitingForUpgrade;
        return true;
    }

    if (isLose()) {
        state_ = GameState::GameOver;
        return false;
    }
    return true;
}

void GameCycle::upgradeHealth() {
    if (playerId_ == InvalidEntityId) return;
    auto* p = dynamic_cast<Player*>(manager_->entity(playerId_));
    if (p) {
        p->upgradeMaxHp(20); // +20 ОЗ
    }
}

void GameCycle::upgradeDamage() {
    if (playerId_ == InvalidEntityId) return;
    auto* p = dynamic_cast<Player*>(manager_->entity(playerId_));
    if (p) {
        p->upgradeDamage(5); // +5 Урона
    }
}

void GameCycle::proceedToNextLevel() {
    nextLevel();
    state_ = GameState::Running;
}

bool GameCycle::isWin() const {
    const auto& em = manager_->entities();
    bool enemiesCleared = em.listByType(EntityType::Enemy).empty()
        && em.listByType(EntityType::EnemyTower).empty()
        && em.listByType(EntityType::EnemyBuilding).empty();
    
    return enemiesCleared && manager_->entities().findPlayerId() != InvalidEntityId;
}

bool GameCycle::isLose() const {
    EntityId pid = manager_->entities().findPlayerId();
    if (pid == InvalidEntityId) return true;
    auto* p = manager_->entity(pid);
    return !p || !p->isAlive();
}

std::optional<Position> GameCycle::playerPos() const {
    if (playerId_ == InvalidEntityId) return std::nullopt;
    return manager_->entities().positionOf(playerId_);
}

void GameCycle::initLevel(int rows, int cols, std::unique_ptr<Player> player) {
    manager_ = std::make_unique<Manager>(Field(rows, cols));
    
    Position start{rows/2, cols/2};
    manager_->field().cellAt(start).setPassable(true);

    if (player) {
        playerId_ = manager_->entities().spawnAt(std::move(player), start);
    } else {
        playerId_ = manager_->entities().spawnAt(std::make_unique<Player>(Balance::PlayerBaseHP, Balance::PlayerBaseDmg), start);
    }

    spawnEntities<Enemy>(Balance::EnemyCount(level_), Balance::EnemyHP(level_), Balance::EnemyDmg(level_));

    spawnEntities<EnemyBuilding>(Balance::BuildingCount(level_), 
            Balance::BuildingBaseHP, 
            Balance::BuildingSpawnInterval(level_),
            Balance::EnemyHP(level_),
            Balance::EnemyDmg(level_));

    spawnEntities<EnemyTower>(Balance::TowerCount(level_), 
            Balance::TowerHP(level_), 
            Balance::TowerDmg(level_), 
            Balance::TowerRange, 
            Balance::TowerCooldown);
}

void GameCycle::nextLevel() {
    std::unique_ptr<Hand> savedHand;
    int nextMaxHp = Balance::PlayerBaseHP; 
    int currentDmg = Balance::PlayerBaseDmg;
    
    if (playerId_ != InvalidEntityId) {
        auto* p = dynamic_cast<Player*>(manager_->entity(playerId_));
        if (p) {
            nextMaxHp = p->maxHp();
            currentDmg = p->baseDamage();
            savedHand = p->takeHand();
            if (savedHand) savedHand->removeRandomSpells(0.5);
        }
    }

    level_++;

    int growth = (level_ - 1) * 2;
    int newRows = initialRows_ + growth;
    int newCols = initialCols_ + growth;
    if (newRows > 25) newRows = 25;
    if (newCols > 25) newCols = 25;

    manager_ = std::make_unique<Manager>(Field(newRows, newCols));

    auto newPlayer = std::make_unique<Player>(nextMaxHp, currentDmg);
    if (savedHand) {
        newPlayer->setHand(std::move(savedHand));
    }

    initLevel(newRows, newCols, std::move(newPlayer));
}

void GameCycle::playerTurn_(const PlayerCommand& cmd) {
    if (playerId_ == InvalidEntityId) return;
    auto* ent = manager_->entity(playerId_);
    if (!ent) return;
    auto* player = static_cast<Player*>(ent);

    switch (cmd.action) {
        case PlayerAction::Move: {
            if (cmd.direction != Direction::None) {
                manager_->movement().tryMove(playerId_, cmd.direction);
            }
            break;
        }
        case PlayerAction::SwitchMode: {
            manager_->entities().switchPlayerMode(playerId_, cmd.mode);
            break;
        }
        case PlayerAction::Attack: {
            if (cmd.target.has_value()) {
                auto pPos = manager_->entities().positionOf(playerId_);
                if (pPos) {
                    int dist = std::abs(pPos->row - cmd.target->row) + std::abs(pPos->col - cmd.target->col);
                    int maxRange = (player->attackMode() == AttackMode::Melee) ? Balance::PlayerMeleeRange : Balance::PlayerRangedRange;
                    
                    if (dist > maxRange) {
                        throw GameException("Target out of range!");
                    }

                    auto targetId = manager_->occupantAt(*cmd.target);
                    if (!targetId) {
                        throw GameException("No target at specified position!");
                    }
                    
                    int damage = player->damage();
                    if (player->attackMode() == AttackMode::Ranged) {
                        damage = static_cast<int>(damage * 0.8);
                    }

                    manager_->combat().damageAndResolve(playerId_, *targetId, damage);
                }
            }
            break;
        }
        case PlayerAction::UseSpell: {
            if (cmd.spellIndex.has_value() && cmd.target.has_value()) {
                int idx = *cmd.spellIndex;
                if (idx >= 0 && idx < static_cast<int>(player->hand().slots().size())) {
                    player->hand().select(static_cast<size_t>(idx));
                    if (!player->hand().useCurrent(*manager_, playerId_, *cmd.target)) {
                        throw GameException("Failed to cast spell! Check range, target or charges.");
                    }
                } else {
                     throw GameException("Invalid spell index!");
                }
            }
            break;
        }
        case PlayerAction::Skip: {
            break;
        }
    }
}

void GameCycle::alliesTurn_() {
    manager_->alliesTurn();
}

void GameCycle::enemiesTurn_() {
    manager_->enemyTurn();
}

void GameCycle::buildingsTurn_() {
    manager_->buildingsTurn();
}

void GameCycle::saveGame(const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw GameSaveException("Cannot open file for writing: " + filename);
    }
    
    ofs << level_ << " " << playerId_ << "\n";
    manager_->save(ofs);
    
    if (ofs.fail()) {
        throw GameSaveException("Error occurred while writing to file");
    }
}

void GameCycle::loadGame(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw GameSaveException("Cannot open file for reading: " + filename);
    }
    
    if (ifs.peek() == std::ifstream::traits_type::eof()) {
        throw GameSaveException("File is empty: " + filename);
    }

    int lvl;
    EntityId pid;
    if (!(ifs >> lvl >> pid)) {
        throw GameSaveException("Failed to read game header (level/playerId). File might be corrupted.");
    }
    
    if (lvl < 1) {
        throw GameSaveException("Invalid level number in save file.");
    }

    try {
        manager_->load(ifs);
    } catch (const std::exception& e) {
        throw GameSaveException(std::string("Error loading game data: ") + e.what());
    }
    
    level_ = lvl;
    playerId_ = pid;
    state_ = GameState::Running;

    int growth = (level_ - 1) * 2;
    int calcRows = manager_->rows() - growth;
    int calcCols = manager_->cols() - growth;
    
    initialRows_ = (calcRows < 10) ? 10 : calcRows;
    initialCols_ = (calcCols < 10) ? 10 : calcCols;
}
