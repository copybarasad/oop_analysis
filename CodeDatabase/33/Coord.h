#pragma once

class Coord {
public:
    Coord();
    Coord(int x, int y);

    int X() const;
    int Y() const;
    void Set(int x, int y);

    bool operator==(const Coord& other) const;
    bool operator!=(const Coord& other) const;

private:
    int x_;
    int y_;
};
