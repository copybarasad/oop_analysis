#ifndef POSITION_HPP
#define POSITION_HPP

// простая целочисленная позиция на поле
struct Position { int x{0}, y{0}; };

// сравнение позиций
inline bool operator==(const Position& a, const Position& b) noexcept {
    return a.x == b.x && a.y == b.y;
}
inline bool operator!=(const Position& a, const Position& b) noexcept {
    return !(a == b);
}
inline bool operator<(const Position& a, const Position& b) noexcept {
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

// сложение как «плюс смещение»
inline Position operator+(const Position& a, const Position& b) noexcept {
    return {a.x + b.x, a.y + b.y};
}

#endif