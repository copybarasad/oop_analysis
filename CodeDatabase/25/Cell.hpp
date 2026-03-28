#pragma once

// Map cell types.
enum class Cell {
    Empty,
    Wall,
    Slow
};

inline bool is_passable(Cell c) { return c != Cell::Wall; }
inline bool is_slow(Cell c) { return c == Cell::Slow; }
