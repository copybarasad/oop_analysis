#pragma once

class Coordinates 
{
protected:
    int X_coord;
    int Y_coord;
public:
    Coordinates(int X_coord = 0, int Y_coord = 0) : X_coord(X_coord), Y_coord(Y_coord) {}

    int getX_coord() const { return X_coord; }
    int getY_coord() const { return Y_coord; }

    void setXY_coord(int newX_coord, int newY_coord) 
    {
        X_coord = newX_coord;
        Y_coord = newY_coord;
    }

    bool operator==(const Coordinates &rival) const 
    {
        return X_coord == rival.X_coord && Y_coord == rival.Y_coord;
    }
};