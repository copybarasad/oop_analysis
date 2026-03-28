#include "../include/MapGrid.h"
#include <cstdlib>
#include <ctime>
#include <queue>
#include <set>

MapGrid::MapGrid(int width, int height) 
    : width(width), height(height) {
    
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        this->width = 15;
        this->height = 15;
    }
    
    cells.resize(this->height, std::vector<CellType>(this->width, CellType::EMPTY));
}

MapGrid::MapGrid(const MapGrid& other) 
    : width(other.width), 
      height(other.height), 
      cells(other.cells) {}

MapGrid::MapGrid(MapGrid&& other) noexcept
    : width(other.width),
      height(other.height),
      cells(std::move(other.cells)) {
    
    other.width = 0;
    other.height = 0;
}

MapGrid& MapGrid::operator=(const MapGrid& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = other.cells;
    }
    return *this;
}

MapGrid& MapGrid::operator=(MapGrid&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = std::move(other.cells);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

int MapGrid::getWidth() const {
    return width;
}

int MapGrid::getHeight() const {
    return height;
}

CellType MapGrid::getCellType(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return CellType::WALL;
    }
    return cells[pos.getY() - 1][pos.getX() - 1];
}

bool MapGrid::isValidPosition(const Position& pos) const {
    return pos.getX() >= 1 && pos.getX() <= width && 
           pos.getY() >= 1 && pos.getY() <= height;
}

bool MapGrid::canMoveTo(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return false;
    }
    CellType type = cells[pos.getY() - 1][pos.getX() - 1];
    return type != CellType::WALL;
}

void MapGrid::setCellType(const Position& pos, CellType type) {
    if (isValidPosition(pos)) {
        cells[pos.getY() - 1][pos.getX() - 1] = type;
    }
}

void MapGrid::generateWalls(int wallCount) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initialized = true;
    }
    
    int generated = 0;
    while (generated < wallCount) {
        int x = 2 + rand() % (width - 4);
        int y = 2 + rand() % (height - 4);
        Position pos(x, y);
        
        if (cells[y][x] == CellType::EMPTY) {
            cells[y][x] = CellType::WALL;
            generated++;
        }
    }
}

void MapGrid::generateSlowingCells(int slowingCount) {
    int generated = 0;
    while (generated < slowingCount) {
        int x = 2 + rand() % (width - 4);
        int y = 2 + rand() % (height - 4);
        Position pos(x, y);
        
        if (cells[y][x] == CellType::EMPTY) {
            cells[y][x] = CellType::SLOWING;
            generated++;
        }
    }
}

void MapGrid::clearGrid() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cells[y][x] = CellType::EMPTY;
        }
    }
}

bool MapGrid::isReachable(const Position& from, const Position& to) const {
    if (!canMoveTo(from) || !canMoveTo(to)) {
        return false;
    }
    
    std::queue<Position> queue;
    std::set<std::pair<int, int>> visited;
    
    queue.push(from);
    visited.insert({from.getX(), from.getY()});
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();
        
        if (current == to) {
            return true;
        }
        
        for (int i = 0; i < 4; i++) {
            Position next(current.getX() + dx[i], current.getY() + dy[i]);
            
            if (!isValidPosition(next) || !canMoveTo(next)) {
                continue;
            }
            
            auto coord = std::make_pair(next.getX(), next.getY());
            if (visited.find(coord) == visited.end()) {
                visited.insert(coord);
                queue.push(next);
            }
        }
    }
    
    return false;
}

int MapGrid::countReachableCells(const Position& start) const {
    if (!canMoveTo(start)) {
        return 0;
    }
    
    std::queue<Position> queue;
    std::set<std::pair<int, int>> visited;
    
    queue.push(start);
    visited.insert({start.getX(), start.getY()});
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();
        
        for (int i = 0; i < 4; i++) {
            Position next(current.getX() + dx[i], current.getY() + dy[i]);
            
            if (!isValidPosition(next) || !canMoveTo(next)) {
                continue;
            }
            
            auto coord = std::make_pair(next.getX(), next.getY());
            if (visited.find(coord) == visited.end()) {
                visited.insert(coord);
                queue.push(next);
            }
        }
    }
    
    return visited.size();
}
