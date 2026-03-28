#include "GameField.h"
#include <stdexcept>
#include <random>

/**
 * @file GameField.cpp
 * @brief Реализация игрового поля
 */

/**
 * @brief Конструктор поля
 */
GameField::GameField(int fieldWidth, int fieldHeight) 
    : width(fieldWidth), height(fieldHeight) {
    
    if (width < MIN_FIELD_SIZE || width > MAX_FIELD_SIZE || 
        height < MIN_FIELD_SIZE || height > MAX_FIELD_SIZE) {
        throw std::invalid_argument("Invalid field size");
    }
    
    grid.resize(height, std::vector<Cell>(width));
}

/**
 * @brief Конструктор копирования
 */
GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), grid(other.grid) {}

/**
 * @brief Конструктор перемещения
 */
GameField::GameField(GameField&& other) noexcept 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

/**
 * @brief Оператор присваивания копированием
 */
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

/**
 * @brief Оператор присваивания перемещением
 */
GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

/**
 * @brief Проверить проходимость клетки
 */
bool GameField::isCellPassable(int x, int y) const {
    validateCoordinates(x, y);
    return grid[y][x].isPassable();
}

/**
 * @brief Получить клетку (неконстантная версия)
 */
Cell& GameField::getCell(int x, int y) {
    validateCoordinates(x, y);
    return grid[y][x];
}

/**
 * @brief Получить клетку (константная версия)
 */
const Cell& GameField::getCell(int x, int y) const {
    validateCoordinates(x, y);
    return grid[y][x];
}

/**
 * @brief Валидация координат
 */
void GameField::validateCoordinates(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Coordinates out of field bounds");
    }
}

/**
 * @brief Сгенерировать заблокированные клетки
 */
void GameField::generateBlockedCells(int blockedCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);
    
    int generated = 0;
    while (generated < blockedCount) {
        int x = distX(gen);
        int y = distY(gen);
        
        if (grid[y][x].getType() == CellType::EMPTY) {
            grid[y][x].setType(CellType::BLOCKED);
            generated++;
        }
    }
}

/**
 * @brief Установить состояние блокировки клетки (ДОБАВЛЕНО)
 */
void GameField::setCellBlocked(int x, int y, bool blocked) {
    validateCoordinates(x, y);
    if (blocked) {
        grid[y][x].setType(CellType::BLOCKED);
    } else {
        grid[y][x].setType(CellType::EMPTY);
    }
}


