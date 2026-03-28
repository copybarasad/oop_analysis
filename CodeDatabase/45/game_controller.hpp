#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "player.hpp"
#include "hand.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "guard_tower.hpp"
#include "game_drawer.hpp"

#include "area_spell.hpp"
#include "trap_spell.hpp"

#include "save_loader.hpp"

#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

#include <chrono>
#include <thread>

#include <fstream>

#include <termios.h>

#include <sstream>

#include "command.hpp"
#include "visual_master.hpp"

void setNonCanonicalMode();
void restoreTerminalMode();

class GameController {
private:
    std::unique_ptr<GameDrawer> gameDrawer;
    Player* player;
    Hand* hand;
    Map* gameMap;

    SaveLoader sl_master;

    int current_level;
    
    

    void load_error(std::string error);

    

    void pause_game();

    void fill_map(int monsters_count, int towers_count, int monsters_hp, int monsters_damage, int towers_hp, int towers_damage);
public:
    GameController(std::unique_ptr<GameDrawer> game_drawer_ptr);
    ~GameController();
    void start();
    void player_step(Command com);
    void monsters_step();
    void towers_step();
    void print_status();
    bool game_active();
    void end_cycle();
    void game_cycle(Command com);

    void new_game();

    void next_level(Command com);

    void save(Command com);
    void load(Command com);

    void gain_stage();
};

#endif