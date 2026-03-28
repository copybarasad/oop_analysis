#ifndef POSITION_HPP
#define POSITION_HPP

// простая целочисленная позиция на поле
class Position {
private:
    int x_{0};
    int y_{0};
public:
    Position() = default;
    Position(int x, int y) : x_(x), y_(y) {}

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    void setX(int v) noexcept { x_ = v; }
    void setY(int v) noexcept { y_ = v; }
};

// сравнение позиций
inline bool operator==(const Position& a, const Position& b) noexcept {
    return a.getX() == b.getX() && a.getY() == b.getY();
}
inline bool operator!=(const Position& a, const Position& b) noexcept {
    return !(a == b);
}
inline bool operator<(const Position& a, const Position& b) noexcept {
    return (a.getX() < b.getX()) || (a.getX() == b.getX() && a.getY() < b.getY());
}

// сложение как «плюс смещение»
inline Position operator+(const Position& a, const Position& b) noexcept {
    return Position{a.getX() + b.getX(), a.getY() + b.getY()};
}

#endif
