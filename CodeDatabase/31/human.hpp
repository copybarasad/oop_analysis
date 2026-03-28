#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "entity.hpp"
#include "field/point.hpp"

class Human : public Entity {
protected:
    bool move_to(Point offset);

public:
    enum class Direction {left, right, up, down, TotalCount};

    Human(Field &field, Point position = {0, 0}, int health = 12);
    
    bool left_motion();
    bool right_motion();
    bool up_motion();
    bool down_motion();
    static Point get_direction_offset(Direction dir);
};

#endif