#ifndef POSITION_H
#define POSITION_H

class Position {
private:
    int x, y;
    
public:
    Position(int pos_x = 0, int pos_y = 0);
    ~Position()  = default;
    int getX() const;
    int getY() const;
    
    bool equals(const Position& other) const;
   
};

#endif