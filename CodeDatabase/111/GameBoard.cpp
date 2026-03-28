#include "GameBoard.h"
#include "GameActor.h"
#include "Foe.h"
#include "EnemyTower.h"
#include <random>
#include <algorithm>

GameBoard::GameBoard(size_t width, size_t height) 
    : width(width), height(height) {
}

GameBoard::GameBoard(size_t width, size_t height, const std::vector<Location>& obstacles)
    : width(width), height(height), obstacles(obstacles) {
}

GameBoard::GameBoard(const GameBoard& other) {
    copyFrom(other);
}

GameBoard::GameBoard(GameBoard&& other) noexcept 
    : width(other.width), height(other.height),
      obstacles(std::move(other.obstacles)),
      actors(std::move(other.actors)) {
    other.width = 0;
    other.height = 0;
}

GameBoard& GameBoard::operator=(const GameBoard& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

GameBoard& GameBoard::operator=(GameBoard&& other) noexcept {
    if (this != &other) {
        clear();
        width = other.width;
        height = other.height;
        obstacles = std::move(other.obstacles);
        actors = std::move(other.actors);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

GameBoard::~GameBoard() {
    clear();
}

bool GameBoard::isLocationValid(const Location& loc) const {
    return loc.row < width && loc.col < height;
}

bool GameBoard::isLocationFree(const Location& loc) const {
    if (!isLocationValid(loc) || isObstacle(loc)) {
        return false;
    }
    
    for (GameActor* actor : actors) {
        if (actor->getPosition() == loc && actor->isAlive()) {
            return false;
        }
    }
    
    return true;
}

bool GameBoard::isObstacle(const Location& loc) const {
    return std::find(obstacles.begin(), obstacles.end(), loc) != obstacles.end();
}

bool GameBoard::isMoveValid(const Location& from, const Location& to, GameActor* actor) const {
    if (!isLocationValid(to)) {
        return false;
    }
    
    // Если пытаемся переместиться на ту же клетку
    if (to == from) {
        return true;
    }
    
    // Проверяем препятствия
    if (isObstacle(to)) {
        return false;
    }
    
    // Проверяем других акторов
    GameActor* target = getActorAt(to);
    if (target && target != actor && target->isAlive()) {
        // Разрешаем перемещение на клетку с врагом или башней для атаки
        if (dynamic_cast<Foe*>(target) || dynamic_cast<EnemyTower*>(target)) {
            return true; // Разрешаем атаку
        }
        return false; // Другие акторы блокируют движение
    }
    
    return true; // Клетка свободна
}

void GameBoard::addActor(GameActor* actor) {
    actors.push_back(actor);
}

void GameBoard::moveActor(const Location& from, const Location& to, GameActor* actor) {
    if (isMoveValid(from, to, actor)) {
        // Проверяем, не атакуем ли мы кого-то
        GameActor* target = getActorAt(to);
        if (target && target != actor && target->isAlive()) {
            // Если это враг или башня - вызываем столкновение для атаки
            if (dynamic_cast<Foe*>(target) || dynamic_cast<EnemyTower*>(target)) {
                actor->handleCollision(target);
            }
        } else {
            // Обычное перемещение
            actor->setPosition(to);
        }
    }
}

GameActor* GameBoard::getActorAt(const Location& loc) const {
    for (GameActor* actor : actors) {
        if (actor->getPosition() == loc && actor->isAlive()) {
            return actor;
        }
    }
    return nullptr;
}

void GameBoard::removeAllActors() {
    actors.clear();
}

size_t GameBoard::getWidth() const {
    return width;
}

size_t GameBoard::getHeight() const {
    return height;
}

int GameBoard::calculateDistance(const Location& a, const Location& b) const {
    return std::abs(static_cast<int>(a.row) - static_cast<int>(b.row)) + 
           std::abs(static_cast<int>(a.col) - static_cast<int>(b.col));
}

void GameBoard::generateRandomObstacles(int minCount, int maxCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> countDist(minCount, maxCount);
    std::uniform_int_distribution<size_t> rowDist(0, width - 1);
    std::uniform_int_distribution<size_t> colDist(0, height - 1);
    
    int obstacleCount = countDist(gen);
    obstacles.clear();
    
    for (int i = 0; i < obstacleCount; ++i) {
        Location obstacle;
        int attempts = 0;
        do {
            obstacle = Location(rowDist(gen), colDist(gen));
            attempts++;
        } while ((!isLocationFree(obstacle) || obstacle == Location(0, 0)) && attempts < 100);
        
        if (attempts < 100) {
            obstacles.push_back(obstacle);
        }
    }
}

void GameBoard::copyFrom(const GameBoard& other) {
    width = other.width;
    height = other.height;
    obstacles = other.obstacles;
    actors = other.actors; // Note: this copies pointers, not objects
}

void GameBoard::clear() {
    actors.clear();
    obstacles.clear();
}