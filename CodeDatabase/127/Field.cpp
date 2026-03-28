#include "Field.h"
#include <stdexcept>  
#include <iostream>

// Конструктор с параметрами (размер поля и его инициализация)
Field::Field(int width, int height) : width_(width), height_(height), grid_(width* height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Размеры поля должны быть больше 0.");
    }
}

// Конструктор копирования
Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_), grid_(other.grid_) {
}

// Оператор присваивания
Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
    }
    return *this;
}

// Геттер для ширины
int Field::getWidth() const {
    return width_;
}

// Геттер для высоты
int Field::getHeight() const {
    return height_;
}

// Метод для проверки, находятся ли координаты внутри поля
bool Field::isInside(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

// Метод для получения ячейки (по индексу)
Cell& Field::getCell(int x, int y) {
    if (!isInside(x, y)) {
        throw std::out_of_range("Координаты вне поля.");
    }
    return grid_[y * width_ + x];
}

// Метод для получения ячейки (по индексу) для константного объекта
const Cell& Field::getCell(int x, int y) const {
    if (!isInside(x, y)) {
        throw std::out_of_range("Координаты вне поля.");
    }
    return grid_[y * width_ + x];
}

Cell::Type Field::getCellType(int x, int y) const {
    return getCell(x, y).getType();
}

// Метод для установки типа ячейки
void Field::setCellType(int x, int y, Cell::Type type) {
    getCell(x, y).setType(type);
}

// Деструктор (по умолчанию)
Field::~Field() = default;
