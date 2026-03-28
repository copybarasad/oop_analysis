#include <iostream>
#include "map.hpp"
#include "guard_tower.hpp"
#include <chrono>
#include <thread>



Map::Map(int mapWidth, int mapHeight) 
    : width(mapWidth), height(mapHeight) {
    width = std::min(25, std::max(10, mapWidth));
    height = std::min(25, std::max(10, mapHeight));
    grid.resize(height, std::vector<Cell>(width));
    
    std::srand(std::time(nullptr));
    
    int placedWalls = 0;
    while (placedWalls < width * height * 0.05) {
        int x = rand() % width;
        int y = rand() % height;
        
        bool canPlace = true;
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = x + dx;
                int ny = y + dy;
                
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    if (grid[ny][nx].getState() == CellState::WALL) {
                        canPlace = false;
                        break;
                    }
                }
            }
            if (!canPlace) break;
        }
        
        if (canPlace && grid[y][x].getState() == CellState::EMPTY) {
            grid[y][x].setWall(true);
            placedWalls++;
        }
    }
}


Map::~Map() {
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.getState() == CellState::MONSTER) {
                cell.removeEntity();
            }
        }
    }
}

void Map::print() const {
    for (const auto& row : grid) {
        for (const Cell& cell : row) {
            switch (cell.getState()) {
                case CellState::EMPTY:
                    std::cout << ". ";
                    break;
                case CellState::PLAYER:
                    std::cout << "P ";
                    break;
                case CellState::MONSTER:
                    std::cout << "M ";
                    break;
                case CellState::WALL:
                    std::cout << "# ";
                    break;
                case CellState::TRAP:
                    std::cout << "T ";
                    break;
                case CellState::TOWER:
                    std::cout << "G ";
                    break;
            }
        }
        std::cout << std::endl;
    }
}
bool Map::addPlayer(int x, int y, Player* playerPtr) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    if (grid[y][x].getState() != CellState::EMPTY) {
        return false;
    }
    
    grid[y][x].setEntity(playerPtr, CellState::PLAYER);
    playerX = x;
    playerY = y;
    return true;
}

bool Map::addEntity(int x, int y, std::unique_ptr<Entity> entity, CellState state) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    if (grid[y][x].getState() != CellState::EMPTY) {
        return false;
    }
    
    grid[y][x].setEntity(entity.release(), state);
    return true;
}

CellState Map::getCellState(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return CellState::EMPTY;
    }
    return grid[y][x].getState();
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}
bool Map::moveEntity(int fromX, int fromY, int toX, int toY) {
    if (fromX < 0 || fromX >= width || fromY < 0 || fromY >= height ||
        toX < 0 || toX >= width || toY < 0 || toY >= height) {
        return false;
    }
    
    Cell& fromCell = grid[fromY][fromX];
    Cell& toCell = grid[toY][toX];
    
    if (!(fromCell.getState() == CellState::PLAYER || fromCell.getState() == CellState::MONSTER)) {
        return false;
    }

    if (!fromCell.getEntityCanMove()) {
        return false;
    }

    if (toCell.getState() == CellState::EMPTY || toCell.getState() == CellState::TRAP) {
        CellState entityState = fromCell.getState();
        //bool trapped = CellState::TRAP == toCell.getState();
        fromCell.transferEntityTo(toCell);
        /*if (trapped) {
            toCell.applyDamage(100);
        }*/
        
        if (entityState == CellState::PLAYER) {
            playerX = toX;
            playerY = toY;
        }

        toCell.setEntityCanMove(false);
        
        return true;
    }
    else if ((toCell.getState() == CellState::MONSTER || toCell.getState() == CellState::PLAYER || toCell.getState() == CellState::TOWER) && !(fromCell.getState() == CellState::MONSTER && toCell.getState() != CellState::PLAYER)){
        int damage = fromCell.getEntityDamage();
        
        toCell.applyDamage(damage);
        
        fromCell.setEntityCanMove(false);
        return true;
    }
    return false;
}

void Map::makeStep(int x, int y) {
    grid[y][x].entityMakeStep(this, x, y);
}

int Map::getMonsterCount() const {
    int count = 0;
    for (const auto& row : grid) {
        for (const Cell& cell : row) {
            if (cell.getState() == CellState::MONSTER) {
                count++;
            }
        }
    }
    return count;
}

int Map::getPlayerX() const {
    return playerX;
}

int Map::getPlayerY() const {
    return playerY;
}

void Map::everyoneCanWalk() {
    for (int i=0;i<height;i++) {
        for (int j=0;j<width;j++) {
            grid[i][j].setEntityCanMove(true);
        }
    }
}

// Конструктор копирования (глубокая копия)
Map::Map(const Map& other) 
    : width(other.width), height(other.height), 
      playerX(other.playerX), playerY(other.playerY) {
    
    grid.resize(height);
    for (int i = 0; i < height; ++i) {
        grid[i].resize(width);
        for (int j = 0; j < width; ++j) {
            CellState state = other.grid[i][j].getState();
            
            if (state == CellState::EMPTY) {
                continue;
            } else if (state == CellState::WALL) {
                grid[i][j].setWall(true);
            } else if (state == CellState::PLAYER) {
                continue;
            } else if (state == CellState::MONSTER) {
                int monsterHp = other.grid[i][j].getEntityHp();
                int monsterDamage = other.grid[i][j].getEntityDamage();
                bool canMove = other.grid[i][j].getEntityCanMove();
                
                Monster* monsterCopy = new Monster(monsterHp, monsterDamage);
                monsterCopy->setCanMove(canMove);
                grid[i][j].setEntity(monsterCopy, CellState::MONSTER);
            }
        }
    }
}

// Оператор присваивания с копированием
Map& Map::operator=(const Map& other) {
    if (this != &other) {
        for (auto& row : grid) {
            for (auto& cell : row) {
                cell.removeEntity();
            }
        }
        
        width = other.width;
        height = other.height;
        playerX = other.playerX;
        playerY = other.playerY;
        
        grid.resize(height);
        for (int i = 0; i < height; ++i) {
            grid[i].resize(width);
            for (int j = 0; j < width; ++j) {
                CellState state = other.grid[i][j].getState();
                
                if (state == CellState::EMPTY) {
                    continue;
                } else if (state == CellState::WALL) {
                    grid[i][j].setWall(true);
                } else if (state == CellState::PLAYER) {
                    grid[i][j].setEntity(nullptr, CellState::EMPTY);
                } else if (state == CellState::MONSTER) {
                    int monsterHp = other.grid[i][j].getEntityHp();
                    int monsterDamage = other.grid[i][j].getEntityDamage();
                    bool canMove = other.grid[i][j].getEntityCanMove();
                    
                    Monster* monsterCopy = new Monster(monsterHp, monsterDamage);
                    monsterCopy->setCanMove(canMove);
                    grid[i][j].setEntity(monsterCopy, CellState::MONSTER);
                }
            }
        }
    }
    return *this;
}

// Конструктор перемещения
Map::Map(Map&& other) noexcept 
    : grid(std::move(other.grid)), 
      width(other.width), height(other.height),
      playerX(other.playerX), playerY(other.playerY) {
    
    other.width = 0;
    other.height = 0;
    other.playerX = -1;
    other.playerY = -1;
    other.grid.clear();
}

// Оператор присваивания с перемещением
Map& Map::operator=(Map&& other) noexcept {
    if (this != &other) {
        for (auto& row : grid) {
            for (auto& cell : row) {
                cell.removeEntity();
            }
        }
        
        grid = std::move(other.grid);
        width = other.width;
        height = other.height;
        playerX = other.playerX;
        playerY = other.playerY;
        
        other.width = 0;
        other.height = 0;
        other.playerX = -1;
        other.playerY = -1;
        other.grid.clear();
    }
    return *this;
}

void Map::applyDamageAt(int x, int y, int damage) {
    CellState cs = getCellState(x, y);
    if (cs == CellState::MONSTER || cs == CellState::PLAYER || cs == CellState::TOWER) {
        grid[y][x].applyDamage(damage);
    }
}

void Map::setTrapAt(int x, int y) {
    grid[y][x].setTrap(true);
}

void Map::setWallAt(int x, int y, bool wall) {
    grid[y][x].setWall(wall);
}

void Map::clearMap() {
    for (int i=0;i<height;i++) {
        for (int j=0;j<width;j++) {
            grid[i][j].setWall(false);
        }
    }
}

std::string Map::getCellStatusAt(int x, int y) const {
    return grid[y][x].getCellStatus();
}
