#ifndef GAME_DRAWER_HPP
#define GAME_DRAWER_HPP

#include <memory>
#include "player.hpp"
#include "hand.hpp"
#include "map.hpp"
#include "visual_master.hpp"

class GameDrawer {
private:
    std::unique_ptr<VisualMaster> visualMaster;
public:
    GameDrawer(std::unique_ptr<VisualMaster> vm) : visualMaster(std::move(vm)) {};
    void draw_game(Map* map, Player* player, Hand* hand);
};
#endif