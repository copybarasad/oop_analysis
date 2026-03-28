#include "GameController.h"
#include <random>
#include <algorithm>
#include <cmath>

GameController::GameController(int fieldWidth, int fieldHeight, int handSize)
    : field(fieldWidth, fieldHeight), player(0, 0), playerHand(handSize), 
      gameOver(false), gameWon(false) {

    spawnInitialEnemies();
    spawnInitialBuildings();
    spawnInitialTowers();

    field.setPlayerPosition(player.get_Position());
}

void GameController::spawnInitialEnemies() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(3, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(3, field.getHeight() - 1);

    for (int i = 0; i < 3; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (!field.isCellEmpty(x, y));

        enemies.emplace_back(x, y);
        field.setEnemyPosition(Position(x, y));
    }
}

void GameController::spawnInitialBuildings() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(5, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(5, field.getHeight() - 1);

    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (!field.isCellEmpty(x, y));

        buildings.emplace_back(x, y);
        field.setBuildingPosition(Position(x, y));
    }
}

void GameController::spawnInitialTowers() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(4, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(4, field.getHeight() - 1);

    for (int i = 0; i < 1; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (!field.isCellEmpty(x, y));

        towers.emplace_back(x, y);
        field.setTowerPosition(Position(x, y));
    }
}

bool GameController::movePlayerUp() {
    return tryMovePlayer(0, -1);
}

bool GameController::movePlayerDown() {
    return tryMovePlayer(0, 1);
}

bool GameController::movePlayerLeft() {
    return tryMovePlayer(-1, 0);
}

bool GameController::movePlayerRight() {
    return tryMovePlayer(1, 0);
}

bool GameController::tryMovePlayer(int dx, int dy) {
    if (gameOver) {
        return false;
    }
    if (player.get_IsSlowed()) {
        player.remove_Slow();
        lastMessage = "Эффект замедления снят! Ход пропущен.";
        return true;
    }

    Position currentPos = player.get_Position();
    Position newPos(currentPos.x + dx, currentPos.y + dy);

    if (!field.isCellPassable(newPos.x, newPos.y)) {
        lastMessage = "Невозможно пройти!";
        return false;
    }

    if (!field.isEnemyHere(newPos.x, newPos.y)) {
        field.clearPlayerPosition(currentPos);
        player.set_Position(newPos);
        field.setPlayerPosition(newPos);
    }

    if (field.getCellType(newPos.x, newPos.y) == CellType::SLOWERING) {
        player.apply_Slow();
        lastMessage = "Вы попали в замедляющую зону!";
    } else {
        lastMessage = "";
    }

    checkPlayerEnemyCollision();
    return true;
}

void GameController::switchPlayerCombatMode() {
    if (!gameOver) {
        player.switch_CombatMode();
        lastMessage = "Режим боя переключён!";
    }
}

bool GameController::attackAtPosition(const Position& target) {
    if (gameOver) return false;

    Position playerPos = player.get_Position();
    
    int range = (player.get_CombatMode() == CombatMode::MELEE) ? 1 : 3;
    
    int dx = std::abs(target.x - playerPos.x);
    int dy = std::abs(target.y - playerPos.y);
    
    if (dx > range || dy > range) {
        lastMessage = "Цель вне радиуса атаки!";
        return false;
    }

    int damage = player.get_Damage();
    
    if (damageEnemyAt(target, damage)) {
        lastMessage = "Враг атакован!";
        return true;
    }
    
    if (damageBuildingAt(target, damage)) {
        lastMessage = "Здание атаковано!";
        return true;
    }
    
    if (damageTowerAt(target, damage)) {
        lastMessage = "Башня атакована!";
        return true;
    }

    lastMessage = "Нет цели для атаки!";
    return false;
}

SpellResult GameController::castSelectedSpell(const Position& target) {
    if (gameOver) {
        return SpellResult(false, false, "Игра окончена!");
    }

    Spell* spell = playerHand.getSelectedSpell();
    if (!spell) {
        return SpellResult(false, false, "Нет выбранного заклинания!");
    }

    EnhancementContext& enhancement = playerHand.getEnhancement();
    SpellResult result = spell->cast(*this, target, enhancement);

    if (result.success && !spell->isEnhancement()) {
        playerHand.resetEnhancement();
    }

    if (result.success) {
        playerHand.removeSpell(playerHand.getSelectedIndex());
        playerHand.checkAndRewardSpell();
    }

    lastMessage = result.message;
    return result;
}

void GameController::selectNextSpell() {
    playerHand.selectNext();
}

void GameController::selectPreviousSpell() {
    playerHand.selectPrevious();
}

bool GameController::selectSpell(int index) {
    return playerHand.selectSpell(index);
}

void GameController::updateGame() {
    if (gameOver) {
        return;
    }

    updateBuildings();
    updateTowers();

    moveEnemies();
    moveAllies();
    
    checkTraps();
    checkAllyEnemyCollision();
    removeDeadEntities();

    if (buildings.empty() && enemies.empty() && towers.empty()) {
        gameWon = true;
        gameOver = true;
        lastMessage = "Победа! Все враги уничтожены!";
    }
}

void GameController::updateBuildings() {
    for (auto& building : buildings) {
        if (!building.isAlive()) continue;
        
        building.update();
        if (building.should_SpawnEnemy()) {
            Position spawnPos = findSpawnPositionNearBuilding(building);
            if (spawnPos.x != -1 && spawnPos.y != -1) {
                enemies.emplace_back(spawnPos.x, spawnPos.y);
                field.setEnemyPosition(spawnPos);
            }
            building.reset_Counter();
        }
    }
}

void GameController::updateTowers() {
    Position playerPos = player.get_Position();
    
    for (auto& tower : towers) {
        if (!tower.isAlive()) continue;
        
        tower.updateCooldown();
        
        if (tower.canAttack() && tower.isInRange(playerPos)) {
            player.take_Damage(tower.get_Damage());
            tower.performAttack();
            lastMessage = "Башня атаковала вас! (-" + std::to_string(tower.get_Damage()) + " HP)";
            
            if (!player.is_Alive()) {
                gameOver = true;
                lastMessage = "Вы погибли от атаки башни!";
            }
        }
    }
}

void GameController::checkTraps() {
    for (auto& enemy : enemies) {
        Position pos = enemy.get_Position();
        if (field.hasTrap(pos.x, pos.y)) {
            int trapDamage = field.getTrapDamage(pos.x, pos.y);
            enemy.take_Damage(trapDamage);
            field.clearTrap(pos);
            
            traps.erase(
                std::remove_if(traps.begin(), traps.end(),
                    [&pos](const Trap& t) { return t.get_Position() == pos; }),
                traps.end()
            );
        }
    }
}

Position GameController::findSpawnPositionNearBuilding(const EnemyBuilding& building) {
    Position buildingPos = building.get_Position();
    std::vector<Position> possiblePositions = {
        {buildingPos.x - 1, buildingPos.y},
        {buildingPos.x + 1, buildingPos.y},
        {buildingPos.x, buildingPos.y - 1},
        {buildingPos.x, buildingPos.y + 1}
    };

    for (const auto& pos : possiblePositions) {
        if (field.isCellEmpty(pos.x, pos.y)) {
            return pos;
        }
    }

    return Position(-1, -1);
}

void GameController::moveEnemies() {
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        Position currentPos = enemy.get_Position();
        Position playerPos = player.get_Position();

        int dx = 0, dy = 0;
        if (currentPos.x < playerPos.x && field.isCellPassable(currentPos.x + 1, currentPos.y)) {
            dx = 1;
        }
        else if (currentPos.x > playerPos.x && field.isCellPassable(currentPos.x - 1, currentPos.y)) {
            dx = -1;
        }
        else if (currentPos.y < playerPos.y && field.isCellPassable(currentPos.x, currentPos.y + 1)) {
            dy = 1;
        }
        else if (currentPos.y > playerPos.y && field.isCellPassable(currentPos.x, currentPos.y - 1)) {
            dy = -1;
        }

        Position newPos(currentPos.x + dx, currentPos.y + dy);

        if (newPos == player.get_Position()) {
            player.take_Damage(enemy.get_Damage());
            if (!player.is_Alive()) {
                gameOver = true;
                lastMessage = "Вы погибли!";
            }
        }
        else if (field.isCellPassable(newPos.x, newPos.y) && !field.isEnemyHere(newPos.x, newPos.y)) {
            field.clearEnemyPosition(currentPos);
            enemy.set_Position(newPos);
            field.setEnemyPosition(newPos);
        }
    }
}

void GameController::moveAllies() {
    if (enemies.empty()) return;

    for (auto& ally : allies) {
        if (!ally.isAlive()) continue;

        Position currentPos = ally.get_Position();
        
        Position nearestEnemy = enemies[0].get_Position();
        int minDist = std::abs(nearestEnemy.x - currentPos.x) + std::abs(nearestEnemy.y - currentPos.y);
        
        for (const auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            Position enemyPos = enemy.get_Position();
            int dist = std::abs(enemyPos.x - currentPos.x) + std::abs(enemyPos.y - currentPos.y);
            if (dist < minDist) {
                minDist = dist;
                nearestEnemy = enemyPos;
            }
        }

        int dx = 0, dy = 0;
        if (currentPos.x < nearestEnemy.x && field.isCellPassable(currentPos.x + 1, currentPos.y)) {
            dx = 1;
        }
        else if (currentPos.x > nearestEnemy.x && field.isCellPassable(currentPos.x - 1, currentPos.y)) {
            dx = -1;
        }
        else if (currentPos.y < nearestEnemy.y && field.isCellPassable(currentPos.x, currentPos.y + 1)) {
            dy = 1;
        }
        else if (currentPos.y > nearestEnemy.y && field.isCellPassable(currentPos.x, currentPos.y - 1)) {
            dy = -1;
        }

        Position newPos(currentPos.x + dx, currentPos.y + dy);

        if (newPos != currentPos) {
            field.clearAllyPosition(currentPos);
            ally.set_Position(newPos);
            field.setAllyPosition(newPos);
        }
    }
}

void GameController::checkPlayerEnemyCollision() {
    for (auto& enemy : enemies) {
        if (enemy.get_Position() == player.get_Position()) {
            player.take_Damage(enemy.get_Damage());
            if (!player.is_Alive()) {
                gameOver = true;
                lastMessage = "Вы погибли!";
            }
            break;
        }
    }
}

void GameController::checkAllyEnemyCollision() {
    for (auto& ally : allies) {
        if (!ally.isAlive()) continue;
        
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            
            if (ally.get_Position() == enemy.get_Position()) {
                enemy.take_Damage(ally.get_Damage());
                ally.take_Damage(enemy.get_Damage());
            }
        }
    }
}

void GameController::removeDeadEntities() {
    int killsBefore = static_cast<int>(enemies.size());
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](Enemy& e) {
                if (!e.isAlive()) {
                    field.clearEnemyPosition(e.get_Position());
                    player.add_Score(10);
                    playerHand.registerKill();
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
    int killsAfter = static_cast<int>(enemies.size());
    int kills = killsBefore - killsAfter;
    if (kills > 0) {
        playerHand.checkAndRewardSpell();
    }

    buildings.erase(
        std::remove_if(buildings.begin(), buildings.end(),
            [this](EnemyBuilding& b) {
                if (!b.isAlive()) {
                    field.clearBuildingPosition(b.get_Position());
                    player.add_Score(25);
                    return true;
                }
                return false;
            }),
        buildings.end()
    );

    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [this](EnemyTower& t) {
                if (!t.isAlive()) {
                    field.clearTowerPosition(t.get_Position());
                    player.add_Score(20);
                    return true;
                }
                return false;
            }),
        towers.end()
    );

    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [this](Ally& a) {
                if (!a.isAlive()) {
                    field.clearAllyPosition(a.get_Position());
                    return true;
                }
                return false;
            }),
        allies.end()
    );
}

bool GameController::damageEnemyAt(const Position& pos, int damage) {
    for (auto& enemy : enemies) {
        if (enemy.get_Position() == pos && enemy.isAlive()) {
            enemy.take_Damage(damage);
            return true;
        }
    }
    return false;
}

bool GameController::damageBuildingAt(const Position& pos, int damage) {
    for (auto& building : buildings) {
        if (building.get_Position() == pos && building.isAlive()) {
            building.take_Damage(damage);
            return true;
        }
    }
    return false;
}

bool GameController::damageTowerAt(const Position& pos, int damage) {
    for (auto& tower : towers) {
        if (tower.get_Position() == pos && tower.isAlive()) {
            tower.take_Damage(damage);
            return true;
        }
    }
    return false;
}

bool GameController::canPlaceTrap(const Position& pos) const {
    if (!field.isValidPosition(pos.x, pos.y)) return false;
    if (!field.isCellPassable(pos.x, pos.y)) return false;
    if (field.hasTrap(pos.x, pos.y)) return false;
    if (field.isEnemyHere(pos.x, pos.y)) return false;
    if (pos == player.get_Position()) return false;
    return true;
}

void GameController::placeTrap(const Position& pos, int damage) {
    traps.emplace_back(pos.x, pos.y, damage);
    field.setTrap(pos, damage);
}

int GameController::summonAllies(int count) {
    Position playerPos = player.get_Position();
    std::vector<Position> possiblePositions = {
        {playerPos.x - 1, playerPos.y},
        {playerPos.x + 1, playerPos.y},
        {playerPos.x, playerPos.y - 1},
        {playerPos.x, playerPos.y + 1},
        {playerPos.x - 1, playerPos.y - 1},
        {playerPos.x + 1, playerPos.y - 1},
        {playerPos.x - 1, playerPos.y + 1},
        {playerPos.x + 1, playerPos.y + 1}
    };

    int summoned = 0;
    for (const auto& pos : possiblePositions) {
        if (summoned >= count) break;
        
        if (field.isCellEmpty(pos.x, pos.y) && field.isCellPassable(pos.x, pos.y)) {
            allies.emplace_back(pos.x, pos.y);
            field.setAllyPosition(pos);
            summoned++;
        }
    }
    return summoned;
}

bool GameController::isGameOver() const {
    return gameOver;
}

bool GameController::isGameWon() const {
    return gameWon;
}

const Player& GameController::getPlayer() const {
    return player;
}

Player& GameController::getPlayerMutable() {
    return player;
}

const GameField& GameController::getField() const {
    return field;
}

const std::vector<Enemy>& GameController::getEnemies() const {
    return enemies;
}

std::vector<Enemy>& GameController::getEnemiesMutable() {
    return enemies;
}

const std::vector<EnemyBuilding>& GameController::getBuildings() const {
    return buildings;
}

std::vector<EnemyBuilding>& GameController::getBuildingsMutable() {
    return buildings;
}

const std::vector<EnemyTower>& GameController::getTowers() const {
    return towers;
}

std::vector<EnemyTower>& GameController::getTowersMutable() {
    return towers;
}

const std::vector<Ally>& GameController::getAllies() const {
    return allies;
}

const std::vector<Trap>& GameController::getTraps() const {
    return traps;
}

const PlayerHand& GameController::getPlayerHand() const {
    return playerHand;
}

PlayerHand& GameController::getPlayerHandMutable() {
    return playerHand;
}

const std::string& GameController::getLastMessage() const {
    return lastMessage;
}

void GameController::setLastMessage(const std::string& msg) {
    lastMessage = msg;
}

void GameController::clearAllEntities() {
    for (auto& enemy : enemies) {
        field.clearEnemyPosition(enemy.get_Position());
    }
    enemies.clear();
    
    for (auto& building : buildings) {
        field.clearBuildingPosition(building.get_Position());
    }
    buildings.clear();
    
    for (auto& tower : towers) {
        field.clearTowerPosition(tower.get_Position());
    }
    towers.clear();
    
    for (auto& ally : allies) {
        field.clearAllyPosition(ally.get_Position());
    }
    allies.clear();
    
    for (auto& trap : traps) {
        field.clearTrap(trap.get_Position());
    }
    traps.clear();
}

void GameController::spawnEnemiesWithStats(int count, int health, int damage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(3, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(3, field.getHeight() - 1);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = disX(gen);
            y = disY(gen);
            attempts++;
        } while (!field.isCellEmpty(x, y) && attempts < 100);

        if (attempts < 100) {
            enemies.emplace_back(x, y, health, damage);
            field.setEnemyPosition(Position(x, y));
        }
    }
}

void GameController::spawnBuildingsWithStats(int count, int health) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(5, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(5, field.getHeight() - 1);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = disX(gen);
            y = disY(gen);
            attempts++;
        } while (!field.isCellEmpty(x, y) && attempts < 100);

        if (attempts < 100) {
            buildings.emplace_back(x, y, 5, health);
            field.setBuildingPosition(Position(x, y));
        }
    }
}

void GameController::spawnTowersWithStats(int count, int health, int damage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(4, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(4, field.getHeight() - 1);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = disX(gen);
            y = disY(gen);
            attempts++;
        } while (!field.isCellEmpty(x, y) && attempts < 100);

        if (attempts < 100) {
            towers.emplace_back(x, y, health, damage);
            field.setTowerPosition(Position(x, y));
        }
    }
}

void GameController::restorePlayerHealth() {
    player.setHealth(player.get_MaxHealth());
}

void GameController::setPlayerStats(int maxHealth, int meleeDamage, int rangedDamage) {
    player.setMaxHealth(maxHealth);
    player.setMeleeDamage(meleeDamage);
    player.setRangedDamage(rangedDamage);
}

GameField& GameController::getFieldMutable() {
    return field;
}
