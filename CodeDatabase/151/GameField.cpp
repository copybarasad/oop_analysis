#include "GameField.h"
#include <iostream>
#include <algorithm>
#include <cmath>

GameField::GameField(int width, int height) : width(width), height(height), player(nullptr) {
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    initializeGrid();
}

GameField::GameField(const GameField& other)
    : width(other.width), height(other.height), player(nullptr) {

    // Копируем сетку
    grid = other.grid;

    // Копируем игрока (глубокая копия)
    if (other.player) {
        player = new Player(other.player->getPosition().x, other.player->getPosition().y);
    }

    // Копируем врагов (глубокая копия)
    for (const auto& enemy : other.enemies) {
        enemies.push_back(new Enemy(*enemy));
    }

    towers = other.towers;
    traps = other.traps;
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height),
    grid(std::move(other.grid)),
    player(other.player),
    enemies(std::move(other.enemies)),  
    towers(std::move(other.towers)),      
    traps(std::move(other.traps)) {

    other.player = nullptr;
    other.width = 0;
    other.height = 0;
}

GameField::~GameField() {
    for (auto enemy : enemies) {
        delete enemy;
    }
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        // Очищаем текущие данные
        delete player;
        for (auto enemy : enemies) {
            delete enemy;
        }
        enemies.clear();

        // Копируем новые данные
        width = other.width;
        height = other.height;
        grid = other.grid;

        if (other.player) {
            player = new Player(other.player->getPosition().x, other.player->getPosition().y);
        }
        else {
            player = nullptr;
        }

        for (const auto& enemy : other.enemies) {
            enemies.push_back(new Enemy(*enemy));
        }

        towers = other.towers;
        traps = other.traps;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        // Очищаем текущие данные
        delete player;
        for (auto enemy : enemies) {
            delete enemy;
        }

        // Перемещаем данные
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        player = other.player;
        enemies = std::move(other.enemies);
        towers = std::move(other.towers);   
        traps = std::move(other.traps);

        // Обнуляем исходный объект
        other.player = nullptr;
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameField::initializeGrid() {
    grid.resize(height, std::vector<Cell>(width));
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell& GameField::getCell(const Position& pos) {
    return grid[pos.y][pos.x];
}

const Cell& GameField::getCell(const Position& pos) const {
    return grid[pos.y][pos.x];
}

Player* GameField::getPlayer() const {
    return player;
}

const std::vector<Enemy*>& GameField::getEnemies() const {
    return enemies;
}

void GameField::placePlayer(int x, int y) {
    Position pos(x, y);
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Invalid player position");
    }

    if (player) {
        // Убираем игрока со старой позиции
        getCell(player->getPosition()).setHasPlayer(false);
        player->setPosition(pos);
    }
    getCell(pos).setHasPlayer(true);
}

void GameField::addEnemy(int x, int y) {
    Position pos(x, y);
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Invalid enemy position");
    }

    if (getCell(pos).isEmpty()) {
        enemies.push_back(new Enemy(x, y));
        getCell(pos).setHasEnemy(true);
    }
}

void GameField::removeDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!(*it)->isAlive()) {
            Position pos = (*it)->getPosition();
            getCell(pos).setHasEnemy(false);
            delete* it;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool GameField::movePlayer(int dx, int dy) {
    if (!player || !player->isAlive()) return false;

    Position currentPos = player->getPosition();
    Position newPos(currentPos.x + dx, currentPos.y + dy);

    if (!isValidPosition(newPos)) return false;

    Cell& newCell = getCell(newPos);

    // Проверяем, не стоит ли на новой позиции враг
    if (newCell.getHasEnemy()) {
        // Игрок атакует врага
        for (auto enemy : enemies) {
            if (enemy->getPosition() == newPos && enemy->isAlive()) {
                enemy->takeDamage(player->getDamage());
                if (!enemy->isAlive()) {
                    player->addScore(10);
                    newCell.setHasEnemy(false);
                }
                break;
            }
        }
        return true;
    }

    // Перемещаем игрока
    if (newCell.isEmpty()) {
        getCell(currentPos).setHasPlayer(false);
        player->setPosition(newPos);
        newCell.setHasPlayer(true);
        return true;
    }

    return false;
}

void GameField::moveEnemies() {
    if (!player) return;

    for (auto enemy : enemies) {
        if (!enemy->isAlive()) continue;

        Position currentPos = enemy->getPosition();
        Position newPos = enemy->getNextMove(player->getPosition());

        if (!isValidPosition(newPos)) continue;

        Cell& newCell = getCell(newPos);

        // Если на новой позиции игрок - атакуем
        if (newCell.getHasPlayer()) {
            player->takeDamage(enemy->getDamage());
            continue;
        }

        // Перемещаем врага
        if (newCell.isEmpty()) {
            getCell(currentPos).setHasEnemy(false);
            enemy->setPosition(newPos);
            newCell.setHasEnemy(true);
        }
    }
}

bool GameField::isGameOver() const {
    return !player || !player->isAlive();
}

void GameField::addTower(const EnemyTower& tower) {
    towers.push_back(tower);
    Position pos = tower.getPosition();
}

void GameField::addTrap(const TrapOnField& trap) {
    traps.push_back(trap);
    std::cout << "Trap placed at (" << trap.getPosition().x << ", " << trap.getPosition().y << ")\n";
}

void GameField::updateTowers() {
    for (auto& tower : towers) {
        tower.update(this);
    }
}

void GameField::checkTraps() {
    for (auto& trap : traps) {
        for (auto* enemy : enemies) {
            if (enemy->isAlive() && trap.checkTrigger(enemy->getPosition())) {
                enemy->takeDamage(trap.getDamage());
                trap.deactivate();
                std::cout << "Trap triggered! Enemy took " << trap.getDamage() << " damage!\n";
                if (!enemy->isAlive()) {
                    player->addScore(10);
                }
                break;
            }
        }
    }
}

void GameField::removeDeactivatedTraps() {
    traps.erase(
        std::remove_if(traps.begin(), traps.end(),
            [](const TrapOnField& trap) { return !trap.isActive(); }),
        traps.end()
    );
}

void GameField::applyDamageInArea(const Position& center, int areaSize, int damage) {
    int halfSize = areaSize / 2;
    for (int x = center.x - halfSize; x <= center.x + halfSize; x++) {
        for (int y = center.y - halfSize; y <= center.y + halfSize; y++) {
            Position pos(x, y);
            applyDamageToPosition(pos, damage);
        }
    }
}

void GameField::applyDamageToPosition(const Position& pos, int damage) {
    if (!isValidPosition(pos)) return;

    if (isEnemyAtPosition(pos)) {
        for (auto* enemy : enemies) {
            if (enemy->getPosition() == pos && enemy->isAlive()) {
                enemy->takeDamage(damage);
                std::cout << "Enemy hit for " << damage << " damage!\n";
                if (!enemy->isAlive()) {
                    player->addScore(10);
                    getCell(pos).setHasEnemy(false);
                }
                break;
            }
        }
    }
}

bool GameField::isEnemyAtPosition(const Position& pos) const {
    for (auto* enemy : enemies) {
        if (enemy->isAlive() && enemy->getPosition() == pos) {
            return true;
        }
    }
    return false;
}

bool GameField::isTowerAtPosition(const Position& pos) const {
    for (const auto& tower : towers) {
        if (tower.getPosition() == pos) {
            return true;
        }
    }
    return false;
}

bool GameField::isEmptyForTrap(const Position& pos) const {
    return isValidPosition(pos) &&
        !isEnemyAtPosition(pos) &&
        !isTowerAtPosition(pos) &&
        !(player && player->getPosition() == pos);
}

const std::vector<EnemyTower>& GameField::getTowers() const {
    return towers;
}

const std::vector<TrapOnField>& GameField::getTraps() const {
    return traps;
}

bool GameField::isLevelCleared() const {
    return enemies.empty();
}

void GameField::setPlayer(Player* p) {
    player = p;
}

void GameField::display() const {
    std::cout << "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Position pos(x, y);
            const Cell& cell = getCell(pos);

            if (cell.getHasPlayer()) {
                std::cout << "P ";
            }
            else if (cell.getHasEnemy()) {
                std::cout << "E ";
            }
            else if (isTowerAtPosition(pos)) {
                std::cout << "T ";
            }
            else {
                bool hasTrap = false;
                for (const auto& trap : traps) {
                    if (trap.getPosition() == pos && trap.isActive()) {
                        std::cout << "X ";
                        hasTrap = true;
                        break;
                    }
                }
                if (!hasTrap) {
                    std::cout << ". ";
                }
            }
        }
        std::cout << "\n";
    }

    if (player) {
        std::cout << "\nPlayer - Health: " << player->getHealth()
            << " | Score: " << player->getScore()
            << " | Mana: " << player->getMana() << "\n";
    }

    std::cout << "Enemies: " << enemies.size()
        << " | Towers: " << towers.size()
        << " | Traps: " << traps.size() << "\n";
}