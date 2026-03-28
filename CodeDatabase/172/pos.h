#pragma once

namespace game {
    // ---------------- Type of direction -----------------
    enum class Direction { Left, Up, Right, Down, None };

    // ---------------- Coordinate struct -----------------
    struct Pos {
        int x{0};
        int y{0};

        // == Constructors ==
        Pos() noexcept {}
        Pos(int nx, int ny): x(nx), y(ny) {}

        // == Operators ==
        Pos operator+(const Pos& other) const noexcept {
            return Pos(x + other.x, y + other.y);
        }

        Pos operator-(const Pos& other) const noexcept {
            return Pos(x - other.x, y - other.y);
        }

        bool operator==(const Pos& other) const noexcept {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const Pos& other) const noexcept {
            return !(*this == other);
        }

        // -------- Convert Direction to Position ---------
        static Pos toPos(Direction dir)
        {
            switch (dir) {
            case Direction::Left:   return {-1, 0};
            case Direction::Down:   return {0, 1};
            case Direction::Right:  return {1, 0};
            case Direction::Up:     return {0, -1};
            case Direction::None:   return {0, 0};

            default:                return {0, 0};
            }
        }
    };
}