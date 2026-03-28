#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <string>
#include "Node.h"
#include <iostream>

class Field {
private:
    int width;
    int length;
    std::vector<std::vector<Node>> grid;

public:
    // Конструкторы
    Field(int w, int l);
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;

    // Геттеры
    int getWidth() const { return width; }
    int getLength() const { return length; }
    
    // Проверки и доступ
    bool areCoordinatesValid(int x, int y) const;
    Node& getNode(int x, int y);
    const Node& getNode(int x, int y) const;  // Добавили const версию
    
    // Генерация препятствий
    void generateRandomObstacles(int percentage);
    
    // Сохранение/загрузка
    void save(std::ostream& os) const;
    void load(std::istream& is);
    
    // Вспомогательные методы
    void clear();
};

#endif // FIELD_H