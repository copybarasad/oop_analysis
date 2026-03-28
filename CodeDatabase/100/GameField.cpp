#include "GameField.h"
#include "../game_main/GameState.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

// Конструктор
GameField::GameField(int width, int height) : width(width), height(height) {

    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width);
        for (int x = 0; x < width; x++) {
            grid[y][x] = Cell(x, y, true);
        }
    }

    // Генерируем стены и замедляющие клетки (10% от общего числа клеток)
    //std::srand(std::time(0));
    generateWalls((width * height) / 10);
    generateSlowing((width * height) / 10);
}

// Деструктор
GameField::~GameField() { }

// Конструктор копирования (Глубокая копия)
GameField::GameField(const GameField& other): width(other.width), height(other.height), grid(other.grid) { }

// Оператор присваивания копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {  // Защита от самоприсваивания: field = field
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

// Конструктор перемещения
GameField::GameField(GameField&& other) noexcept : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

// Оператор присваивания перемещением
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

// Геттеры
int GameField::getWidth() const { return width; }

int GameField::getHeight() const { return height; }

const Cell& GameField::getCell(int x, int y) const { return grid[y][x]; }

bool GameField::isCellSlowing(int x, int y) const { return grid[y][x].getIsSlowing(); }

bool GameField::isCellTrap(int x, int y) const { return grid[y][x].getHasTrap(); }

bool GameField::isCellPassable(int x, int y) const { 
    if (x < 0 || x >= width || y < 0 || y >= height) 
        return false;
    return grid[y][x].getIsPassable();
}

// Отрисовка спец. клеток
void GameField::generateWalls(int wallCount) {
    for (int i = 0; i < wallCount; i++) {
        int x = std::rand() % width;
        int y = std::rand() % height;
        grid[y][x].setIsPassable(false);
    }
}

void GameField::generateSlowing(int slowCount) {
    for (int i = 0; i < slowCount; i++) {
        int x = std::rand() % width;
        int y = std::rand() % height;
        if (grid[y][x].getIsPassable())
            grid[y][x].setIsSlowing(true);
    }
}

// Установить ловушку
void GameField::setTrap(int x, int y, bool hasTrap) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x].setHasTrap(hasTrap);
    }
}

void GameField::setSlowing(int x, int y, bool isSlowing) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x].setIsSlowing(isSlowing);
    }
}

void GameField::setPassable(int x, int y, bool passable) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x].setIsPassable(passable);
    }
}