#ifndef VISUAL_MASTER_HPP
#define VISUAL_MASTER_HPP

#include <string>
#include "map.hpp"
#include "player.hpp"
#include "hand.hpp"

class VisualMaster {
public:
    ~VisualMaster() = default;
    virtual void draw_game(Map* map, Player* player, Hand* hand) = 0;
};

#endif