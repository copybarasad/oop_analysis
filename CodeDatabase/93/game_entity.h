#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

class Position {
public:
    int x;
    int y;
    
    Position(int x = 0, int y = 0);
    bool operator==(const Position& other) const;
};

#endif