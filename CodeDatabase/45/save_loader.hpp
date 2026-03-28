#ifndef SAVE_LOADER_HPP
#define SAVE_LOADER_HPP

#include "player.hpp"
#include "hand.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "guard_tower.hpp"

#include "area_spell.hpp"
#include "trap_spell.hpp"
#include "direct_spell.hpp"

#include <string>
#include <iostream>
// weq
#include <fstream>
#include <string>

class SaveLoader {
private:
    void load_error(std::string error);
public:
    SaveLoader();
    void save(std::string filename, Map* map, Hand* hand, int level);
    bool load(std::string filename, Map*& map, Hand*& hand, Player*& player, int* level);
};

#endif