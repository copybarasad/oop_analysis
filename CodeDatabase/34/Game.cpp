#include "Game.h"
#include "Exceptions.h"
#include "SpellHand.h"
#include "spells/DirectDamageSpell.h"
#include "spells/AreaDamageSpell.h"
#include "spells/TrapSpell.h"
#include "spells/SummonSpell.h"
#include "spells/EnhancementSpell.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>

namespace Game {

Game::Game(int level, Player& playerRef, int scoreToWin) 
    : levelNumber(level), scoreToWin(scoreToWin), player(playerRef), currentTurn(0), resultState(LevelResult::InProgress) {
    initializeNewLevel(level);
}

Game::Game(std::istream& is, Player& playerRef) 
    : player(playerRef), resultState(LevelResult::InProgress) {
    deserialize(is);
}

void Game::initializeNewLevel(int level) {
    currentTurn = 0;
    
    enemies.clear();
    allies.clear();
    traps.clear();
    enemyBuildings.clear();
    enemyTowers.clear();
    
    int mapSize = 10 + level * 2;
    if (mapSize > 25) mapSize = 25;
    gameMap = std::make_unique<Map>(mapSize, mapSize);

    placePlayerRandomly();

    int enemyCount = 5 + level * 2;
    int enemyHealth = 50 + level * 10;
    int enemyDamage = 5 + level * 2;
    placeEnemiesRandomly(enemyCount, enemyHealth, enemyDamage);
    
    int buildingSpawnInterval = 5 - level;
    if (buildingSpawnInterval < 2) buildingSpawnInterval = 2;
    placeEnemyBuildingsRandomly(2, buildingSpawnInterval);
    
    placeEnemyTowersRandomly(1 + level);
}

void Game::addObserver(IObserver* observer) { observers.push_back(observer); }
void Game::removeObserver(IObserver* observer) { observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end()); }
void Game::notify(const Event& event) {
    for (auto* observer : observers) {
        observer->onNotify(event);
    }
}

void Game::addLogMessage(const std::string& message) {
    turnLogs.push_back(message);
}

const std::vector<std::string>& Game::getTurnLogs() const { 
    return turnLogs;
}

void Game::clearTurnLogs() {
    turnLogs.clear();
}

void Game::runTurn(ICommand& command) {
    clearTurnLogs();
    command.execute(*this);
    if (!hasEnded()) {
        processAITurns();
    }
}

void Game::movePlayer(int dr, int dc) {
    player.resetTurnState();
    Utils::Point currentPos = player.getPosition();
    Utils::Point targetPos = {currentPos.row + dr, currentPos.column + dc};
    
    if (!player.canMoveThisTurn()) {
        notify({EventType::GenericMessage, std::string("You are slowed and cannot move!")});
        return;
    } 
    if (!gameMap->isValidPosition(targetPos)) {
        notify({EventType::GenericMessage, std::string("You cannot move outside the map.")});
        return;
    } 
    if (!gameMap->getCell(targetPos).isPassable() || getBuildingAtPosition(targetPos) || getTowerAtPosition(targetPos)) {
        notify({EventType::GenericMessage, std::string("The path is blocked.")});
        return;
    } 
    
    if (Enemy* enemy = getEnemyAtPosition(targetPos)) {
        handlePlayerCombat(targetPos);
    } else {
        player.move(targetPos);
        notify({EventType::PlayerMoved, PositionData{currentPos, targetPos}});
    }

    if (gameMap->getCell(player.getPosition()).isSlowing()) {
        player.setSlowedForNextTurn(true);
    } else {
        player.setSlowedForNextTurn(false);
    }
}

void Game::playerSwitchCombatMode() {
    player.switchCombatMode();
    notify({EventType::GenericMessage, std::string("Combat mode switched to " ) + (player.getCombatMode() == CombatMode::Melee ? "Melee." : "Ranged.")});
}

void Game::playerCastSpell(int spellIndex, Utils::Point target) {
    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
        notify({EventType::GenericMessage, std::string("Invalid spell choice.")});
        return;
    }

    ISpell* spellToCheck = player.getSpellHand().getSpell(spellIndex);
    if (!spellToCheck) return;

    if (spellToCheck->requiresTarget() && !gameMap->isValidPosition(target)) {
        notify({EventType::GenericMessage, std::string("Invalid target coordinates.")});
        return;
    }
    
    std::unique_ptr<ISpell> spellToCast = player.getSpellHand().useSpell(spellIndex);
    if (!spellToCast) return;
    
    notify({EventType::PlayerCastedSpell, SpellCastData{spellToCast->getName(), target}});

    int stacks = player.getEnhancementStacks();
    bool success = spellToCast->cast(*this, target, stacks);

    if (success) {
        if (dynamic_cast<EnhancementSpell*>(spellToCast.get()) == nullptr) {
            player.resetEnhancementStacks();
        }
    } else {
        notify({EventType::GenericMessage, std::string("Failed to cast spell. Card returned to hand.")});
        player.getSpellHand().addSpell(std::move(spellToCast));
    }
}

void Game::processAITurns() {
    currentTurn++;

    processAllyTurns();
    processEnemyTurns();
    processEnemyTowerTurns();
    processEnemyBuildingTurns();
    processTraps();
    cleanupDefeated();

    if (!player.isAlive()) {
        notify({EventType::LevelLost, std::monostate{}});
        resultState = LevelResult::Loss;
    } else if (player.getScore() >= scoreToWin) {
        notify({EventType::LevelWon, levelNumber});
        resultState = LevelResult::Win;
    } else {
        notify({EventType::TurnStarted, currentTurn});
    }
}

void Game::setResult(LevelResult result) {
    resultState = result;
}

int Game::getLevelNumber() const { return levelNumber; }
int Game::getScoreToWin() const { return scoreToWin; }
int Game::getCurrentTurn() const { return currentTurn; }
bool Game::hasEnded() const { return resultState != LevelResult::InProgress; }
LevelResult Game::getResult() const { return resultState; }
const Map& Game::getMap() const { return *gameMap; }
const Player& Game::getPlayer() const { return player; }
const std::vector<std::unique_ptr<Enemy>>& Game::getEnemies() const { return enemies; }
const std::vector<std::unique_ptr<EnemyBuilding>>& Game::getEnemyBuildings() const { return enemyBuildings; }
const std::vector<std::unique_ptr<EnemyTower>>& Game::getEnemyTowers() const { return enemyTowers; }
const std::vector<std::unique_ptr<Ally>>& Game::getAllies() const { return allies; }
const std::vector<std::unique_ptr<Trap>>& Game::getTraps() const { return traps; }
Player& Game::getPlayer() { return player; }

void Game::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&levelNumber), sizeof(levelNumber));
    os.write(reinterpret_cast<const char*>(&scoreToWin), sizeof(scoreToWin));
    os.write(reinterpret_cast<const char*>(&currentTurn), sizeof(currentTurn));
    
    gameMap->serialize(os);

    serializeVector(os, enemies);
    serializeVector(os, allies);
    serializeVector(os, traps);
    serializeVector(os, enemyBuildings);
    serializeVector(os, enemyTowers);
}

void Game::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&levelNumber), sizeof(levelNumber));
    is.read(reinterpret_cast<char*>(&scoreToWin), sizeof(scoreToWin));
    is.read(reinterpret_cast<char*>(&currentTurn), sizeof(currentTurn));
    if (is.fail()) throw FileReadException("Failed to read level data.");

    gameMap = std::make_unique<Map>(is);

    deserializeVector(is, enemies);
    deserializeVector(is, allies);
    deserializeVector(is, traps);
    deserializeVector(is, enemyBuildings);
    deserializeVector(is, enemyTowers);
}

template<typename T>
void Game::serializeVector(std::ostream& os, const std::vector<std::unique_ptr<T>>& vec) const {
    size_t size = vec.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& item : vec) {
        item->serialize(os);
    }
}

template<typename T>
void Game::deserializeVector(std::istream& is, std::vector<std::unique_ptr<T>>& vec) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if(is.fail()) throw FileReadException("Failed to read vector size for " + std::string(typeid(T).name()));
    if(size > 1000) throw InvalidDataException("Unrealistic entity count in save file for " + std::string(typeid(T).name()));
    
    vec.clear();
    for (size_t i = 0; i < size; ++i) {
        auto item = std::make_unique<T>(is);
        vec.push_back(std::move(item));
    }
}

void Game::placePlayerRandomly() {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    Utils::Point pos;
    bool placed = false;
    while (!placed) {
        pos.row = rng.getRandomInt(0, gameMap->getHeight() - 1);
        pos.column = rng.getRandomInt(0, gameMap->getWidth() - 1);
        if (gameMap->getCell(pos).isPassable() && !isCellOccupied(pos, false)) {
            player.move(pos);
            placed = true;
        }
    }
}

void Game::placeEnemiesRandomly(int count, int health, int damage) {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    for (int i = 0; i < count; ++i) {
        Utils::Point pos;
        bool placed = false;
        while (!placed) {
            pos.row = rng.getRandomInt(0, gameMap->getHeight() - 1);
            pos.column = rng.getRandomInt(0, gameMap->getWidth() - 1);
            if (gameMap->getCell(pos).isPassable() && !isCellOccupied(pos, true)) {
                enemies.push_back(std::make_unique<Enemy>(pos, health, damage));
                placed = true;
            }
        }
    }
}

void Game::placeEnemyBuildingsRandomly(int count, int spawnInterval) {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    for (int i = 0; i < count; ++i) {
        Utils::Point pos;
        bool placed = false;
        while (!placed) {
            pos.row = rng.getRandomInt(0, gameMap->getHeight() - 1);
            pos.column = rng.getRandomInt(0, gameMap->getWidth() - 1);
            if (gameMap->getCell(pos).isPassable() && !isCellOccupied(pos, true)) {
                enemyBuildings.push_back(std::make_unique<EnemyBuilding>(pos, spawnInterval));
                placed = true;
            }
        }
    }
}

void Game::placeEnemyTowersRandomly(int count) {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    for (int i = 0; i < count; ++i) {
        Utils::Point pos;
        bool placed = false;
        while (!placed) {
            pos.row = rng.getRandomInt(0, gameMap->getHeight() - 1);
            pos.column = rng.getRandomInt(0, gameMap->getWidth() - 1);
            if (gameMap->getCell(pos).isPassable() && !isCellOccupied(pos, true)) {
                enemyTowers.push_back(std::make_unique<EnemyTower>(pos));
                placed = true;
            }
        }
    }
}

void Game::processAllyTurns() {
    if (allies.empty()) return;
    for (const auto& allyPtr : allies) {
        if (!allyPtr->isAlive()) continue;
        Enemy* closestEnemy = nullptr;
        int minDistance = 1000;
        for (const auto& enemyPtr : enemies) {
            if (!enemyPtr->isAlive()) continue;
            int dist = std::abs(allyPtr->getPosition().row - enemyPtr->getPosition().row) + std::abs(allyPtr->getPosition().column - enemyPtr->getPosition().column);
            if (dist < minDistance) {
                minDistance = dist;
                closestEnemy = enemyPtr.get();
            }
        }
        if (closestEnemy) {
            if (minDistance <= 1) {
                closestEnemy->takeDamage(allyPtr->getDamage());
                notify({EventType::AllyAttacked, DamageData{"Ally", "Enemy", allyPtr->getDamage(), closestEnemy->getPosition()}});
            } else {
                Utils::Point dir = getNearestTargetDirection(allyPtr->getPosition(), closestEnemy->getPosition());
                Utils::Point nextPos(allyPtr->getPosition().row + dir.row, allyPtr->getPosition().column + dir.column);
                if (gameMap->isValidPosition(nextPos) && gameMap->getCell(nextPos).isPassable() && !isCellOccupied(nextPos)) {
                    allyPtr->move(nextPos);
                }
            }
        }
    }
}

void Game::processEnemyTurns() {
    if(enemies.empty()) return;
    for (const auto& enemyPtr : enemies) {
        if (!enemyPtr->isAlive()) continue;
        if (isAdjacent(enemyPtr->getPosition(), player.getPosition())) {
            player.takeDamage(enemyPtr->getDamage());
            notify({EventType::PlayerTookDamage, DamageData{"Enemy", "Player", enemyPtr->getDamage(), player.getPosition()}});
        } else {
            Utils::Point dir = getNearestTargetDirection(enemyPtr->getPosition(), player.getPosition());
            Utils::Point nextPos(enemyPtr->getPosition().row + dir.row, enemyPtr->getPosition().column + dir.column);
            if (gameMap->isValidPosition(nextPos) && gameMap->getCell(nextPos).isPassable() && !isCellOccupied(nextPos, true)) {
                Utils::Point oldPos = enemyPtr->getPosition();
                enemyPtr->move(nextPos);
                notify({EventType::EnemyMoved, PositionData{oldPos, nextPos}});
            }
        }
    }
}

void Game::processEnemyBuildingTurns() {
    if(enemyBuildings.empty()) return;
    for (const auto& building : enemyBuildings) {
        if(building->isAlive() && building->shouldSpawnEnemyThisTurn()) {
            spawnEnemy(building->getPosition());
            building->resetSpawnTimer();
        }
    }
}

void Game::processEnemyTowerTurns() {
    if (enemyTowers.empty()) return;
    for (auto& tower : enemyTowers) {
        if (!tower->isAlive()) continue;
        tower->tickCooldown();
        if (tower->canAttack()) {
            tower->attack(*this);
        }
    }
}

void Game::processTraps() {
    if(traps.empty()) return;
    for (auto& trap : traps) {
        if (trap->isTriggered()) continue;
        for (auto& enemy : enemies) {
            if (enemy->isAlive() && enemy->getPosition() == trap->getPosition()) {
                enemy->takeDamage(trap->getDamage());
                trap->trigger();
                notify({EventType::TrapTriggered, DamageData{"Trap", "Enemy", trap->getDamage(), enemy->getPosition()}});
                if(!enemy->isAlive()) {
                    player.onEnemyKilled();
                    notify({EventType::EnemyDefeated, enemy->getPosition()});
                }
                break;
            }
        }
    }
}

void Game::handlePlayerCombat(const Utils::Point& targetPosition) {
    Enemy* enemy = getEnemyAtPosition(targetPosition);
    if (!enemy) return;
    int damage = player.getCurrentDamage();
    enemy->takeDamage(damage);
    notify({EventType::PlayerAttacked, DamageData{"Player", "Enemy", damage, targetPosition}});
    if (!enemy->isAlive()) {
        player.addScore(100);
        player.onEnemyKilled();
        notify({EventType::EnemyDefeated, targetPosition});
        
        if (player.shouldReceiveNewSpell()) {
            std::unique_ptr<ISpell> newSpell = SpellHand::createRandomSpell();
            notify({EventType::PlayerGainedSpell, newSpell->getName()});
            if (!player.getSpellHand().addSpell(std::move(newSpell))) {
                notify({EventType::GenericMessage, std::string("But your hand is full!")});
            }
            player.resetKillTracker();
        }
    }
}

Utils::Point Game::getNearestTargetDirection(const Utils::Point& from, const Utils::Point& to) const {
    Utils::Point dir(0, 0);
    int dRow = to.row - from.row;
    int dCol = to.column - from.column;
    if (dRow != 0) dir.row = (dRow > 0) ? 1 : -1;
    if (dCol != 0) dir.column = (dCol > 0) ? 1 : -1;
    if (dir.row != 0 && dir.column != 0) {
        if (Utils::RandomGenerator::getInstance().getRandomBool()) dir.column = 0;
        else dir.row = 0;
    }
    return dir;
}

void Game::cleanupDefeated() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e){ return !e->isAlive(); }), enemies.end());
    allies.erase(std::remove_if(allies.begin(), allies.end(), [](const auto& a){ return !a->isAlive(); }), allies.end());
    enemyTowers.erase(std::remove_if(enemyTowers.begin(), enemyTowers.end(), [](const auto& t){ return !t->isAlive(); }), enemyTowers.end());
    enemyBuildings.erase(std::remove_if(enemyBuildings.begin(), enemyBuildings.end(), [](const auto& b){ return !b->isAlive(); }), enemyBuildings.end());
    traps.erase(std::remove_if(traps.begin(), traps.end(), [](const auto& t){ return t->isTriggered(); }), traps.end());
}

bool Game::isAdjacent(const Utils::Point& p1, const Utils::Point& p2) const {
    int dr = std::abs(p1.row - p2.row);
    int dc = std::abs(p1.column - p2.column);
    return (dr == 0 && dc == 1) || (dr == 1 && dc == 0);
}

void Game::spawnEnemy(const Utils::Point& buildingPosition) {
    std::vector<Utils::Point> freeAdjacent = getFreeAdjacentPositions(buildingPosition);
    if (!freeAdjacent.empty()) {
        Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
        Utils::Point spawnPos = freeAdjacent[rng.getRandomInt(0, freeAdjacent.size() - 1)];
        int enemyHealth = 50 + levelNumber * 10;
        int enemyDamage = 5 + levelNumber * 2;
        enemies.push_back(std::make_unique<Enemy>(spawnPos, enemyHealth, enemyDamage));
        notify({EventType::EnemySpawned, spawnPos});
    }
}

void Game::addTrap(std::unique_ptr<Trap> trap) { 
    notify({EventType::TrapPlaced, trap->getPosition()});
    traps.push_back(std::move(trap)); 
}
void Game::addAlly(std::unique_ptr<Ally> ally) { 
    notify({EventType::AllySpawned, ally->getPosition()});
    allies.push_back(std::move(ally)); 
}

bool Game::isCellOccupied(const Utils::Point& position, bool checkPlayerAndAllies) const {
    if (checkPlayerAndAllies) {
        if (player.getPosition() == position) return true;
        if (getAllyAtPosition(position)) return true;
    }
    if (getEnemyAtPosition(position)) return true;
    if (getBuildingAtPosition(position)) return true;
    if (getTowerAtPosition(position)) return true;
    return false;
}

std::vector<Utils::Point> Game::getFreeAdjacentPositions(const Utils::Point& position) const {
    std::vector<Utils::Point> freePositions;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            if (std::abs(dr) + std::abs(dc) > 1) continue;
            Utils::Point checkPos(position.row + dr, position.column + dc);
            if (gameMap->isValidPosition(checkPos) && gameMap->getCell(checkPos).isPassable() && !isCellOccupied(checkPos, true)) {
                freePositions.push_back(checkPos);
            }
        }
    }
    return freePositions;
}

Enemy* Game::getEnemyAtPosition(const Utils::Point& position) {
    for (const auto& ptr : enemies) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
const Enemy* Game::getEnemyAtPosition(const Utils::Point& position) const {
    for (const auto& ptr : enemies) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
EnemyBuilding* Game::getBuildingAtPosition(const Utils::Point& position) {
    for (const auto& ptr : enemyBuildings) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
const EnemyBuilding* Game::getBuildingAtPosition(const Utils::Point& position) const {
    for (const auto& ptr : enemyBuildings) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
EnemyTower* Game::getTowerAtPosition(const Utils::Point& position) {
    for (const auto& ptr : enemyTowers) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
const EnemyTower* Game::getTowerAtPosition(const Utils::Point& position) const {
    for (const auto& ptr : enemyTowers) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
Ally* Game::getAllyAtPosition(const Utils::Point& position) {
    for (const auto& ptr : allies) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
const Ally* Game::getAllyAtPosition(const Utils::Point& position) const {
    for (const auto& ptr : allies) if (ptr->isAlive() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
Trap* Game::getTrapAtPosition(const Utils::Point& position) {
    for (const auto& ptr : traps) if (!ptr->isTriggered() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}
const Trap* Game::getTrapAtPosition(const Utils::Point& position) const {
    for (const auto& ptr : traps) if (!ptr->isTriggered() && ptr->getPosition() == position) return ptr.get();
    return nullptr;
}

}