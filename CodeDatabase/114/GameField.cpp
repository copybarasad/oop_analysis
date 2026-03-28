#include "GameField.h"
#include "Cell.h"
#include "Entity.h"
#include "events/EventNotifier.h"
#include "events/EntityDeathEvent.h"
#include <utility>
#include <optional>
#include <functional>

GameField::GameField(int w, int h) : width(w), height(h), field(h, std::vector<Cell>(w)), player(100, 20, 15) {}

GameField::GameField(const GameField& other)
    : width(other.width)
    , height(other.height)
    , field(other.field)
    , enemies(other.enemies)
    , buildings(other.buildings)
    , pets(other.pets)
    , player(other.player)
{}

GameField::GameField(GameField&& other) noexcept
    : width(other.width)
    , height(other.height)
    , field(std::move(other.field))
    , enemies(std::move(other.enemies))
    , buildings(std::move(other.buildings))
    , pets(std::move(other.pets))
    , player(std::move(other.player))
{}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        field = other.field;
        enemies = other.enemies;
        buildings = other.buildings;
        pets = other.pets;
        player = other.player;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        field = std::move(other.field);
        enemies = std::move(other.enemies);
        buildings = std::move(other.buildings);
        pets = std::move(other.pets);
        player = std::move(other.player);
    }
    return *this;
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::isPositionPassable(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return field[y][x].getType() != CellType::WALL && !field[y][x].getHasEntity();
}

bool GameField::isPositionEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return this->isPositionPassable(x, y) && field[y][x].getType() != CellType::SLOW;
}

std::optional<std::reference_wrapper<Entity>> GameField::findEntityAt(int x, int y) {
    for (auto& enemy : enemies) {
        if (enemy.getX() == x && enemy.getY() == y) return std::ref(enemy);
    }
    for (auto& building : buildings) {
        if (building.getX() == x && building.getY() == y) return std::ref(building);
    }
    return std::nullopt;
}

void GameField::removeDeadEntities() {
    // Удаляем мертвых врагов
    auto enemyIt = enemies.begin();
    while (enemyIt != enemies.end()) {
        if (!enemyIt->isAlive()) {
            // Уведомляем о смерти сущности
            int x = enemyIt->getX();
            int y = enemyIt->getY();
            EntityDeathEvent deathEvent(enemyIt->getEntityName(), x, y);
            EventNotifier::getInstance().notify(deathEvent);
            
            // Очищаем клетку от сущности
            if (isValidPosition(x, y)) {
                field[y][x].setHasEntity(false);
            }
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }
    
    // Удаляем мертвые здания
    auto buildingIt = buildings.begin();
    while (buildingIt != buildings.end()) {
        if (!buildingIt->isAlive()) {
            // Уведомляем о смерти сущности
            int x = buildingIt->getX();
            int y = buildingIt->getY();
            EntityDeathEvent deathEvent(buildingIt->getEntityName(), x, y);
            EventNotifier::getInstance().notify(deathEvent);
            
            // Очищаем клетку от сущности
            if (isValidPosition(x, y)) {
                field[y][x].setHasEntity(false);
            }
            buildingIt = buildings.erase(buildingIt);
        } else {
            ++buildingIt;
        }
    }
}
