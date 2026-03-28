#ifndef CELL_H
#define CELL_H

class Cell{
public:
    enum class Type {Player, Enemy, Empty, Tower, Obstacle, Slow, Player_slow, Enemy_slow, Trap, Player_trap, Magic_tower};
    
private:
    Type type;

public:
    Cell(Type type_ = {Type::Empty});

    Type get_type();

    void change_type(Type t);

    ~Cell();
};
#endif