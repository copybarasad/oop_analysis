#ifndef POSITION_H
#define POSITION_H

#include <functional>
#include <iostream>

class Position {
private:
    int x_;
    int y_;

public:
    Position(int x = 0, int y = 0) : x_(x), y_(y) {}
    
    int x() const { return x_; }
    int y() const { return y_; }
    
    void setX(int x) { 
        if (x >= 0) x_ = x; 
    }
    
    void setY(int y) { 
        if (y >= 0) y_ = y; 
    }
    
    void setPosition(int x, int y) {
        setX(x);
        setY(y);
    }

    bool operator==(const Position& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
    
    Position operator+(const Position& other) const {
        return Position(x_ + other.x_, y_ + other.y_);
    }
    
    bool operator<(const Position& other) const {
        if (x_ != other.x_) return x_ < other.x_;
        return y_ < other.y_;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << "(" << pos.x_ << ", " << pos.y_ << ")";
        return os;
    }
};

namespace std {
    template<>
    struct hash<Position> {
        std::size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.x()) ^ (std::hash<int>()(pos.y()) << 1);
        }
    };
}

#endif