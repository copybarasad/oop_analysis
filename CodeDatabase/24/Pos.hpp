#pragma once
#include <cmath>

// Класс для работы с координатами на игровом поле
class Pos {
private:
    int x;  // Координата X
    int y;  // Координата Y
    
public:
    // Конструктор
    Pos(int x = 0, int y = 0);
    
    // Геттеры
    int getX() const;
    int getY() const;
    void get(int& outX, int& outY) const;
    
    // Сеттеры
    void setX(int newX);
    void setY(int newY);
    void set(int newX, int newY);
    
    // Операции с координатами
    Pos operator+(const Pos& other) const;
    Pos operator-(const Pos& other) const;
    bool operator==(const Pos& other) const;
    bool operator!=(const Pos& other) const;
    Pos operator*(int scalar) const;
    
    // Вспомогательные методы
    double distanceTo(const Pos& other) const;
    bool isValid(int maxX, int maxY) const;
    bool isValid(int minX, int minY, int maxX, int maxY) const;
};
