// Cell.h
#ifndef CELL_H
#define CELL_H

#include <string>

class Cell {
public:
    enum class Type { Empty, Player, Enemy, Impassable };

    Cell();
    explicit Cell(Type t);

    Type getType() const;
    void setType(Type t);

    char toChar() const;

private:
    Type type_;
};

#endif 