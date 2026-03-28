#include "Field.h"
#include "Enemy.h"
#include "Player.h" 
#include <iostream>
#include <cstdlib>
#include <ctime>

void Field::Field_initialize() {
    pos.resize(height, std::vector<Position_field>(width));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pos[y][x].SetBlock(false);
        }
    }
    hasActiveTrap = false;
}

void Field::generate_block(int level, int playerX, int playerY, 
                          const std::vector<Enemy>& enemies, 
                          const std::vector<Tower>& towers) {
    
    srand(time(nullptr) + level); 
    int blockedCount = (width * height) * 0.15;
    int wallsPlaced = 0;

    while (wallsPlaced < blockedCount) {
        int x = rand() % width;
        int y = rand() % height;
        
        // ПРОСТАЯ ПРОВЕРКА: пропускаем клетки с объектами
        if (x == playerX && y == playerY) continue; // игрок
        
        bool occupied = false;
        for (const auto& enemy : enemies) { // враги
            if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                occupied = true;
                break;
            }
        }
        if (occupied) continue;
        
        for (const auto& tower : towers) { // башни
            if (tower.isAlive() && tower.getX() == x && tower.getY() == y) {
                occupied = true;
                break;
            }
        }
        if (occupied) continue;
        
        // Ставим стену
        setWall(x, y);
        wallsPlaced++;
    }
}

Field::Field(int w, int h, bool generateWalls) {
    width = std::max(10, std::min(25, w));
    height = std::max(10, std::min(25, h));
    hasActiveTrap = false;

    Field_initialize();
    
    
    std::cout << "Создано поле размером " << width << "x" << height << std::endl;
}

Field::Field(const Field& other)
    : width(other.width), height(other.height), pos(other.pos) {
    std::cout << "Вызван конструктор копирования" << std::endl;
}

Field::Field(Field&& other) noexcept
    : width(other.width), height(other.height), pos(std::move(other.pos)) {
    other.width = 0;
    other.height = 0;
    std::cout << "Вызван конструктор перемещения" << std::endl;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        pos = other.pos;
    }
    std::cout << "Вызван оператор присваивания с копированием" << std::endl;
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        pos = std::move(other.pos);

        other.width = 0;
        other.height = 0;
    }
    std::cout << "Вызван оператор присваивания с перемещением" << std::endl;
    return *this;
}

void Field::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& cell = pos[y][x];
            
            if (cell.isPlayer()) {
                std::cout << "P ";
            } else if (cell.isEnemy()) {
                std::cout << "E ";
            } else if (cell.isTower()) {
                std::cout << "T ";
            } else if (cell.isTrap()) {
                std::cout << "X ";
            } else if (cell.isBlock()) {
                std::cout << "# ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

bool Field::valid_pos(int x, int y) const {
    return x >= 0 && y >= 0 && x < width && y < height;
}

bool Field::CanMove(int x, int y) const {
    return valid_pos(x, y) && pos[y][x].free_pos();
}


void Field::updateField(int playerX, int playerY, const std::vector<std::pair<int, int>>& enemyPositions) {
    for (auto& row : pos) {
        for (auto& cell : row) {
            cell.SetPlayer(false);
            cell.SetEnemy(false);
            cell.SetTrap(false);
            cell.SetTower(false);
        }
    }

    if (valid_pos(playerX, playerY)) {
        pos[playerY][playerX].SetPlayer(true);
    }

    for (const auto& enemyPos : enemyPositions) {
        if (valid_pos(enemyPos.first, enemyPos.second)) {
            pos[enemyPos.second][enemyPos.first].SetEnemy(true);
        }
    }

    if (hasActiveTrap && valid_pos(trapX, trapY)) {
        pos[trapY][trapX].SetTrap(true);
    }

    for (const auto& tower : towers) {
        if (tower.isAlive() && valid_pos(tower.getX(), tower.getY())) {
            pos[tower.getY()][tower.getX()].SetTower(true);
        }
    }
}

void Field::setTrap(int x, int y, int damage) {
    if (!valid_pos(x, y) || !CanMove(x, y)) {
        std::cout << "Нельзя установить ловушку в эту клетку!\n";
        return;
    }

    if (hasActiveTrap) {
        removeTrap();
        std::cout << "Старая ловушка заменена!\n";
    }

    trapX = x;
    trapY = y;
    trapDamage = damage;
    hasActiveTrap = true;
    
    std::cout << "Ловушка установлена на (" << x << ", " << y << ")!\n";
}

void Field::checkTrap(int x, int y, std::vector<Enemy>& enemies) {
    if (!hasActiveTrap) return;

    if (x == trapX && y == trapY) {
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                std::cout << "Ловушка сработала на враге на (" << x << ", " << y << ")!\n";
                std::cout << "Враг получает " << trapDamage << " урона!\n";
                enemy.takeDamage(trapDamage);
                removeTrap();
                break;
            }
        }
    }
}

void Field::removeTrap() {
    if (hasActiveTrap) {
        hasActiveTrap = false;
        std::cout << "Ловушка деактивирована.\n";
    }
}

void Field::addTower(int x, int y, int range, int health) {
    if (valid_pos(x, y) && CanMove(x, y)) {
        towers.emplace_back(x, y, range);
        std::cout << "Башня построена на (" << x << ", " << y << "), радиус: " << range << std::endl;
    } else {
        std::cout << "Нельзя построить башню на (" << x << ", " << y << ")" << std::endl;
    }
}

void Field::updateTowers(Player& player, std::vector<Enemy>& enemies) {
    for (auto& tower : towers) {
        if (tower.isAlive()) {
            tower.update(player, enemies, *this);
        }
    }
}

std::vector<Tower>& Field::getTowers() {
    return towers;
}

const std::vector<Tower>& Field::getTowers() const {
    return towers;
}

std::vector<std::pair<int, int>> Field::getAllWalls() const {
    std::vector<std::pair<int, int>> walls;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!pos[y][x].free_pos()) {
                walls.push_back({x, y});
            }
        }
    }
    return walls;
}

void Field::setWall(int x, int y) {
    if (valid_pos(x, y)) {
        pos[y][x].SetBlock(true);
    }
}


int Field::getTrapX() const { return trapX; }
int Field::getTrapY() const { return trapY; }
int Field::getTrapDamage() const { return trapDamage; }
bool Field::isTrapActive() const { return hasActiveTrap; }
int Field::getWidth() const { return width; }
int Field::getHeight() const { return height; }