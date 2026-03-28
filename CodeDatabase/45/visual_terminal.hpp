#ifndef VISUAL_TERMINAL_HPP
#define VISUAL_TERMINAL_HPP

#include <string>
#include "visual_master.hpp"
#include "map.hpp"
#include "player.hpp"
#include "hand.hpp"
#include <iostream>

class VisualTerminal : public VisualMaster {
public:
    ~VisualTerminal() = default;
    virtual void draw_game(Map* map, Player* player, Hand* hand) override;
};

#endif