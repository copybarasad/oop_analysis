#include "GameLevel.h"
#include "GameController.h" 

GameLevel::GameLevel(int width, int height) : field(width, height) {}

Field& GameLevel::getField() { return field; }
const Field& GameLevel::getField() const { return field; }

const std::vector<Enemy>& GameLevel::getEnemies() const { return enemies; }
const std::vector<EnemySpawner>& GameLevel::getSpawners() const { return spawners; }
const std::vector<Trap>& GameLevel::getTraps() const { return traps; }
const std::vector<Ally>& GameLevel::getAllies() const { return allies; }
const std::vector<EnemyTower>& GameLevel::getTowers() const { return towers; }

std::vector<Enemy>& GameLevel::getEnemiesEx() { return enemies; }
std::vector<EnemySpawner>& GameLevel::getSpawnersEx() { return spawners; }
std::vector<Trap>& GameLevel::getTrapsEx() { return traps; }
std::vector<Ally>& GameLevel::getAlliesEx() { return allies; }
std::vector<EnemyTower>& GameLevel::getTowersEx() { return towers; }

void GameLevel::clearEntities() {
    enemies.clear();
    spawners.clear();
    traps.clear();
    allies.clear();
    towers.clear();
}

bool GameLevel::isCleared() const {
    return enemies.empty() && spawners.empty() && towers.empty();
}

void GameLevel::update(Player& player, GameController& game, EventLogger* logger) {
    for (auto& spawner : spawners) spawner.processTurn(field, enemies);
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.processTurn(player, field, traps, logger);
        }
    }

    for (auto& ally : allies) {
        if (ally.isAlive()) {
            ally.processTurn(field, enemies);
        }
    }

    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.processTurn(player, game);
        }
    }
}

void GameLevel::cleanupDead(Player& player, EventLogger* logger) {
    auto remove_dead = [&](auto& list, CellType type, int score) {
        auto it = std::remove_if(list.begin(), list.end(), [&](const auto& e) {
            if (!e.isAlive()) {
                if (field.getCellType(e.getX(), e.getY()) == type) {
                    field.setCellType(e.getX(), e.getY(), CellType::EMPTY);
                }
                player.addScore(score);
                if (score > 0 && logger) {
                    logger->log(LogMsg::ENTITY_DESTROYED, score);
                }
                return true;
            }
            return false;
        });
        list.erase(it, list.end());
    };

    remove_dead(enemies, CellType::ENEMY, 25);
    remove_dead(allies, CellType::ALLY, 0);
    remove_dead(towers, CellType::ENEMY_TOWER, 100);
    remove_dead(spawners, CellType::SPAWNER, 150);
}

void GameLevel::damageAt(int x, int y, int damage) {
    if (!field.isValidPosition(x, y)) return;
    
    for (auto& e : enemies) {
        if (e.isAlive() && e.getX() == x && e.getY() == y) { e.takeDamage(damage); return; }
    }
    for (auto& t : towers) {
        if (t.isAlive() && t.getX() == x && t.getY() == y) { t.takeDamage(damage); return; }
    }
    for (auto& s : spawners) {
        if (s.isAlive() && s.getX() == x && s.getY() == y) { s.takeDamage(damage); return; }
    }
}

void GameLevel::placeTrap(int x, int y, int damage) {
    traps.emplace_back(x, y, damage);
    field.setCellType(x, y, CellType::TRAP);
}

bool GameLevel::spawnAllyNear(int x, int y) {
    int directions[8][2] = {{0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for (auto& dir : directions) {
        int sx = x + dir[0], sy = y + dir[1];
        if (field.isValidPosition(sx, sy) && field.getCellType(sx, sy) == CellType::EMPTY) {
            allies.emplace_back(sx, sy);
            field.setCellType(sx, sy, CellType::ALLY);
            return true;
        }
    }
    return false;
}