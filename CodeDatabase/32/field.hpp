#ifndef FIELD_H
#define FIELD_H

#include "cell.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

class Field {
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell> > grid_;

public:
    Field(int width, int height);

    Field(const Field& other);                    // конструктор копирования
    Field(Field&& other) noexcept;                // конструктор перемещения
    Field& operator=(const Field& other);         // оператор копирования
    Field& operator=(Field&& other) noexcept;     // оператор перемещения
    ~Field() = default;                           // деструктор (можно = default)


    int width () const;
    int height() const;

    Cell& At(int x, int y);
    const Cell& At(int x, int y) const;

    void Clear();
    void Print() const;
};

#endif