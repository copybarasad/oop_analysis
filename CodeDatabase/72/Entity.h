//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_ENTITY_H
#define GAME_TERM_ENTITY_H
#include "../Position.h"

#define DEFAULT_POSITION {0, 0}

class Entity {

protected:
    Position position;

public:

    explicit Entity(const Position & position = DEFAULT_POSITION);

    Position getPosition() const;

    void setPosition(const Position & position);

    void move(const Position & delta);

};


#endif //GAME_TERM_ENTITY_H