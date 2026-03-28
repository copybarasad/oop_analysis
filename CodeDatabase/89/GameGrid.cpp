#include "GameGrid.h"
#include <iostream>
#include <stdexcept>

GameGrid::GameGrid(int fieldWidth, int fieldHeight, int impassableCellsCount) 
    : width(fieldWidth), height(fieldHeight), 
      grid(height, std::vector<Cell>(width)) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field dimensions must be between 10x10 and 25x25.");
    }
    initializeGrid();
    generateImpassableCells(impassableCellsCount);
}

int GameGrid::getWidth() const { return width; }
int GameGrid::getHeight() const { return height; }

void GameGrid::initializeGrid() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j].setType(CellType::Empty);
        }
    }
}

void GameGrid::generateImpassableCells(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);

    int placed = 0;
    int attempts = 0;
    const int maxAttempts = count * 10 + 100;

    while (placed < count && attempts < maxAttempts) {
        int x = disX(gen);
        int y = disY(gen);
        attempts++;

        if (x < 3 && y < 3) {
            continue;
        }

        if (grid[y][x].getType() == CellType::Empty) {
            grid[y][x].setType(CellType::Impassable);
            placed++;
        }
    }
}

bool GameGrid::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

Cell& GameGrid::getCell(int x, int y) {
    if (!isValidCoordinate(x, y)) throw std::out_of_range("Invalid coordinates");
    return grid[y][x];
}

const Cell& GameGrid::getCell(int x, int y) const {
    if (!isValidCoordinate(x, y)) throw std::out_of_range("Invalid coordinates");
    return grid[y][x];
}

void GameGrid::updateCell(int x, int y, CellType type) {
    if (isValidCoordinate(x, y)) {
        grid[y][x].setType(type);
    }
}

bool GameGrid::isCellPassable(int x, int y) const {
    return isValidCoordinate(x, y) && grid[y][x].isPassable();
}

bool GameGrid::isCellEmpty(int x, int y) const {
    return isValidCoordinate(x, y) && grid[y][x].getType() == CellType::Empty;
}

void GameGrid::printGrid() const {
    std::cout << "\n--- Game Field ---" << std::endl;
    
    std::cout << "  ";
    for (int j = 0; j < width; ++j) {
        std::cout << (j % 10) << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < height; ++i) {
        std::cout << (i % 10) << " ";
        for (int j = 0; j < width; ++j) {
            char displayChar = '.';
            switch (grid[i][j].getType()) {
                case CellType::Empty:      displayChar = '.'; break;
                case CellType::Impassable: displayChar = '#'; break;
                case CellType::Player:     displayChar = 'P'; break;
                case CellType::Enemy:      displayChar = 'E'; break;
                case CellType::Tower:      displayChar = 'T'; break;
                case CellType::Trap:       displayChar = 'X'; break;
            }
            std::cout << displayChar << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "------------------" << std::endl;
}