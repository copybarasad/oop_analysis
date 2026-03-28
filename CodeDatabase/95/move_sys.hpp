#ifndef MOVE_SYS_HPP
#define MOVE_SYS_HPP

#include <vector>

class Position;
class Entity;

class Mover {
public: 
    static void Move(Entity* e, int dx, int dy, std::vector<Position> free_cells, std::vector<Position> e_poses);
};

#endif