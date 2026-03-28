#ifndef POSITION_HPP
#define POSITION_HPP

class Position{
    private:
        int x_coord;
        int y_coord;
    public:
        Position(int x, int y);

        int get_x() const;
        int get_y() const;

        bool operator==(const Position& other) const;
        bool operator!=(const Position& other) const;
        Position operator+(const Position& other) const;
};

#endif