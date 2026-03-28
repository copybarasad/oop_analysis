#pragma once
#include <vector>
#include "Cell.h"

class Gamefield {
private:
    int length;
    int width;
    std::vector<Cell> cells;

public:
    Gamefield& operator=(const Gamefield& other);
    Gamefield& operator=(Gamefield&& other) noexcept;
    Gamefield(int length, int width);
    Gamefield(const Gamefield& p);
    Gamefield(Gamefield&& p) noexcept;

    Cell* get_cell(int x, int y);
    int get_length() const;
    int get_width() const;

    Cell* get_random_cell();

    void print() const;
};
