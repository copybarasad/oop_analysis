#include "GameCell.h"

GameCell::GameCell(CellType cellType)
    : type(cellType), enemy(nullptr) {}

GameCell::GameCell(const GameCell& other)
    : type(other.type), enemy(nullptr) {
    if (other.enemy) {
        enemy = std::make_unique<Enemy>(*other.enemy);
    }
}

GameCell& GameCell::operator=(const GameCell& other) {
    if (this != &other) {
        type = other.type;
        if (other.enemy) {
            enemy = std::make_unique<Enemy>(*other.enemy);
        } else {
            enemy.reset();
        }
    }
    return *this;
}

CellType GameCell::getType() const {
    return type;
}

void GameCell::setType(CellType cellType) {
    type = cellType;
}

bool GameCell::hasEnemy() const {
    return enemy != nullptr;
}

std::optional<Enemy> GameCell::getEnemy() const {
    if (enemy) {
        return *enemy;
    }
    return std::nullopt;
}

void GameCell::setEnemy(std::unique_ptr<Enemy> newEnemy) {
    enemy = std::move(newEnemy);
}

std::unique_ptr<Enemy> GameCell::removeEnemy() {
    return std::move(enemy);
}

bool GameCell::isPassable() const {
    return type != CellType::OBSTACLE && !hasEnemy();
}
