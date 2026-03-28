#include "gamefield.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// Trap implementation
Trap::Trap(const Position& position, int damage) 
    : position(position), damage(damage), active(true) {}

bool Trap::trigger(Player* player) {
    if (!active || !player) return false;
    
    if (player->getPosition() == position) {
        player->takeDamage(damage);
        active = false;
        std::cout << "Trap triggered! " << damage << " damage dealt!\n";
        return true;
    }
    return false;
}

const Position& Trap::getPosition() const { return position; }
bool Trap::isActive() const { return active; }
int Trap::getDamage() const { return damage; }
void Trap::deactivate() { active = false; }

// GameField implementation
GameField::GameField(int width, int height) : width(width), height(height), externalPlayer(nullptr) {
}

GameField::~GameField() {
    // Destructor - dynamic arrays already handled by unique_ptr
}

GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height) {
    traps = other.traps;
    for (const auto& tower : other.towers) {
        towers.push_back(std::unique_ptr<EnemyTower>(new EnemyTower(*tower)));
    }
}

GameField::GameField(GameField&& other) 
    : width(other.width), height(other.height),
      traps(std::move(other.traps)), 
      towers(std::move(other.towers)),
      externalPlayer(std::move(other.externalPlayer)) {
    other.width = 0;
    other.height = 0;
    other.externalPlayer = nullptr;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        traps = other.traps;
        towers.clear();
        for (const auto& tower : other.towers) {
            towers.push_back(std::unique_ptr<EnemyTower>(new EnemyTower(*tower)));
        }
        externalPlayer = other.externalPlayer;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        traps = std::move(other.traps);
        towers = std::move(other.towers);
        externalPlayer = std::move(other.externalPlayer);
        other.width = 0;
        other.height = 0;
        other.externalPlayer = nullptr;
    }
    return *this;
}

int GameField::getWidth() const { return width; }
int GameField::getHeight() const { return height; }

bool GameField::isValidPosition(const Position& position) const {
    return position.x >= 0 && position.x < width && 
           position.y >= 0 && position.y < height;
}

void GameField::placePlayer(Player* player) {
    externalPlayer = player;
}

void GameField::clearCell(const Position& position) {
    traps.erase(std::remove_if(traps.begin(), traps.end(), 
        [&](const Trap& trap) { return trap.getPosition() == position; }), 
        traps.end());
}

void GameField::placeTrap(const Position& position, int damage) {
    if (isValidPosition(position)) {
        traps.emplace_back(position, damage);
    }
}

void GameField::addTower(std::unique_ptr<EnemyTower> tower) {
    if (tower && isValidPosition(tower->getPosition())) {
        towers.push_back(std::move(tower));
    }
}

void GameField::addEnemy(const MovingEnemy& enemy) {
    enemies.push_back(enemy);
}

const std::vector<MovingEnemy>& GameField::getEnemies() const {
    return enemies;
}

bool GameField::hasEnemyAt(const Position& pos) const {
    for (const auto& enemy : enemies) {
        if (enemy.getPosition() == pos && enemy.isAlive()) {
            return true;
        }
    }
    return false;
}

bool GameField::hasBuildingAt(const Position& position) const {
    for (const auto& tower : towers) {
        if (tower->getPosition() == position) {
            return true;
        }
    }
    return false;
}

void GameField::applyTowerDamage(Player* player, int damage) {
    if (player) {
        player->takeDamage(damage);
        std::cout << "Player takes " << damage << " tower damage! Health: " << player->getHealth() << "\n";
    }
}

void GameField::applyDamageAt(const Position& position, int damage) {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->getPosition() == position && it->isAlive()) {
            it->takeDamage(damage);
            std::cout << "Enemy at (" << position.x << "," << position.y 
                      << ") takes " << damage << " damage! HP: " 
                      << it->getHealth() << "\n";
            
            if (!it->isAlive()) {
                std::cout << "Enemy defeated!\n";
                it = enemies.erase(it);  // REMOVE DEAD ENEMY
                continue;  // Skip increment
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    // Apply damage to towers
    for (auto it = towers.begin(); it != towers.end(); ) {
        if ((*it)->getPosition() == position) {
            std::cout << "Tower at (" << position.x << ", " << position.y 
                      << ") takes " << damage << " damage!\n";
            
            // Track enemy destruction for spell rewards
            if (externalPlayer) {
                externalPlayer->incrementEnemiesDestroyed();
            }
            
            it = towers.erase(it);
            std::cout << "Tower destroyed!\n";
        } else {
            ++it;
        }
    }
    
    // Apply damage to player if they're at that position
    if (externalPlayer && externalPlayer->getPosition() == position) {
        externalPlayer->takeDamage(damage);
    }
}

int GameField::getAliveEnemyCount() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            count++;
        }
    }
    return count;
}

void GameField::updateTowerCooldowns() {
    for (auto& tower : towers) {
        tower->updateCooldown();
    }
}

void GameField::checkTraps() {
    if (!externalPlayer) return;
    for (auto& trap : traps) {
        trap.trigger(externalPlayer);
    }
    traps.erase(std::remove_if(traps.begin(), traps.end(), 
        [](const Trap& trap) { return !trap.isActive(); }), 
        traps.end());
}

void GameField::display(const std::vector<MovingEnemy>& movingEnemies) const {
    std::cout << "\nGame Field " << width << "x" << height << ":\n";
    
    // Top coordinates
    std::cout << "   ";
    for (int x = 0; x < width; x++) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    for (int y = 0; y < height; y++) {
        // Left coordinate
        if (y < 10) std::cout << y << "  ";
        else std::cout << y << " ";
        
        for (int x = 0; x < width; x++) {
            Position pos(x, y);
            char cell = '.';
            
            // CHECK PLAYER
            if (externalPlayer && externalPlayer->getPosition() == pos) {
                cell = 'P';
            } 
            // CHECK ENEMIES FROM PASSED LIST
            else {
                bool enemyHere = false;
                for (const auto& enemy : movingEnemies) {
                    if (enemy.getPosition() == pos && enemy.isAlive()) {
                        cell = 'E';
                        enemyHere = true;
                        break;
                    }
                }
                // CHECK TOWERS only if no enemy
                if (!enemyHere && hasBuildingAt(pos)) {
                    cell = 'T';
                }
            }
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
}

Player* GameField::getPlayer() const {
    return externalPlayer;
}

void GameField::movePlayer(const Position& newPosition) {
    if (isValidPosition(newPosition)) {
        externalPlayer->setPosition(newPosition);
        checkTraps();
        updateTowerCooldowns();
    }
}