#include "map.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

Map::Map(int length, int width) 
    : length(std::max(5, std::min(50, length))),
      width(std::max(5, std::min(50, width))) {
    initializeBoard();
}

void Map::initializeBoard() {
    board.resize(length);
    for (int x = 0; x < length; ++x) {
        board[x].resize(width);
        for (int y = 0; y < width; ++y) {
            board[x][y] = std::make_unique<Cell>(true, Cell::Type::COMMON);
        }
    }
}

const Cell& Map::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Map::getCell: Invalid position (" + 
                               std::to_string(x) + ", " + 
                               std::to_string(y) + ")");
    }
    
    if (x >= static_cast<int>(board.size()) || y >= static_cast<int>(board[x].size())) {
        throw std::out_of_range("Map::getCell: Index out of range");
    }
    
    if (!board[x][y]) {
        throw std::runtime_error("Map::getCell: Cell is null");
    }
    
    return *board[x][y];
}

Cell& Map::getCell(int x, int y) {
    return const_cast<Cell&>(static_cast<const Map*>(this)->getCell(x, y));
}

int Map::getLength() const {
    return length;
}

int Map::getWidth() const {
    return width;
}

bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < length && y >= 0 && y < width;
}

bool Map::isCellPassable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return board[x][y]->isPassable();
}

void Map::setCellType(int x, int y, Cell::Type type) {
    if (isValidPosition(x, y)) {
        board[x][y]->setCellType(type);
    }
}

void Map::printMap() const {
    std::cout << "   ";
    for (int x = 0; x < length; ++x) {
        std::cout << std::setw(2) << x % 10 << ' ';
    }
    std::cout << '\n';
    
    for (int y = 0; y < width; ++y) {
        std::cout << std::setw(2) << y % 10 << ' ';
        for (int x = 0; x < length; ++x) {
            const Cell& cell = *board[x][y];
            
            if (!cell.isEmptyCell()) {
                std::cout << " X ";
            } else {
                switch (cell.getCellType()) {
                    case Cell::Type::BLOCKING:
                        std::cout << " # ";
                        break;
                    case Cell::Type::SLOWING:
                        std::cout << " ~ ";
                        break;
                    case Cell::Type::COMMON:
                    default:
                        std::cout << " . ";
                        break;
                }
            }
        }
        std::cout << '\n';
    }
}