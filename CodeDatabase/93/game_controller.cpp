#include "game_controller.h"
#include "game_systems.h"
#include "game_initializer.h"
#include <iostream>
#include <cmath>

GameController::GameController(int fieldWidth, int fieldHeight, int enemyCount)
    : player(0, 0), field(fieldWidth, fieldHeight), gameRunning(true), currentLevel_(1) {
    if (enemyCount < 3 || enemyCount > 5) {
        std::cout << "Error: Enemy count must be between 3 and 5. Using default value 3.\n";
        enemyCount = 3;
    }
    GameInitializer::initializeEnemies(enemies, field, enemyCount);
    GameInitializer::initializeTowers(towers_, field, 1);
}

GameController::GameController()
    : player(0, 0), field(), gameRunning(true), currentLevel_(1) {}

bool GameController::movePlayer(int deltaX, int deltaY) {
    if (!gameRunning) return false;

    Position currentPos = player.getPosition();
    int newX = currentPos.x + deltaX;
    int newY = currentPos.y + deltaY;

    if (!field.isPositionPassable(newX, newY)) return false;
    if (field.isTowerAt(newX, newY, towers_)) return false;

    if (CombatSystem::performMeleeAttack(player, enemies, newX, newY)) return true;

    player.move(deltaX, deltaY);
    return true;
}

bool GameController::attackEnemy(int directionX, int directionY) {
    if (!gameRunning) return false;
    return CombatSystem::performRangedAttack(player, enemies, field, directionX, directionY);
}

void GameController::switchPlayerCombatMode() {
    if (gameRunning) player.switchCombatMode();
}

void GameController::updateGame() {
    if (!gameRunning) return;
    if (!player.isAlive()) { gameRunning = false; return; }

    for (auto& tower : towers_) {
        tower.tick();
        if (tower.canAttackThisTurn()) {
            Position tPos = tower.getPosition();
            Position pPos = player.getPosition();
            int dx = std::abs(tPos.x - pPos.x);
            int dy = std::abs(tPos.y - pPos.y);
            if (dx <= tower.getRange() && dy <= tower.getRange()) {
                tower.attackPlayer(player);
                tower.resetCooldown();
            }
        }
    }

    EnemyAI::processEnemyTurns(enemies, player, field, *this);
    field.removeInactiveTraps();

    if (GameStateChecker::areAllEnemiesDefeated(enemies)) gameRunning = false;
}

bool GameController::isGameRunning() const { return gameRunning; }
bool GameController::isGameWon() const { return GameStateChecker::isGameWon(enemies, gameRunning, player); }
const Player& GameController::getPlayer() const { return player; }
const std::vector<Enemy>& GameController::getEnemies() const { return enemies; }
const GameField& GameController::getField() const { return field; }
const std::vector<EnemyTower>& GameController::getTowers() const { return towers_; }
std::vector<EnemyTower>& GameController::getTowers() { return towers_; }

void GameController::addTrap(const Trap& trap) {
    field.addTrap(trap);
}

void GameController::addTower(const EnemyTower& tower) {
    towers_.push_back(tower);
}

bool GameController::usePlayerSpell(size_t index, int x, int y) {
    if (!gameRunning) return false;
    return player.getHand().useSpell(index, *this, x, y);
}

void GameController::save(std::ostream& os) const {
    field.save(os);
    player.save(os);
    os << enemies.size() << "\n";
    for (const auto& e : enemies) e.save(os);
    os << towers_.size() << "\n";
    for (const auto& t : towers_) t.save(os);
    os << gameRunning << "\n";
}

void GameController::load(std::istream& is) {
    field.load(is);
    player.load(is);
    currentLevel_ = player.getLevel();

    size_t ec; is >> ec;
    enemies.resize(ec);
    for (auto& e : enemies) e.load(is);

    size_t tc; is >> tc;
    towers_.resize(tc);
    for (auto& t : towers_) t.load(is);

    int r; is >> r;
    gameRunning = (r != 0);
}

void GameController::setPlayer(Player&& p) { player = std::move(p); }
void GameController::setEnemies(std::vector<Enemy>&& e) { enemies = std::move(e); }
void GameController::setTowers(std::vector<EnemyTower>&& t) { towers_ = std::move(t); }