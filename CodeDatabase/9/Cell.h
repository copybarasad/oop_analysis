#ifndef CELL_H
#define CELL_H

class Cell{
public:
    Cell();

    bool isPassable();
    void setPassable(bool value);

    bool isSlow();
    void setSlow(bool v);

    char getSymbol();
    void setSymbol(char c);

    bool hasTrap();
    int  trapDamage();
    void setTrap(int damage);
    void clearTrap();

private:
    bool passable_;
    bool slow_;
    char symbol_;

    bool trap_;
    int  trapDamage_;
};


#endif