#ifndef CELL_HPP
#define CELL_HPP

#include <string>

class Cell {
public:
    enum class Type {
        COMMON,
        BLOCKING,
        SLOWING
    };

private:
    bool isEmpty;
    Type cellType;

public:
    Cell(bool empty = true, Type type = Type::COMMON);
    
    bool isEmptyCell() const;
    Type getCellType() const;
    void setCellType(Type type);
    void setEmpty(bool empty);
    bool isPassable() const;
    
    static std::string typeToString(Type type);
    static Type stringToType(const std::string& typeStr);
};

#endif