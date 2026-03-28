#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include <stdexcept>
#include <cstdlib>
#include <algorithm>

Field::Field(int w, int h) : width(w), height(h) {
    if (w < 10 || h < 10 || w > 25 || h > 25) 
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");

    grid = new Cell*[height];
    for (int i = 0; i < height; i++) 
        grid[i] = new Cell[width];

    initializeWalls();
}

Field::Field(const Field& other) : width(other.width), height(other.height) {
    grid = new Cell*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new Cell[width];
        for (int j = 0; j < width; j++)
            grid[i][j] = other.grid[i][j];
    }

    if (other.player) {
        player = std::make_unique<Player>(*other.player);
    }

    enemies.reserve(other.enemies.size());
    for (const auto& enemy : other.enemies) {
        enemies.push_back(std::make_unique<Enemy>(*enemy));
    }
}

Field& Field::operator=(const Field& other) {
    if (this == &other)
        return *this;

    for (int i = 0; i < height; i++) 
        delete[] grid[i];
    delete[] grid;
    
    player.reset();
    enemies.clear();

    width = other.width;
    height = other.height;

    grid = new Cell*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new Cell[width];
        for (int j = 0; j < width; j++)
            grid[i][j] = other.grid[i][j];
    }

    if (other.player) {
        player = std::make_unique<Player>(*other.player);
    }

    enemies.reserve(other.enemies.size());
    for (const auto& enemy : other.enemies) {
        enemies.push_back(std::make_unique<Enemy>(*enemy));
    }

    return *this;
}

Field::Field(Field&& other) noexcept 
    : width(other.width), height(other.height), grid(other.grid), 
      player(std::move(other.player)), enemies(std::move(other.enemies)) {
    other.grid = nullptr;
    other.width = 0;
    other.height = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other)
        return *this;

    for (int i = 0; i < height; i++) 
        delete[] grid[i];
    delete[] grid;

    width = other.width;
    height = other.height;
    grid = other.grid;
    player = std::move(other.player);
    enemies = std::move(other.enemies);

    other.grid = nullptr;
    other.width = 0;
    other.height = 0;

    return *this;
}

Field::~Field() {
    for (int i = 0; i < height; i++) 
        delete[] grid[i];
    delete[] grid;
}

void Field::initializeWalls() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (std::rand() % 10 == 0) 
                grid[i][j].setType(CellType::Wall);
        }
    }
}

void Field::setPlayer(std::unique_ptr<Player> p) {
    player = std::move(p);
    if (player) {
        grid[player->getY()][player->getX()].setType(CellType::Player);
    }
}

void Field::addEnemy(std::unique_ptr<Enemy> enemy) {
    if (enemy) {
        grid[enemy->getY()][enemy->getX()].setType(CellType::Enemy);
        enemies.push_back(std::move(enemy));
    }
}

void Field::removeEnemy(Enemy* enemy) {
    auto it = std::find_if(enemies.begin(), enemies.end(),
        [enemy](const std::unique_ptr<Enemy>& e) { return e.get() == enemy; });
    
    if (it != enemies.end()) {
        grid[(*it)->getY()][(*it)->getX()].setType(CellType::Empty);
        enemies.erase(it);
    }
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Field::isWall(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return grid[y][x].isWall();
}

bool Field::isEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return grid[y][x].isEmpty();
}

bool Field::hasPlayerAt(int x, int y) const {
    return player && player->getX() == x && player->getY() == y;
}

bool Field::hasEnemyAt(int x, int y) const {
    return getEnemyAt(x, y) != nullptr;
}

Player* Field::getPlayer() const {
    return player.get();
}

Enemy* Field::getEnemyAt(int x, int y) const {
    for (const auto& enemy : enemies) {
        if (enemy->getX() == x && enemy->getY() == y) {
            return enemy.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Enemy>>& Field::getEnemies() const {
    return enemies;
}

void Field::clearCell(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(CellType::Empty);
    }
}

void Field::setWall(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(CellType::Wall);
    }
}

void Field::placeEntity(Entity* entity) {
    if (entity && isValidPosition(entity->getX(), entity->getY())) {
        CellType type = (dynamic_cast<Player*>(entity)) ? CellType::Player : CellType::Enemy;
        grid[entity->getY()][entity->getX()].setType(type);
    }
}

int Field::getWidth() const {
    return width;
}

int Field::getHeight() const {
    return height;
}

void Field::draw() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (grid[i][j].getType()) {
                case CellType::Empty: std::cout << "."; break;
                case CellType::Player: std::cout << "P"; break;
                case CellType::Enemy: std::cout << "E"; break;
                case CellType::Wall:  std::cout << "#"; break;
            }
        }
        std::cout << "\n";
    }
}
