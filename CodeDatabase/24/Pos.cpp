#include "Pos.hpp"
#include <cmath>

// Конструктор позиции
Pos::Pos(int x, int y) : x(x), y(y) {}

// Геттеры
int Pos::getX() const { return x; }
int Pos::getY() const { return y; }
void Pos::get(int& outX, int& outY) const { outX = x; outY = y; }

// Сеттеры
void Pos::setX(int newX) { x = newX; }
void Pos::setY(int newY) { y = newY; }
void Pos::set(int newX, int newY) { x = newX; y = newY; }

// Операции с позициями
Pos Pos::operator+(const Pos& other) const {
    return Pos(x + other.x, y + other.y);
}

Pos Pos::operator-(const Pos& other) const {
    return Pos(x - other.x, y - other.y);
}

bool Pos::operator==(const Pos& other) const {
    return x == other.x && y == other.y;
}

bool Pos::operator!=(const Pos& other) const {
    return !(*this == other);
}

Pos Pos::operator*(int scalar) const {
    return Pos(x * scalar, y * scalar);
}

// Вспомогательные методы
double Pos::distanceTo(const Pos& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Проверка валидности позиции в пределах границ
bool Pos::isValid(int maxX, int maxY) const {
    return x >= 0 && x < maxX && y >= 0 && y < maxY;
}

bool Pos::isValid(int minX, int minY, int maxX, int maxY) const {
    return x >= minX && x < maxX && y >= minY && y < maxY;
}
