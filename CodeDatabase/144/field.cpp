#include "field.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

Field::Field(int width, int height) 
    : width(std::max(10, std::min(25, width))), 
      height(std::max(10, std::min(25, height))) {
    
    grid.resize(height, std::vector<Cell>(width));
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double randomValue = dis(gen);
            if (randomValue < 0.15) {
                grid[y][x] = Cell(CellType::WALL);
            } else if (randomValue < 0.25) {
                grid[y][x] = Cell(CellType::SLOW);
            }
        }
    }
    
    for (int y = 0; y < height; ++y) {
        grid[y][0] = Cell(CellType::WALL);
        grid[y][width - 1] = Cell(CellType::WALL);
    }
    for (int x = 0; x < width; ++x) {
        grid[0][x] = Cell(CellType::WALL);
        grid[height - 1][x] = Cell(CellType::WALL);
    }
}

Field::Field(const Field& other) 
    : width(other.width), height(other.height), grid(other.grid) {}

Field::Field(Field&& other) noexcept
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int Field::getWidth() const {
    return width;
}

int Field::getHeight() const {
    return height;
}

Cell Field::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return Cell(CellType::WALL);
    }
    return grid[y][x];
}

CellType Field::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return CellType::WALL;
    }
    return grid[y][x].getType();
}

bool Field::canMoveTo(int x, int y) const {
    return isValidPosition(x, y) && isCellPassable(x, y);
}

bool Field::canPlaceEntity(int x, int y, int playerX, int playerY) const {
    return isValidPosition(x, y) && isCellPassable(x, y) && 
           !(x == playerX && y == playerY);
}

bool Field::isCellPassable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].isPassable();
}

bool Field::isSlowCell(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].getType() == CellType::SLOW;
}

void Field::findPathToPlayer(int fromX, int fromY, int playerX, int playerY, int& moveX, int& moveY) const {
    moveX = fromX;
    moveY = fromY;
    if (std::abs(playerX - fromX) > std::abs(playerY - fromY)) {
        if (playerX > fromX && isCellPassable(fromX + 1, fromY)) {
            moveX = fromX + 1;
        } else if (playerX < fromX && isCellPassable(fromX - 1, fromY)) {
            moveX = fromX - 1;
        } else if (playerY > fromY && isCellPassable(fromX, fromY + 1)) {
            moveY = fromY + 1;
        } else if (playerY < fromY && isCellPassable(fromX, fromY - 1)) {
            moveY = fromY - 1;
        }
    } else {
        if (playerY > fromY && isCellPassable(fromX, fromY + 1)) {
            moveY = fromY + 1;
        } else if (playerY < fromY && isCellPassable(fromX, fromY - 1)) {
            moveY = fromY - 1;
        } else if (playerX > fromX && isCellPassable(fromX + 1, fromY)) {
            moveX = fromX + 1;
        } else if (playerX < fromX && isCellPassable(fromX - 1, fromY)) {
            moveX = fromX - 1;
        }
    }
}

bool Field::hasLineOfSight(int fromX, int fromY, int toX, int toY) const {
    int dx = std::abs(toX - fromX);
    int dy = std::abs(toY - fromY);
    int sx = (fromX < toX) ? 1 : -1;
    int sy = (fromY < toY) ? 1 : -1;
    int err = dx - dy;
    
    int currentX = fromX;
    int currentY = fromY;
    
    while (currentX != toX || currentY != toY) {
        if (currentX != fromX || currentY != fromY) {
            if (!isCellPassable(currentX, currentY)) {
                return false;
            }
        }
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            currentX += sx;
        }
        if (e2 < dx) {
            err += dx;
            currentY += sy;
        }
    }
    
    return true;
}

Field::Snapshot Field::createSnapshot() const {
	Snapshot snapshot;
	snapshot.width = width;
	snapshot.height = height;
	snapshot.cells.reserve(width * height);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			snapshot.cells.push_back(grid[y][x].getType());
		}
	}
	return snapshot;
}

void Field::applySnapshot(const Snapshot& snapshot) {
	width = snapshot.width;
	height = snapshot.height;
	grid.assign(height, std::vector<Cell>(width));
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int index = y * width + x;
			if (index >= 0 && index < static_cast<int>(snapshot.cells.size())) {
				grid[y][x].setType(snapshot.cells[index]);
			}
		}
	}
}