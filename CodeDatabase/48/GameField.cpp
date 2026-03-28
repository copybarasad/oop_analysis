#include "lib/GameField.h"
#include <iostream>
#include <random>
#include <stdexcept>
#include <iomanip>

// Конструктор
GameField::GameField(int w, int h) {
    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    
    width = w;
    height = h;
    initializeField();
    
    // Автоматическая генерация препятствий при создании поля
    generateObstacles(0.2); // 20% препятствий
}

// Конструктор копирования (глубокая копия)
GameField::GameField(const GameField& other) {
    width = other.width;
    height = other.height;
    copyField(other);
}

// Конструктор перемещения
GameField::GameField(GameField&& other) noexcept {
    width = other.width;
    height = other.height;
    field = std::move(other.field);
    
    // Обнуляем исходный объект
    other.width = 0;
    other.height = 0;
}

// Оператор присваивания с копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        copyField(other);
    }
    return *this;
}

// Оператор присваивания с перемещением
GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        field = std::move(other.field);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

// Инициализация поля
void GameField::initializeField() {
    field.resize(height);
    for (int i = 0; i < height; ++i) {
        field[i].resize(width);
        for (int j = 0; j < width; ++j) {
            field[i][j] = Cell(true); // Все клетки проходимые по умолчанию
        }
    }
}

// Глубокая копия поля
void GameField::copyField(const GameField& other) {
    field.resize(other.height);
    for (int i = 0; i < other.height; ++i) {
        field[i].resize(other.width);
        for (int j = 0; j < other.width; ++j) {
            field[i][j] = other.field[i][j]; // Копируем каждую клетку
        }
    }
}

// Геттеры
int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell& GameField::getCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    return field[y][x];
}

const Cell& GameField::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    return field[y][x];
}

// Проверка координат
bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Очистка клетки
void GameField::clearCell(int x, int y) {
    if (isValidPosition(x, y)) {
        field[y][x].setHasPlayer(false);
        field[y][x].setHasEnemy(false);
    }
}

// Установка позиции игрока
void GameField::setPlayerPosition(int x, int y) {
    if (isValidPosition(x, y) && isCellPassable(x, y)) {
        // Очищаем старую позицию игрока
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (field[i][j].getHasPlayer()) {
                    field[i][j].setHasPlayer(false);
                }
            }
        }
        field[y][x].setHasPlayer(true);
    }
}

// Установка позиции врага
void GameField::setEnemyPosition(int x, int y) {
    if (isValidPosition(x, y) && isCellPassable(x, y)) {
        field[y][x].setHasEnemy(true);
    }
}

// Проверка проходимости клетки
bool GameField::isCellPassable(int x, int y) const {
    return isValidPosition(x, y) && field[y][x].getIsPassable();
}

// Проверка пуста ли клетка
bool GameField::isCellEmpty(int x, int y) const {
    return isValidPosition(x, y) && field[y][x].isEmpty();
}

void GameField::setTowerPosition(int x, int y) {
    if (isValidPosition(x, y) && isCellPassable(x, y)) {
        field[y][x].setHasTower(true);
    }
}

// Очистка позиции башни
void GameField::clearTowerPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        field[y][x].setHasTower(false);
    }
}

// Отображение поля
void GameField::display() const {
    std::cout << "\n";
    // Верхняя граница с координатами
    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << std::setw(2) << x;
    }
    std::cout << "\n";
    
    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << "--";
    }
    std::cout << "-\n";
    
    for (int y = 0; y < height; ++y) {
        // Левая граница с координатами
        std::cout << std::setw(2) << y << "|";
        
        for (int x = 0; x < width; ++x) {
            std::cout << " " << field[y][x].getSymbol();
        }
        std::cout << " |\n";
    }
    
    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << "--";
    }
    std::cout << "-\n";
    
    std::cout << "Legend: P=Player, E=Enemy, T=Tower, #=Wall, .=Empty\n";
}

// Генерация препятствий
void GameField::generateObstacles(double obstaclePercentage) {
    if (obstaclePercentage < 0 || obstaclePercentage > 0.5) {
        obstaclePercentage = 0.2; // Защита от некорректного значения
    }
    
    int totalCells = width * height;
    int obstacleCount = static_cast<int>(totalCells * obstaclePercentage);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);
    
    int generated = 0;
    int attempts = 0;
    const int maxAttempts = totalCells * 2;
    
    while (generated < obstacleCount && attempts < maxAttempts) {
    int x = distX(gen);
    int y = distY(gen);
    
    // Защищаем углы И клетки вокруг углов
    bool isProtected = 
        // Сам угол (0,0) и соседние клетки
        (x == 0 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == 1) || (x == 1 && y == 1) ||
        // Угол (w-1,0) и соседние
        (x == width-1 && y == 0) || (x == width-2 && y == 0) || 
        (x == width-1 && y == 1) || (x == width-2 && y == 1) ||
        // Угол (0,h-1) и соседние  
        (x == 0 && y == height-1) || (x == 1 && y == height-1) ||
        (x == 0 && y == height-2) || (x == 1 && y == height-2) ||
        // Угол (w-1,h-1) и соседние
        (x == width-1 && y == height-1) || (x == width-2 && y == height-1) ||
        (x == width-1 && y == height-2) || (x == width-2 && y == height-2);
    
        if (!isProtected && isCellEmpty(x, y)) {
            field[y][x].setIsPassable(false);
            generated++;
        }
    attempts++;
    }
    
    std::cout << "Generated " << generated << " obstacles on " << width << "x" << height << " field\n";
}