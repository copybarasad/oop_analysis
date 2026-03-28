#pragma once

enum Direction
{
    Left,
    Up,
    Right,
    Down
};

struct Cell
{
    int _x;
    int _y;

    Cell();

    Cell(int x, int y);

    bool operator==(const Cell& other) const;

    bool operator!=(const Cell& other) const;
};