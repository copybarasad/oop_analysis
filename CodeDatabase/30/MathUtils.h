#pragma once
#include "Position.h"
#include <cstdlib>

inline int manhattan(const Position& a, const Position& b) {
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}
