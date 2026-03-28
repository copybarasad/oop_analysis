#ifndef AI_ENTITY_HPP
#define AI_ENTITY_HPP

#include "human.hpp"
#include <climits>
#include <string>

class AIEntity : public Human {
protected:
    Entity* find_closest_target(Team target_team, int scan_radius = 5);
    Entity* find_visible_target(Team target_team, int vis_radius = 10);
    Direction get_best_dir(Point to);
    std::string idle_step();
    
public:
    AIEntity(Field &field, Point position = {0, 0}, int health = 12);
};

#endif