#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "EventSystem.h"
#include <random>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iostream>

bool Field::inBounds(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

Field::Field(int width, int height)
    : width_(std::clamp(width, 10, 25)),
      height_(std::clamp(height, 10, 25)),
      grid_(height_, std::vector<Cell>(width_, Cell(CellType::EMPTY))) {}

Field::Field(const Field& other) = default;
Field::Field(Field&& other) noexcept = default;
Field& Field::operator=(const Field& other) = default;
Field& Field::operator=(Field&& other) noexcept = default;

int Field::getWidth() const { return width_; }
int Field::getHeight() const { return height_; }

const Cell& Field::getCell(int x, int y) const {
    if (!inBounds(x, y)) throw std::out_of_range("Cell out of bounds");
    return grid_[y][x];
}

Cell& Field::getCell(int x, int y) {
    if (!inBounds(x, y)) throw std::out_of_range("Cell out of bounds");
    return grid_[y][x];
}

std::pair<int, int> Field::findPlayer() const {
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getPlayer())
                return {x, y};
    throw std::runtime_error("Player not found");
}

bool Field::isPlayerOnField() const {
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getPlayer())
                return true;
    return false;
}

std::vector<std::pair<int, int>> Field::findEnemies() const {
    std::vector<std::pair<int, int>> res;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getEnemy() && grid_[y][x].getEnemy()->isAlive())
                res.emplace_back(x, y);
    return res;
}

std::vector<std::pair<int, int>> Field::findBases() const {
    std::vector<std::pair<int, int>> res;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getBase() && grid_[y][x].getBase()->isAlive())
                res.emplace_back(x, y);
    return res;
}

std::vector<std::pair<int, int>> Field::findTowers() const {
    std::vector<std::pair<int, int>> res;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getTower() && grid_[y][x].getTower()->isAlive())
                res.emplace_back(x, y);
    return res;
}

std::vector<std::pair<int, int>> Field::findAllies() const {
    std::vector<std::pair<int, int>> res;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (grid_[y][x].getAlly() && grid_[y][x].getAlly()->isAlive())
                res.emplace_back(x, y);
    return res;
}

std::pair<int, int> Field::findTowerPosition(const EnemyTower* tower) const {
    if (!tower) throw std::invalid_argument("Tower pointer is null");
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            auto cellTower = grid_[y][x].getTower();
            if (cellTower.get() == tower) {
                return {x, y};
            }
        }
    }
    
    throw std::runtime_error("Tower not found on field");
}

bool Field::movePlayer(int dx, int dy) {
    auto [x, y] = findPlayer();
    int nx = x + dx, ny = y + dy;
    if (!inBounds(nx, ny)) return false;
    Cell& target = grid_[ny][nx];
    if (!target.isPassable()) return false;

    auto player = grid_[y][x].getPlayer();
    if (!player) return false;

    if (player->getAttackMode() == AttackMode::RANGED) {
        return rangedAttack(dx, dy, player->getRangedRange());
    }

    if (auto enemy = target.getEnemy()) {
        if (enemy->isAlive()) {
            enemy->takeDamage(player->getDamage());
            
            GameEvent event;
            event.type = EventType::PLAYER_ATTACK;
            event.actor = "Player";
            event.target = "Enemy";
            event.value = player->getDamage();
            event.x = nx;
            event.y = ny;
            EventSystem::getInstance().notify(event);
            
            if (!enemy->isAlive()) {
                target.setEnemy(nullptr);
                player->addPoints(10);
                
                GameEvent destroyEvent;
                destroyEvent.type = EventType::ENEMY_DESTROYED;
                destroyEvent.actor = "Player";
                destroyEvent.target = "Enemy";
                destroyEvent.x = nx;
                destroyEvent.y = ny;
                EventSystem::getInstance().notify(destroyEvent);
            }
            return true;
        }
    }

    if (auto base = target.getBase()) {
        if (base->isAlive()) {
            base->takeDamage(player->getDamage());
            
            GameEvent event;
            event.type = EventType::PLAYER_ATTACK;
            event.actor = "Player";
            event.target = "Base";
            event.value = player->getDamage();
            event.x = nx;
            event.y = ny;
            EventSystem::getInstance().notify(event);
            
            if (!base->isAlive()) {
                target.setBase(nullptr);
                player->addPoints(50);
                
                GameEvent destroyEvent;
                destroyEvent.type = EventType::BASE_DESTROYED;
                destroyEvent.actor = "Player";
                destroyEvent.target = "Base";
                destroyEvent.x = nx;
                destroyEvent.y = ny;
                EventSystem::getInstance().notify(destroyEvent);
            }
            return true;
        }
    }

    if (auto tower = target.getTower()) {
        if (tower->isAlive()) {
            tower->takeDamage(player->getDamage());
            
            GameEvent event;
            event.type = EventType::PLAYER_ATTACK;
            event.actor = "Player";
            event.target = "Tower";
            event.value = player->getDamage();
            event.x = nx;
            event.y = ny;
            EventSystem::getInstance().notify(event);
            
            if (!tower->isAlive()) {
                target.setTower(nullptr);
                player->addPoints(30);
                player->getHand().addRandomSpell();
                
                GameEvent destroyEvent;
                destroyEvent.type = EventType::TOWER_DESTROYED;
                destroyEvent.actor = "Player";
                destroyEvent.target = "Tower";
                destroyEvent.x = nx;
                destroyEvent.y = ny;
                EventSystem::getInstance().notify(destroyEvent);
                
                GameEvent spellEvent;
                spellEvent.type = EventType::SPELL_OBTAINED;
                spellEvent.actor = "Player";
                spellEvent.details = "За уничтожение башни";
                EventSystem::getInstance().notify(spellEvent);
            }
            return true;
        }
    }

    if (target.hasTrap()) {
        player->takeDamage(15);
        target.setTrap(false);
        
        GameEvent event;
        event.type = EventType::TRAP_TRIGGERED;
        event.actor = "Player";
        event.value = 15;
        event.x = nx;
        event.y = ny;
        EventSystem::getInstance().notify(event);
    }

    grid_[y][x].setPlayer(nullptr);
    grid_[ny][nx].setPlayer(player);

    if (target.isSlow()) {
        player->setSlowed(true);
    }

    return true;
}

bool Field::rangedAttack(int dx, int dy, int range) {
    auto [x, y] = findPlayer();
    auto player = grid_[y][x].getPlayer();
    if (!player) return false;

    bool attacked = false;

    for (int i = 1; i <= range; ++i) {
        int nx = x + dx * i;
        int ny = y + dy * i;
        
        if (!inBounds(nx, ny)) break;
        
        Cell& target = grid_[ny][nx];
        
        if (auto enemy = target.getEnemy()) {
            if (enemy->isAlive()) {
                enemy->takeDamage(player->getDamage());
                
                GameEvent event;
                event.type = EventType::PLAYER_ATTACK;
                event.actor = "Player";
                event.target = "Enemy";
                event.value = player->getDamage();
                event.x = nx;
                event.y = ny;
                EventSystem::getInstance().notify(event);
                
                if (!enemy->isAlive()) {
                    target.setEnemy(nullptr);
                    player->addPoints(10);
                    
                    GameEvent destroyEvent;
                    destroyEvent.type = EventType::ENEMY_DESTROYED;
                    destroyEvent.actor = "Player";
                    destroyEvent.target = "Enemy";
                    destroyEvent.x = nx;
                    destroyEvent.y = ny;
                    EventSystem::getInstance().notify(destroyEvent);
                }
                attacked = true;
                break;
            }
        }
        
        if (auto base = target.getBase()) {
            if (base->isAlive()) {
                base->takeDamage(player->getDamage());
                
                GameEvent event;
                event.type = EventType::PLAYER_ATTACK;
                event.actor = "Player";
                event.target = "Base";
                event.value = player->getDamage();
                event.x = nx;
                event.y = ny;
                EventSystem::getInstance().notify(event);
                
                if (!base->isAlive()) {
                    target.setBase(nullptr);
                    player->addPoints(50);
                    
                    GameEvent destroyEvent;
                    destroyEvent.type = EventType::BASE_DESTROYED;
                    destroyEvent.actor = "Player";
                    destroyEvent.target = "Base";
                    destroyEvent.x = nx;
                    destroyEvent.y = ny;
                    EventSystem::getInstance().notify(destroyEvent);
                }
                attacked = true;
                break;
            }
        }
        
        if (auto tower = target.getTower()) {
            if (tower->isAlive()) {
                tower->takeDamage(player->getDamage());
                
                GameEvent event;
                event.type = EventType::PLAYER_ATTACK;
                event.actor = "Player";
                event.target = "Tower";
                event.value = player->getDamage();
                event.x = nx;
                event.y = ny;
                EventSystem::getInstance().notify(event);
                
                if (!tower->isAlive()) {
                    target.setTower(nullptr);
                    player->addPoints(30);
                    player->getHand().addRandomSpell();
                    
                    GameEvent destroyEvent;
                    destroyEvent.type = EventType::TOWER_DESTROYED;
                    destroyEvent.actor = "Player";
                    destroyEvent.target = "Tower";
                    destroyEvent.x = nx;
                    destroyEvent.y = ny;
                    EventSystem::getInstance().notify(destroyEvent);
                    
                    GameEvent spellEvent;
                    spellEvent.type = EventType::SPELL_OBTAINED;
                    spellEvent.actor = "Player";
                    spellEvent.details = "За уничтожение башни";
                    EventSystem::getInstance().notify(spellEvent);
                }
                attacked = true;
                break;
            }
        }
        
        if (!target.isPassable()) {
            break;
        }
    }
    
    return attacked;
}

void Field::moveEnemies() {
    auto enemies = findEnemies();
    auto playerPos = findPlayer();
    auto player = grid_[playerPos.second][playerPos.first].getPlayer();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (const auto& [x, y] : enemies) {
        auto enemy = grid_[y][x].getEnemy();
        if (!enemy || !enemy->isAlive()) continue;
        
        if (std::uniform_real_distribution<>(0, 1)(gen) < 0.8) {
            int dx = (playerPos.first > x) ? 1 : (playerPos.first < x) ? -1 : 0;
            int dy = (playerPos.second > y) ? 1 : (playerPos.second < y) ? -1 : 0;
            
            if (std::uniform_real_distribution<>(0, 1)(gen) < 0.5 && dx != 0) dy = 0;
            if (std::uniform_real_distribution<>(0, 1)(gen) < 0.5 && dy != 0) dx = 0;
            
            int nx = x + dx, ny = y + dy;
            if (inBounds(nx, ny) && grid_[ny][nx].isPassable()) {
                if (grid_[ny][nx].getPlayer()) {
                    player->takeDamage(enemy->getDamage());
                    
                    GameEvent event;
                    event.type = EventType::PLAYER_DAMAGE_TAKEN;
                    event.actor = "Enemy";
                    event.target = "Player";
                    event.value = enemy->getDamage();
                    event.x = nx;
                    event.y = ny;
                    EventSystem::getInstance().notify(event);
                    
                } else if (grid_[ny][nx].isEmpty()) {
                    grid_[y][x].setEnemy(nullptr);
                    grid_[ny][nx].setEnemy(enemy);
                }
            }
        }
    }
}

void Field::updateBases() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            auto base = grid_[y][x].getBase();
            if (!base || !base->isAlive()) continue;
            base->tick();
            if (base->canSpawn()) {
                std::vector<std::pair<int, int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
                for (auto [dx, dy] : dirs) {
                    int nx = x + dx, ny = y + dy;
                    if (inBounds(nx, ny) && grid_[ny][nx].isEmpty() && grid_[ny][nx].isPassable()) {
                        grid_[ny][nx].setEnemy(std::make_shared<Enemy>());
                        base->resetTimer();
                        
                        GameEvent event;
                        event.type = EventType::ENEMY_DESTROYED;
                        event.actor = "Base";
                        event.target = "Spawn";
                        event.x = nx;
                        event.y = ny;
                        event.details = "Создан новый враг";
                        EventSystem::getInstance().notify(event);
                        
                        break;
                    }
                }
            }
        }
    }
}

void Field::initializeRandomObstacles(double wallProb, double slowProb) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            double r = dis(gen);
            if (r < wallProb) {
                grid_[y][x] = Cell(CellType::WALL);
            } else if (r < wallProb + slowProb) {
                grid_[y][x] = Cell(CellType::SLOW);
            }
        }
    }
}

void Field::placeTrap(int x, int y) {
    if (inBounds(x, y) && getCell(x, y).isEmpty()) {
        grid_[y][x].setTrap(true);
    }
}

void Field::placeAlly(int x, int y, std::shared_ptr<Ally> ally) {
    if (inBounds(x, y) && getCell(x, y).isEmpty()) {
        grid_[y][x].setAlly(ally);
        
        GameEvent event;
        event.type = EventType::ALLY_SUMMONED;
        event.actor = "Player";
        event.x = x;
        event.y = y;
        EventSystem::getInstance().notify(event);
    }
}