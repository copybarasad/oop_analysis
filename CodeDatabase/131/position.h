#ifndef OOP_POSITION_H_
#define OOP_POSITION_H_

#include <cstddef>

namespace rpg {
    struct Position {
        size_t x;
        size_t y;

        constexpr Position() : x(0), y(0) {
        }

        constexpr Position(size_t x_pos, size_t y_pos) : x(x_pos), y(y_pos) {
        }

        bool operator==(const Position &other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Position &other) const {
            return !(*this == other);
        }
    };
}

#endif
