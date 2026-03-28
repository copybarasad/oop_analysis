//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_VECTOR2_H
#define GAME_TERM_VECTOR2_H

#define DEFAULT_X 0
#define DEFAULT_Y 0
#include "utils /ISerializable.h"

class Position: ISerializable {

    int x;
    int y;

public:

    Position(const int & x = DEFAULT_X, const int & y = DEFAULT_Y);

    void setCoords(const int & x, const int & y);

    void setX(const int & x);

    void setY(const int & y);

    int getX() const;

    int getY() const;

    bool operator==(const Position & obj) const;

    bool operator>(const Position & obj) const;

    bool operator<(const Position & obj) const;

    void operator+=(const Position & obj);

    Position operator+(const Position & obj) const;

    void load(Reader &reader) override;

    void save(Saver &saver) const override;
};


#endif //GAME_TERM_VECTOR2_H