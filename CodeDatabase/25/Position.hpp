#pragma once

// Simple grid position.
struct Pos {
    int x{0};
    int y{0};
};

inline bool operator==(const Pos& a, const Pos& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Pos& a, const Pos& b) {
    return !(a == b);
}
