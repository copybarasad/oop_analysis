#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <stdexcept>
#include "cell.hpp"

struct Point {
    int x{0};
    int y{0};
};

class Board {
public:
    Board(int width, int height, double wall_density = 0.18, unsigned seed = 0u);

    Board(const Board& other);
    Board& operator=(const Board& other);

    Board(Board&& other) noexcept;
    Board& operator=(Board&& other) noexcept;

    ~Board();

    int Width() const noexcept;
    int Height() const noexcept;

    bool InBounds(int x, int y) const noexcept;
    bool IsPassable(int x, int y) const;

    void SetWall(int x, int y);
    void SetFloor(int x, int y);

    CellType GetCellType(int x, int y) const;

private:
    int width_;
    int height_;
    std::vector<Cell> cells_;

    int Index(int x, int y) const noexcept;
};

#endif