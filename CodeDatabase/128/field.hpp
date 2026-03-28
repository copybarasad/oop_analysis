#pragma once
#include <vector>
#include "cell.hpp"

class Field {
public:
    Field(int width, int height);

    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;

    int getWidth() const noexcept { return width_; } 
    int getHeight() const noexcept { return height_; }

    Cell& At(int x, int y); 
    const Cell& At(int x, int y) const;
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};
