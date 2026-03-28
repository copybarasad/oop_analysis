#include "GameBoard.hpp"
#include "defines.hpp"
#include "ally.hpp"
#include "trap.hpp"
#include "EnemyTower.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>
#include <vector>


void GameBoard::allocateMemory() {
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new Cell[width];
    }
}

void GameBoard::freeMemory() {
    if (grid) {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }

    for (auto p : enemies) delete p;
    enemies.clear();
    for (auto p : buildings) delete p;
    buildings.clear();
    for (auto p : towers) delete p;
    towers.clear();
    for (auto p : allies) delete p;
    allies.clear();
    for (auto p : traps) delete p;
    traps.clear();

}

void GameBoard::copyFrom(const GameBoard& other) {
    width = other.width;
    height = other.height;
    message_log = other.message_log;
    
    allocateMemory();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j] = other.grid[i][j];
        }
    }

    player = other.player; 
    enemies = other.enemies;
    buildings = other.buildings;
    towers = other.towers;
    allies = other.allies;
    traps = other.traps;
}

GameBoard::GameBoard(int w, int h) : width(w), height(h), grid(nullptr), player(nullptr) {
    if (width < 10) width = 10;
    if (width > 30) width = 30;
    if (height < 10) height = 10;
    if (height > 30) height = 30;
    
    allocateMemory();
    srand(static_cast<unsigned int>(time(0)));
}

GameBoard::~GameBoard() {
    freeMemory();
}

GameBoard::GameBoard(const GameBoard& other) : grid(nullptr) {
    copyFrom(other);
}

GameBoard& GameBoard::operator=(const GameBoard& other) {
    if (this != &other) {
        freeMemory();
        copyFrom(other);
    }
    return *this;
}

GameBoard::GameBoard(GameBoard&& other) noexcept
    : width(other.width), height(other.height), grid(other.grid), player(other.player), 
      enemies(std::move(other.enemies)), buildings(std::move(other.buildings)),
      towers(std::move(other.towers)), allies(std::move(other.allies)), traps(std::move(other.traps)),
      message_log(std::move(other.message_log)) {
    
    observers = std::move(other.observers);

    other.grid = nullptr; 
    other.width = 0;
    other.height = 0;
}

GameBoard& GameBoard::operator=(GameBoard&& other) noexcept {
    if (this != &other) {
        freeMemory();
        width = other.width;
        height = other.height;
        grid = other.grid;
        player = other.player;
        enemies = std::move(other.enemies);
        buildings = std::move(other.buildings);
        towers = std::move(other.towers);
        allies = std::move(other.allies);
        traps = std::move(other.traps);
        message_log = std::move(other.message_log);
        observers = std::move(other.observers);
        
        other.grid = nullptr;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameBoard::initializeBoard(double wall_chance, double slow_chance) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
                grid[i][j].setType(Cell::WALL);
                continue;
            }
            if ((i == 1 && j == 1) || (i == 1 && j == 2) || (i == 2 && j == 1)) {
                grid[i][j].setType(Cell::GROUND);
                continue; 
            }
            double r = static_cast<double>(rand()) / RAND_MAX;
            
            if (r < wall_chance) {
                grid[i][j].setType(Cell::WALL);
            } else if (r < wall_chance + slow_chance) {
                grid[i][j].setType(Cell::SLOWING);
            } else {
                grid[i][j].setType(Cell::GROUND);
            }
        }
    }
}

bool GameBoard::isDiagonalPathBlocked(int x1, int y1, int x2, int y2) {
    if (x1 == x2 || y1 == y2) return false; 
    Cell& cell1 = grid[y1][x2];
    Cell& cell2 = grid[y2][x1];
    if (!cell1.isPassable() && !cell2.isPassable()) {
        return true;
    }
    return false;
}

bool GameBoard::isGround(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].getType() == Cell::GROUND;
}

Entity* GameBoard::getEntityAt(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }
    return grid[y][x].getEntity();
}

void GameBoard::addPlayer(Player* p) { 
    if(p) { 
        player = p;
        grid[p->getY()][p->getX()].setEntity(p);
        grid[p->getY()][p->getX()].setType(Cell::GROUND);
    } 
}
void GameBoard::addEnemy(Enemy* e) { 
    if(e) { 
        enemies.push_back(e); 
        grid[e->getY()][e->getX()].setEntity(e); 
    } 
}
void GameBoard::addBuilding(EnemyBuilding* b) { 
    if(b) { 
        buildings.push_back(b);
        grid[b->getY()][b->getX()].setEntity(b); 
        grid[b->getY()][b->getX()].setType(Cell::WALL); 
    } 
}
void GameBoard::addTower(EnemyTower* t) { 
    if(t) { 
        towers.push_back(t); 
        grid[t->getY()][t->getX()].setEntity(t); 
        grid[t->getY()][t->getX()].setType(Cell::WALL); 
    } 
}
void GameBoard::addAlly(Ally* a) { 
    if(a) { 
        allies.push_back(a); 
        grid[a->getY()][a->getX()].setEntity(a); 
    } 
}
bool GameBoard::addTrap(Trap* t) {
    if (!t) return false;
    int x = t->getX(), y = t->getY();
    if (x >= 0 && x < width && y >= 0 && y < height && grid[y][x].isPassable() && grid[y][x].getType() == Cell::GROUND) {
        traps.push_back(t);
        grid[y][x].setEntity(t);
        return true;
    }
    delete t;
    return false;
}

bool GameBoard::movePlayer(int dx, int dy) {
    if (!player) return false;

    int oldX = player->getX();
    int oldY = player->getY();
    int newX = oldX + dx;
    int newY = oldY + dy;

    if (newX < 0 || newX >= width || newY < 0 || newY >= height || (dx == 0 && dy == 0)) {
        return false;
    }

    if (!grid[newY][newX].isPassable()) {
        addLogMessage("Path blocked!");
        return false;
    }
    
    grid[oldY][oldX].setEntity(nullptr);
    player->setPosition(newX, newY);
    grid[newY][newX].setEntity(player);

    std::string moveMsg = "Player moved to (" + std::to_string(newX) + ", " + std::to_string(newY) + ")";
    notify(EventType::MOVE, moveMsg);

    if (grid[newY][newX].getType() == Cell::SLOWING) {
        player->setSlowed(true);
        addLogMessage("You stepped on a slowing trap! You miss the next turn.");
        notify(EventType::INFO, "Player entered slowing trap");
    }
    
    return true;
}

bool GameBoard::playerAttack() {
    if (!player) return false;

    Entity* target = nullptr;
    double min_distance = std::numeric_limits<double>::max();

    auto find_closest = [&](auto& container) {
        for (auto* entity : container) {
            int dx = player->getX() - entity->getX();
            int dy = player->getY() - entity->getY();
            double distance = std::sqrt(dx * dx + dy * dy);
            if (distance < min_distance) {
                min_distance = distance;
                target = entity;
            }
        }
    };

    find_closest(enemies);
    find_closest(buildings);
    find_closest(towers);

    if (!target) {
        addLogMessage("There are no targets left.");
        return false;
    }
    if (min_distance > player->getAttackRange()) {
        addLogMessage("No targets in range.");
        return false;
    }
    if (isDiagonalPathBlocked(player->getX(), player->getY(), target->getX(), target->getY())) {
        addLogMessage("Target is blocked by walls.");
        return false;
    }
    
    int dmg = player->getDamage();
    target->takeDamage(dmg); 

    std::string targetType = "Target";
    EntityType type = target->getEntityType();
    if (type == EntityType::ENEMY) targetType = "Enemy";
    else if (type == EntityType::BUILDING) targetType = "Building";
    else if (type == EntityType::TOWER) targetType = "Tower";

    addLogMessage("You attacked " + targetType + " for " + std::to_string(dmg) + " dmg.");
    notify(EventType::DAMAGE, "Player dealt " + std::to_string(dmg) + " damage to " + targetType);

    if (target->getHealth() != -1 && target->getHealth() <= 0) {
        addLogMessage(targetType + " defeated!");
        notify(EventType::DEATH, targetType + " eliminated by Player");
        
        target->onDeath(player);          
        target->removeFromBoard(*this); 
    } else if (target->getHealth() > 0) {
        addLogMessage(targetType + " HP: " + std::to_string(target->getHealth()));
    }
    return true;
}

bool GameBoard::playerCastSpell(int spell_index, int targetX, int targetY) {
    if (!player || !player->getHand()) return false;
    Spell* spell = player->getHand()->getSpell(spell_index);
    if (!spell) return false;

    if (spell->cast(*this, *player, targetX, targetY)) {
        player->getHand()->useSpell(spell_index);
        return true;
    }
    return false;
}

void GameBoard::moveEnemies() {
    for (size_t i = 0; i < enemies.size(); ) {
        Enemy* enemy = enemies[i];

        int dx = player->getX() - enemy->getX();
        int dy = player->getY() - enemy->getY();
        
        if (abs(dx) + abs(dy) == 1) {
            enemyAttack(enemy);
            i++;
            continue;
        }

        int currentX = enemy->getX();
        int currentY = enemy->getY();
        int nextX = currentX;
        int nextY = currentY;
        bool moved = false;

        if (abs(dx) >= abs(dy)) {
            int stepX = (dx > 0) ? 1 : -1;
            if (grid[currentY][currentX + stepX].isPassable()) { nextX += stepX; moved = true; } 
            else if (dy != 0) {
                int stepY = (dy > 0) ? 1 : -1;
                if (grid[currentY + stepY][currentX].isPassable()) { nextY += stepY; moved = true; }
            }
        } else {
            int stepY = (dy > 0) ? 1 : -1;
            if (grid[currentY + stepY][currentX].isPassable()) { nextY += stepY; moved = true; }
            else if (dx != 0) {
                int stepX = (dx > 0) ? 1 : -1;
                if (grid[currentY][currentX + stepX].isPassable()) { nextX += stepX; moved = true; }
            }
        }

        if (moved) {
            Entity* entity_on_cell = getEntityAt(nextX, nextY);
            if (entity_on_cell && entity_on_cell->getEntityType() == EntityType::ENEMY) {
                i++;
                continue;
            }

            if (entity_on_cell) {
                entity_on_cell->onSteppedOn(enemy);
                if (entity_on_cell->getEntityType() == EntityType::TRAP) {
                    addLogMessage("Enemy steps on a trap!");
                    notify(EventType::DAMAGE, "Enemy hit by Trap");
                    entity_on_cell->removeFromBoard(*this);
                }

                if (enemy->getHealth() <= 0) {
                    addLogMessage("Enemy died from a trap!");
                    notify(EventType::DEATH, "Enemy died from Trap");
                    enemy->onDeath(player);
                    enemy->removeFromBoard(*this);
                    continue; 
                }
            }
            
            if (grid[nextY][nextX].isPassable()) {
                grid[currentY][currentX].setEntity(nullptr);
                enemy->setPosition(nextX, nextY);
                grid[nextY][nextX].setEntity(enemy);
            }
        }
        i++;
    }
}

void GameBoard::updateBuildings() {
    std::vector<Enemy*> spawned_enemies;
    for (auto* building : buildings) {
        if (building->shouldSpawn()) {
            building->resetSpawnTimer();
            bool spawned = false;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int newX = building->getX() + dx, newY = building->getY() + dy;
                    if (newX >= 0 && newX < width && newY >= 0 && newY < height && grid[newY][newX].isPassable()) {
                        spawned_enemies.push_back(new Enemy(newX, newY));
                        spawned = true;
                        break;
                    }
                }
                if (spawned) break;
            }
        }
    }
    if (!spawned_enemies.empty()) {
        addLogMessage("New enemies have spawned!");
        notify(EventType::INFO, "Enemy Building spawned " + std::to_string(spawned_enemies.size()) + " enemies");
        for (auto* e : spawned_enemies) {
            addEnemy(e);
        }
    }
}

void GameBoard::updateTowers() {
    for (auto* tower : towers) {
        tower->tickCooldown();
        if (tower->canAttack()) {
            double distance = std::sqrt(pow(player->getX() - tower->getX(), 2.0) + pow(player->getY() - tower->getY(), 2.0));
            if (distance <= tower->getRange()) {
                addLogMessage("An enemy tower shoots at you!");
                notify(EventType::DAMAGE, "Tower shot Player for " + std::to_string(tower->getDamage()));
                player->takeDamage(tower->getDamage());
                addLogMessage("Player health: " + std::to_string(player->getHealth()));
                tower->resetCooldown();
            }
        }
    }
}

void GameBoard::moveAllies() {
    for (size_t i = 0; i < allies.size(); ) {
        Ally* ally = allies[i];
        Entity* target = nullptr;
        double min_distance = std::numeric_limits<double>::max();
        
        auto find_closest_enemy = [&](auto& container) {
            for (auto* entity : container) {
                EntityType type = entity->getEntityType();
                if (type == EntityType::ENEMY || type == EntityType::BUILDING || type == EntityType::TOWER) {
                    double dist = std::hypot(ally->getX() - entity->getX(), ally->getY() - entity->getY());
                    if (dist < min_distance) {
                        min_distance = dist;
                        target = entity;
                    }
                }
            }
        };
        find_closest_enemy(enemies);
        find_closest_enemy(buildings);
        find_closest_enemy(towers);

        if (target) {
            if (min_distance < 1.5) {
                addLogMessage("Ally attacks an enemy!");
                target->takeDamage(ally->getDamage());
                notify(EventType::DAMAGE, "Ally dealt " + std::to_string(ally->getDamage()) + " damage");

                if (target->getHealth() != -1 && target->getHealth() <= 0) {
                    addLogMessage("Ally defeated an enemy!");
                    notify(EventType::DEATH, "Enemy defeated by Ally");
                    target->onDeath(player);
                    target->removeFromBoard(*this);
                }
            } else {
                int dx = target->getX() - ally->getX();
                int dy = target->getY() - ally->getY();
                int moveX = 0, moveY = 0;
                if (abs(dx) > abs(dy)) { moveX = (dx > 0) ? 1 : -1; }
                else if (dy != 0) { moveY = (dy > 0) ? 1 : -1; }
                else if (dx != 0) { moveX = (dx > 0) ? 1 : -1; }
                int newX = ally->getX() + moveX;
                int newY = ally->getY() + moveY;
                if (newX >= 0 && newX < width && newY >= 0 && newY < height && grid[newY][newX].isPassable()) {
                    grid[ally->getY()][ally->getX()].setEntity(nullptr);
                    ally->setPosition(newX, newY);
                    grid[newY][newX].setEntity(ally);
                }
            }
        }
        i++;
    }
}

void GameBoard::enemyAttack(Enemy* enemy) {
    if (player) {
        player->takeDamage(enemy->getDamage());
        addLogMessage("Enemy attacks Player! HP: " + std::to_string(player->getHealth()));
        notify(EventType::DAMAGE, "Player took " + std::to_string(enemy->getDamage()) + " damage from Enemy");
    }
}

void GameBoard::damageArea(int cx, int cy, int size, int damage) {
    int startX = std::max(0, cx - (size-1) / 2);
    int startY = std::max(0, cy - (size-1) / 2);
    int endX = std::min(width - 1, cx + size / 2);
    int endY = std::min(height - 1, cy + size / 2);

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Entity* target = getEntityAt(x, y);
            if (!target) continue;
            
            EntityType type = target->getEntityType();
            if (type == EntityType::PLAYER || type == EntityType::ALLY) continue;

            notify(EventType::DAMAGE, "Area spell hit entity at " + std::to_string(x) + "," + std::to_string(y));

            target->takeDamage(damage);
            
            if (target->getHealth() <= 0) {
                switch(type) {
                    case EntityType::ENEMY:
                        notify(EventType::DEATH, "Enemy killed by Area Spell");
                        removeEnemy(static_cast<Enemy*>(target));
                        break;
                    case EntityType::BUILDING:
                        notify(EventType::DEATH, "Building destroyed by Area Spell");
                        removeBuilding(static_cast<EnemyBuilding*>(target));
                        break;
                    case EntityType::TOWER:
                        notify(EventType::DEATH, "Tower destroyed by Area Spell");
                        removeTower(static_cast<EnemyTower*>(target));
                        break;
                    default: break;
                }
            }
        }
    }
}

void GameBoard::addLogMessage(const std::string& message) {
    if(message_log.size() >= 5) {
        message_log.erase(message_log.begin());
    }
    message_log.push_back(message);
    notify(EventType::INFO, message);
}

void GameBoard::clearLog() {
    message_log.clear();
}

void GameBoard::removeEnemy(Enemy* enemy) { if(enemy) removeEntityFromVector(enemies, enemy); }
void GameBoard::removeAlly(Ally* ally) { if(ally) removeEntityFromVector(allies, ally); }
void GameBoard::removeTrap(Trap* trap) { if(trap) removeEntityFromVector(traps, trap); }

void GameBoard::removeBuilding(EnemyBuilding* building) {
    if (!building) return;
    int x = building->getX(), y = building->getY();
    removeEntityFromVector(buildings, building);
    grid[y][x].setType(Cell::GROUND);
}

void GameBoard::removeTower(EnemyTower* tower) {
    if (!tower) return;
    int x = tower->getX(), y = tower->getY();
    removeEntityFromVector(towers, tower);
    grid[y][x].setType(Cell::GROUND);
}