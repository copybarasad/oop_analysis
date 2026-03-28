#pragma once

class Cell {
public:
    enum Type {
        EMPTY,
        WALL,
        OBSTACLE
    };

    Cell(Type type = EMPTY);
    Type getType() const;
    void setType(Type type);
    char getChar() const;

private:
    Type m_type;
};