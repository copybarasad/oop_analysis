#ifndef UNTITLED_COORD_H
#define UNTITLED_COORD_H

struct Coord {
    int row;
    int col;
    Coord() : row(0), col(0) {}
    Coord(int r, int c) : row(r), col(c) {}
};


#endif //UNTITLED_COORD_H