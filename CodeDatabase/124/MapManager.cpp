#include "MapManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

MapManager::MapManager(int w, int h) : width(w), height(h) {
    if (w < 10 || w > 25 || h < 10 || h > 25) 
        throw std::invalid_argument("Размер поля 10x10 до 25x25");
    allocateCells();
}

MapManager::~MapManager() { deallocateCells(); }

void MapManager::allocateCells() {
    cells = new Cell*[height];
    for (int i = 0; i < height; i++) 
        cells[i] = new Cell[width];
}

void MapManager::deallocateCells() {
    if (cells) {
        for (int i = 0; i < height; i++) 
            delete[] cells[i];
        delete[] cells;
        cells = nullptr;
    }
}

bool MapManager::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

Cell& MapManager::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) 
        throw std::out_of_range("Координаты вне границ");
    return cells[y][x];
}

bool MapManager::isCellEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    const Cell& cell = getCell(x, y);
    return !cell.getIsOccupied() && cell.getIsPassable();
}

void MapManager::clearCell(int x, int y) {
    if (isValidPosition(x, y)) 
        getCell(x, y).clear();
}

void MapManager::initRandom() { 
    static bool initialized = false;
    if (!initialized) {
        std::srand(std::time(0));
        initialized = true; 
    }
}

void MapManager::addObstacles(int count) {
    initRandom();
    int added = 0, attempts = count * 3;
    while (added < count && attempts-- > 0) {
        int x = std::rand() % width, y = std::rand() % height;
        if (isValidPosition(x, y) && getCell(x, y).getIsPassable() && !getCell(x, y).getIsOccupied()) {
            getCell(x, y).setIsPassable(false);
            added++;
        }
    }
}

void MapManager::addObstacle(int x, int y) {
    if (isValidPosition(x, y)) 
        getCell(x, y).setIsPassable(false);
}