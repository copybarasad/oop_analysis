#include "GameField.h"
#include "GameExceptions.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

GameField::GameField(int fieldWidth, int fieldHeight, const Position& playerStartPos) 
    : width(fieldWidth), height(fieldHeight) {
    
    // Проверка валидности размеров поля
    if (fieldWidth <= 0 || fieldHeight <= 0) {
        throw FieldException("Неверные размеры поля: " + 
                           std::to_string(fieldWidth) + "x" + 
                           std::to_string(fieldHeight));
    }
    
    // Проверка максимальных размеров (предотвращение переполнения памяти)
    if (fieldWidth > 100 || fieldHeight > 100) {
        throw FieldException("Слишком большие размеры поля: " +
                           std::to_string(fieldWidth) + "x" + 
                           std::to_string(fieldHeight));
    }
    
    // Инициализация клеток
    cells.resize(width);
    for (int x = 0; x < width; x++) {
        cells[x].resize(height);
        for (int y = 0; y < height; y++) {
            cells[x][y] = Cell();
        }
    }
    
    // Генерация специальных клеток
    try {
        initializeBlockedCells(playerStartPos);
        initializeSlowedCells(playerStartPos);
    } catch (const std::exception& e) {
        throw FieldException("Ошибка инициализации специальных клеток: " + std::string(e.what()));
    }
}

// Конструктор копирования (глубокая копия)
GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height) {
    
    try {
        copyFrom(other);
    } catch (const std::exception& e) {
        throw FieldException("Ошибка копирования игрового поля: " + std::string(e.what()));
    }
}

// Конструктор перемещения
GameField::GameField(GameField&& other) noexcept {
    moveFrom(std::move(other));
}

// Оператор присваивания с копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        try {
            copyFrom(other);
        } catch (const std::exception& e) {
            throw FieldException("Ошибка присваивания игрового поля: " + std::string(e.what()));
        }
    }
    return *this;
}

// Оператор присваивания с перемещением
GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

bool GameField::isPositionValid(const Position& position) const {
    return position.first >= 0 && position.first < width && 
           position.second >= 0 && position.second < height;
}

Cell& GameField::getCell(const Position& position) {
    // Исключение при невалидной позиции
    if (!isPositionValid(position)) {
        throw FieldException("Неверная позиция: (" + 
                           std::to_string(position.first) + ", " + 
                           std::to_string(position.second) + ")");
    }
    return cells[position.first][position.second];
}

const Cell& GameField::getCell(const Position& position) const {
    // Исключение при невалидной позиции
    if (!isPositionValid(position)) {
        throw FieldException("Неверная позиция: (" + 
                           std::to_string(position.first) + ", " + 
                           std::to_string(position.second) + ")");
    }
    return cells[position.first][position.second];
}

bool GameField::canMoveToPosition(const Position& position) const {
    if (!isPositionValid(position)) {
        return false;
    }
    const Cell& cell = getCell(position);
    return cell.isPassable();
}

bool GameField::placeObjectAtPosition(const Position& position) {
    if (!canMoveToPosition(position)) {
        return false;
    }
    
    try {
        Cell& cell = getCell(position);
        cell.setOccupied(true);
        return true;
    } catch (const FieldException& e) {
        // Перебрасываем исключение с дополнительной информацией
        throw FieldException("Не удалось разместить объект на позиции (" +
                           std::to_string(position.first) + ", " + 
                           std::to_string(position.second) + "): " + e.what());
    }
}

void GameField::removeObjectFromPosition(const Position& position) {
    if (isPositionValid(position)) {
        try {
            Cell& cell = getCell(position);
            cell.setOccupied(false);
        } catch (const FieldException& e) {
            // Игнорируем ошибки при удалении - позиция может быть невалидной
        }
    }
}

void GameField::initializeBlockedCells(const Position& playerStartPos) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int blockedCount = (width * height) / 10;

    for (int i = 0; i < blockedCount; i++) {
        int x = std::rand() % width;
        int y = std::rand() % height;
        
        // Не ставим блоки рядом с игроком
        if (x == playerStartPos.first && y == playerStartPos.second) continue;
        if (abs(x - playerStartPos.first) <= 1 && abs(y - playerStartPos.second) <= 1) continue;
        
        try {
            cells[x][y].setType(CellType::BLOCKED);
        } catch (const std::exception& e) {
            throw FieldException("Ошибка установки заблокированной клетки: " + std::string(e.what()));
        }
    }
}

void GameField::initializeSlowedCells(const Position& playerStartPos) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)) + 1);
    int slowedCount = (width * height) / 10;

    for (int i = 0; i < slowedCount; i++) {
        int x = std::rand() % width;
        int y = std::rand() % height;
        
        // Не ставим замедление на игрока и рядом
        if (x == playerStartPos.first && y == playerStartPos.second) continue;
        if (abs(x - playerStartPos.first) <= 2 && abs(y - playerStartPos.second) <= 2) continue;
        
        try {
            // Не ставим на заблокированные клетки
            if (cells[x][y].getType() == CellType::BLOCKED) continue;
            
            cells[x][y].setType(CellType::SLOW);
        } catch (const std::exception& e) {
            throw FieldException("Ошибка установки замедляющей клетки: " + std::string(e.what()));
        }
    }
}

// Вспомогательный метод для глубокого копирования
void GameField::copyFrom(const GameField& other) {
    width = other.width;
    height = other.height;
    
    cells.resize(width);
    for (int x = 0; x < width; x++) {
        cells[x].resize(height);
        for (int y = 0; y < height; y++) {
            cells[x][y] = other.cells[x][y];
        }
    }
}

// Вспомогательный метод для перемещения
void GameField::moveFrom(GameField&& other) noexcept {
    width = other.width;
    height = other.height;
    cells = std::move(other.cells);
    
    // Обнуляем перемещенный объект
    other.width = 0;
    other.height = 0;
}
