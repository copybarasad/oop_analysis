#include <random>
#include "GameField.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Constants.h"
#include "InstantDamageSpell.h"
#include <iostream>
#include <queue>
#include <cmath>

GameField::GameField(int width, int height, int level, std::function<void(int, int, int, float)> applyDamage): 
    width_(width)
    , height_(height)
    , applyDamage(applyDamage)
    , level_(level) {
    std::cout << "level " << level_ << std::endl;
    if(
        !(width >= 10 && width <= 25 && height >= 10 && height <= 25)
    ) {
        throw std::invalid_argument("Invalid field size");
    }
    std::srand(std::time(nullptr));
    bool spawned_enemy_spawner = false;
    bool spawned_player = false;
    bool spawned_tower = false;

    cells_.resize(height_);

    for (int i = 0; i < height_; i++) {
        cells_[i].reserve(width_);
        for (int j = 0; j < width_; j++) {
            if(i == 0 || i == height_ - 1 || j == 0 || j == width_ - 1) {
                cells_[i].emplace_back(TILE_WALL);
            } else if(rand() % 10 == 0) {
                cells_[i].emplace_back(TILE_WALL);
            } else if(rand() % 10 == 1) {
                cells_[i].emplace_back(TILE_PLAYER_TRAP);
            }
            else{
                cells_[i].emplace_back(TILE_FLOOR);
                if(!spawned_enemy_spawner && rand() % 3 == 1) {
                    cells_[i][j].setSpawnerBuilding(
                        new SpawnerBuilding("simple", [this](GameField* f){
                        f->spawnEntity("simple");
                    } , 3)
                    );
                    spawned_enemy_spawner = true;
                } else if(!spawned_player && rand() % 3 == 1) {
                    addPlayer(new Player("crystalKing", 3.0f, 200 * level, 30 * level), j, i);
                    spawned_player = true;
                } else if(!spawned_tower && rand() % 3 == 1) {
                    cells_[i][j].setTower(
                        new Tower(new InstantDamageSpell(1, 40 * level), [applyDamage](int damage, int x, int y, float delay){
                            applyDamage(damage, x, y, delay);
                        }, 5, 3, 200 * level)
                    );
                    spawned_tower = true;
                }
            }
        }
    }
}

GameField::~GameField() {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            delete cells_[y][x].getEnemy();
            delete cells_[y][x].getSpawnerBuilding();
            delete cells_[y][x].getPlayer();
            delete cells_[y][x].getAlly();
            delete cells_[y][x].getTower();
        }
    }
}

void GameField::addEnemy(Enemy* entity, int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        if(!isFree(x, y)) return;
        entity->setX(x);
        entity->setY(y);
        entity->setTarget(x, y);
        cells_[y][x].setEnemy(entity);

        if(cells_[y][x].getTrap()) {
            applyDamage(cells_[y][x].getTrap(), x, y, 0.5f);
            cells_[y][x].removeTrap();
        }
    }
}

void GameField::addAlly(Enemy* entity, int x, int y) {
    if(!entity) return;
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        if(!isFree(x, y)) return;
        entity->setX(x);
        entity->setY(y);
        entity->setTarget(x, y);
        cells_[y][x].setAlly(entity);
    }
}

void GameField::addTower(Tower* tower, int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        if(!isFree(x, y)) return;
        cells_[y][x].setTower(tower);
    }
}

void GameField::addPlayer(Player* player, int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        player->setX(x);
        player->setY(y);
        player->setTarget(x, y);
        cells_[y][x].setPlayer(player);
    }
}

void GameField::addBuilding(SpawnerBuilding* building, int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        cells_[y][x].setSpawnerBuilding(building);
    }
}

std::pair<int, int> GameField::getEnemyCell(Enemy* entity) const {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getEnemy() == entity) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

std::pair<int, int> GameField::getAllyCell(Enemy* entity) const {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getAlly() == entity) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

std::pair<int, int> GameField::getTowerCell(Tower* entity) const {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getTower() == entity) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

std::pair<int, int> GameField::getPlayerCell() const {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getPlayer() != nullptr) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

Player* GameField::getPlayer() const {
    std::pair<int, int> pos = getPlayerCell();
    if(pos.first == -1 || pos.second == -1) return nullptr;
    return cells_[pos.second][pos.first].getPlayer();
}

void GameField::spawnEntity(std::string spawner_name) {
    int sx = -1, sy = -1;
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            SpawnerBuilding* b = cells_[y][x].getSpawnerBuilding();
            if(b != nullptr && b->getName() == spawner_name) {
                sx = x;
                sy = y;
                break;
            }
        }
        if(sx != -1 && sy != -1) break;
    }
    if(sx != -1 && sy != -1) {
        for(int y = std::max(0, sy - 1); y < std::min(height_, sy + 2); y++) {
            for(int x = std::max(0, sx - 1); x < std::min(width_, sx + 2); x++) {
                if(x == sx && y == sy) continue;
                if(isFree(x, y)) {
                    addEnemy(new Enemy("fireKing", 3.0f, 200 * level_, 20 * level_), x, y);
                    return;
                }
            }   
        }
    }
}

void GameField::spawnAlly(Enemy* ally) {
    std::pair<int, int> pos = getFreePosition();
    if(pos.first != -1 && pos.second != -1) {
        addAlly(ally, pos.first, pos.second);
        return;
    }
    delete ally;
}

void GameField::moveEnemy(Enemy* entity, int x, int y) {
    std::pair<int, int> cell = getEnemyCell(entity);
    int trap = getTrap(x, y);
    if(cell.first != -1 && cell.second != -1) {
        if(x == cell.first && y == cell.second) return;
        cells_[cell.second][cell.first].setEnemy(nullptr);
        entity->setTarget(x, y);
        cells_[y][x].setEnemy(entity);
        if(trap) {
            std::cout << "Enemy trapped at " << x << ", " << y << " , damage: " << getTrap(x, y) << std::endl;
            applyDamage(getTrap(x, y), x, y, 0.3f);
            cells_[y][x].removeTrap();
        }
    }
}

void GameField::moveAlly(Enemy* entity, int x, int y) {
    std::pair<int, int> cell = getAllyCell(entity);
    if(cell.first != -1 && cell.second != -1) {
        if(x == cell.first && y == cell.second) return;
        cells_[cell.second][cell.first].setAlly(nullptr);
        entity->setTarget(x, y);
        cells_[y][x].setAlly(entity);
    }
}

void GameField::movePlayer(int x, int y) {
    std::pair<int, int> cell = getPlayerCell();
    if(cell.first == -1 && cell.second == -1) return;
    Player* p = cells_[cell.second][cell.first].getPlayer();

    if(p) {
        p->setTarget(x, y);
        cells_[y][x].setPlayer(p);
        cells_[cell.second][cell.first].setPlayer(nullptr);
        if(cells_[y][x].getCell() == TILE_PLAYER_TRAP) {
            p->stuck();
        }
    }
}

std::pair<int, int> GameField::getFreePosition() {
    for(int i = 0; i < height_; i++) {
        for(int j = 0; j < width_; j++) {
           if(cells_[i][j].isFree()) {
                return {j, i};
           }
        }
    }
    return {-1, -1};
}

std::pair<int, int> GameField::getClosestAllyOrPlayer(int x, int y) {
    double dist = width_ + height_;
    std::pair<int, int> pos = {-1, -1};
    for(int i = 0; i < height_; i++) {
        for(int j = 0; j < width_; j++) {
            if(i == y && j == x) continue;
            double current_dist = hypot(x - j, y - i);
            if((cells_[i][j].getPlayer() != nullptr || cells_[i][j].getAlly() != nullptr)
             && current_dist < dist) {
                dist = current_dist;
                pos = {j, i};
            }
        }
    }
    return pos;
}

std::pair<int, int> GameField::getClosestEnemy(int x, int y) {
    double dist = width_ + height_;
    std::pair<int, int> pos = {-1, -1};
    for(int i = 0; i < height_; i++) {
        for(int j = 0; j < width_; j++) {
            if(i == y && j == x) continue;
            Enemy* enemy = cells_[i][j].getEnemy();
            double current_dist = hypot(x - j, y - i);
            if(enemy != nullptr && current_dist < dist) {
                dist = current_dist;
                pos = {j, i};
            }
        }
    }
    return pos;
}

void GameField::swap(int x1, int y1, int x2, int y2) {
    if(x1 >= 0 && x1 < width_ && y1 >= 0 && y1 < height_ && x2 >= 0 && x2 < width_ && y2 >= 0 && y2 < height_) {
        std::swap(cells_[y1][x1], cells_[y2][x2]);
    }
}


void GameField::removeEnemy(Enemy* entity) {
    if(!entity) return;
    std::pair<int, int> cell = getEnemyCell(entity);
    if(cell.first != -1 && cell.second != -1) {
        delete entity;
        cells_[cell.second][cell.first].setEnemy(nullptr);
        std::cout << "GameField.cpp:removeEnemy" << std::endl;
    }
}

void GameField::removeAlly(Enemy* entity) {
    if(!entity) return;
    std::pair<int, int> cell = getAllyCell(entity);
    if(cell.first != -1 && cell.second != -1) {
        delete entity;
        cells_[cell.second][cell.first].setAlly(nullptr);
        std::cout << "GameField.cpp:removeAlly" << std::endl;
    }
}

void GameField::removeTower(Tower* entity) {
    if(!entity) return;
    std::pair<int, int> cell = getTowerCell(entity);
    if(cell.first != -1 && cell.second != -1) {
        delete entity;
        cells_[cell.second][cell.first].setTower(nullptr);
        std::cout << "GameField.cpp:removeTower" << std::endl;
    }
}

void GameField::removePlayer() {
    std::pair<int, int> cell = getPlayerCell();
    if(cell.first != -1 && cell.second != -1) {
        delete cells_[cell.second][cell.first].getPlayer();
        cells_[cell.second][cell.first].setPlayer(nullptr);
    }
}

void GameField::remove(int x, int y) {
    if(x >= 0 && x < width_ && y >= 0 && y < height_) {
        delete cells_[y][x].getEnemy();
        delete cells_[y][x].getSpawnerBuilding();
        delete cells_[y][x].getPlayer();
        delete cells_[y][x].getAlly();
        delete cells_[y][x].getTower();
        cells_[y][x] = Cell(TILE_FLOOR);
    }
}


std::vector<Enemy*> GameField::getEnemiesInRow(int x1, int y1, int x2, int y2) const {
    std::vector<Enemy*> res;
    int sx = x1, sy = y1, ex = x2, ey = y2;
    if(x1 > x2) {
        std::swap(sx, ex);
    }
    if(y1 > y2) {
        std::swap(sy, ey);
    }
    if(sy == ey) {
        for(int x = sx; x <= ex; x++) {
            if(cells_[sy][x].getCell() == TILE_WALL) return res;
            if(cells_[sy][x].getEnemy() != nullptr) {
                res.push_back(cells_[sy][x].getEnemy());
            }
        }
    } else if(sx == ex) {
        for(int y = sy; y <= ey; y++) {
            if(cells_[y][sx].getCell() == TILE_WALL) return res;
            if(cells_[y][sx].getEnemy() != nullptr) {
                res.push_back(cells_[y][sx].getEnemy());
            }
        }
    }
    return res;
}

std::vector<Enemy*> GameField::getEnemiesInArea(int cx, int cy, int radius) const {
    std::vector<Enemy*> res;
    for(int y = std::max(0, int(cy - std::floor(radius / 2.))); y < std::min(height_, int(std::ceil(cy + radius / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(radius / 2.))); x < std::min(width_, int(std::ceil(cx + radius / 2.))); x++) {
            if(cells_[y][x].getEnemy() != nullptr) {
                res.push_back(cells_[y][x].getEnemy());
            }
        }
    }
    return res;
}

std::vector<Tower*> GameField::getTowersInArea(int cx, int cy, int radius) const {
    std::vector<Tower*> res;
    for(int y = std::max(0, int(cy - std::floor(radius / 2.))); y < std::min(height_, int(std::ceil(cy + radius / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(radius / 2.))); x < std::min(width_, int(std::ceil(cx + radius / 2.))); x++) {
            if(cells_[y][x].getTower() != nullptr) {
                res.push_back(cells_[y][x].getTower());
            }
        }
    }
    return res;
}


std::vector<Enemy*> GameField::getAlliesInArea(int cx, int cy, int radius) const {
    std::vector<Enemy*> res;
    for(int y = std::max(0, int(cy - std::floor(radius / 2.))); y < std::min(height_, int(std::ceil(cy + radius / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(radius / 2.))); x < std::min(width_, int(std::ceil(cx + radius / 2.))); x++) {
            if(cells_[y][x].getAlly() != nullptr) {
                res.push_back(cells_[y][x].getAlly());
            }
        }
    }
    return res;
}

std::vector<Entity*> GameField::getAlliesInAreaWithPlayer(int cx, int cy, int radius) const {
    std::vector<Entity*> res;
    for(int y = std::max(0, int(cy - std::floor(radius / 2.))); y < std::min(height_, int(std::ceil(cy + radius / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(radius / 2.))); x < std::min(width_, int(std::ceil(cx + radius / 2.))); x++) {
            if(cells_[y][x].getAlly() != nullptr) {
                res.push_back(cells_[y][x].getAlly());
            } else if(cells_[y][x].getPlayer() != nullptr) {
                res.push_back(cells_[y][x].getPlayer());
            }
        }
    }
    return res;
}

void GameField::setColorInArea(int cx, int cy, int radius, sf::Color color, float t) {
    for(int y = std::max(0, int(cy - std::floor(radius / 2.))); y < std::min(height_, int(std::ceil(cy + radius / 2.))); y++) {
        for(int x = std::max(0, int(cx - std::floor(radius / 2.))); x < std::min(width_, int(std::ceil(cx + radius / 2.))); x++) {
            cells_[y][x].setColor(color, t);
        }
    }
}

std::vector<std::pair<int, int>> GameField::findPath(int x1, int y1, int x2, int y2) const {
    if(x1 == x2 && y1 == y2) return {};
    
    const Cell *startCell = &cells_[y1][x1], *endCell = &cells_[y2][x2];

    std::vector<std::vector<bool>> visited(height_, std::vector<bool>(width_, false));
    std::vector<std::vector<std::pair<int, int>>> came_from(height_, std::vector<std::pair<int, int>>(width_, {-1, -1}));
    std::queue<std::pair<int, int>> q;
    q.push({x1, y1});
    visited[y1][x1] = true;

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};

    while (!q.empty())
    {
        auto p = q.front();
        q.pop();
        int x = p.first, y = p.second;

        if (x == x2 && y == y2) {
            std::vector<std::pair<int, int>> path;
            while (x != -1 && y != -1) {
                path.push_back({x, y});
                auto prev = came_from[y][x];
                x = prev.first;
                y = prev.second;
            }
            path.pop_back();
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || nx >= width_ || ny < 0 || ny >= height_) continue;
            
            if (visited[ny][nx]) continue;
            
            if (getCell(nx, ny) == TILE_WALL || getSpawnerBuildingAt(nx, ny)) continue;
            
            const Cell* cell = &cells_[ny][nx];
            if (cell == nullptr || (!cell->isFree() && cell != startCell && cell != endCell)) continue;

            visited[ny][nx] = true;
            came_from[ny][nx] = {x, y};
            q.push({nx, ny});
        }
    }
    
    return {};
}

void GameField::makeBuildingsMove() {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getSpawnerBuilding() != nullptr) {
                cells_[y][x].getSpawnerBuilding()->makeTurn(this);
            }
            if(cells_[y][x].getTower() != nullptr) {
                std::vector<std::pair<int, int>> allies_positions, enimies_positions;
                int r = cells_[y][x].getTower()->getRadius();
                for(int i = std::max(0, y - r); i < std::min(height_ - 1, y + r); i++) {
                    for(int j = std::max(0, x - r); j < std::min(width_ - 1, x + r); j++) {
                        if(cells_[i][j].getAlly() != nullptr || cells_[i][j].getPlayer() != nullptr) {
                            allies_positions.push_back({j, i});
                        } else if(cells_[i][j].getEnemy() != nullptr) {
                            enimies_positions.push_back({j, i});
                        }
                    }
                }
                cells_[y][x].getTower()->makeTurn(this);
            }
        }
    }
}

// Moved implementations from header file

sf::Color GameField::getColorAt(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with color get at x: " << x << " y: " << y << std::endl; 
        return sf::Color::Black;
    }
    return cells_[y][x].getColor();
}

Enemy* GameField::getEnemyAt(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get enemy at x: " << x << " y: " << y << std::endl;
        return nullptr;
    }
    return cells_[y][x].getEnemy();
}

Enemy* GameField::getAllyAt(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get ally at x: " << x << " y: " << y << std::endl;
        return nullptr;
    }
    return cells_[y][x].getAlly(); 
}

SpawnerBuilding* GameField::getSpawnerBuildingAt(int x, int y) const {
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get ally at x: " << x << " y: " << y << std::endl;
        return nullptr;
    }
    return cells_[y][x].getSpawnerBuilding();
}

Tower* GameField::getTowerAt(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get tower at x: " << x << " y: " << y << std::endl;
        return nullptr;
    }
    return cells_[y][x].getTower(); 
}

char GameField::getCell(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get cell at x: " << x << " y: " << y << std::endl;
        return ' ';
    }
    return cells_[y][x].getCell();
}

int GameField::getTrap(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with get trap at x: " << x << " y: " << y << std::endl;
        return false;
    }
    return cells_[y][x].getTrap();
}

bool GameField::isFree(int x, int y) const { 
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with is free at x: " << x << " y: " << y << std::endl;
        return false;
    }
    return cells_[y][x].isFree();
}

void GameField::installTrapAt(int x, int y, int v) {
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with install trap at x: " << x << " y: " << y << std::endl;
        return;
    }
    if(cells_[y][x].isFree()) {
        std::cerr << "Trap installed at x: " << x << " y: " << y << std::endl;
        cells_[y][x].installTrap(v);
    }
}

void GameField::updateCell(int x, int y, float dt) {
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Out of bounds with update cell at x: " << x << " y: " << y << std::endl;
        return;
    }
    cells_[y][x].update(dt);
}

void GameField::clearSpawnerBuildings() {
    for(int y = 0; y < height_; y++) {
        for(int x = 0; x < width_; x++) {
            if(cells_[y][x].getSpawnerBuilding() != nullptr) {
                delete cells_[y][x].getSpawnerBuilding();
                cells_[y][x].setSpawnerBuilding(nullptr);
            }
        }
    }
}