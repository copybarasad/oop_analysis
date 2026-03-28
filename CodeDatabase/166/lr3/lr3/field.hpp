#ifndef FIELD_HPP
#define FIELD_HPP
#include <vector>
#include "cell.hpp"
#include "position.hpp"

// прямоугольное поле клеток, хранит только клетки (логика перемещений вне класса)
class Field {
public:
    Field() = default;
    // размеры задаются через конструктор
    Field(int w, int h);
    ~Field() = default;

    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    // размеры поля
    int getWidth()  const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

    // доступ к клетке с проверкой границ
    const Cell& at(int x, int y) const;
    Cell&       at(int x, int y);

    // проверки границ и проходимости
    bool inBounds(int x, int y) const noexcept;
    bool passable(int x, int y) const noexcept;

private:
    // размеры и хранение клеток (row-major)
    int w_, h_;
    std::vector<Cell> grid_;
};

#endif