#pragma once
#include "Constants.h"

#include <string>

class Cell {
public:
    enum class Type {Empty, Player, Enemy, Wall, Trap, Tower};
private:
    Type contains = Type::Empty;

public:
    Cell();
    Cell(Type type);
    ~Cell() = default;
    Type getType() const;
    void setType(Type t);

    std::string to_string() const;
};