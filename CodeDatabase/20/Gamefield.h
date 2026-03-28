#pragma once
#include <vector>

class Cell;

class Gamefield {
private:
    int length;
    int width;
    std::vector<std::vector<Cell>> gf;

public:
    Gamefield(int length, int width);
    Gamefield(int side);
    Gamefield(const Gamefield& p);
    Gamefield(Gamefield&& p) noexcept;
    Gamefield& operator=(const Gamefield& other);
    Gamefield& operator=(Gamefield&& other) noexcept;

    Cell* getCell(int x, int y);
    int get_leng();
    int get_width();
    bool in_bounds(int x, int y) const;
};
