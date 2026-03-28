#pragma once
#include <string.h>
#include <iostream>
#include "../world/GameField.h"

class FieldRenderer{
private:
    static const std::string COLOR_RESET;
    static const std::string COLOR_PLAYER;
    static const std::string COLOR_ENEMY;
    static const std::string COLOR_WALL;
    static const std::string COLOR_EMPTY;
    static const std::string COLOR_SLOWING;
    static const std::string COLOR_TOWER;
    static const std::string COLOR_TRAP;

    class CellDisplay {
    public:
        char symbol;
        std::string color;
        CellDisplay(char s, const std::string& c) : symbol(s), color(c) {}
    };
    CellDisplay getCellSymbolColor(const GameField& field,int x, int y) const;
public:
    
    void printFieldWithColors(const GameField& field) const;
};
