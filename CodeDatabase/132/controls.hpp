#pragma once

#include <iostream>
#include <set>
#include <map>
#include <termios.h>

namespace Key {
    extern const char UP;
    extern const char DOWN;
    extern const char LEFT;
    extern const char RIGHT;
    extern const char CHANGE_COMBAT;
    extern const char SHOOT;

    extern const char UNKNOWN;

    extern std::set<char> AVAILABLE;

    extern std::set<char> POSITIONAL;

    extern std::map<char, std::pair<int, int>> DESCRIPTION;
}

namespace Menu {
    extern const char NEW_GAME;

    extern const char SAVE_GAME;
    extern const char LOAD_GAME;

    extern std::set<char> AVAILABLE;

    extern const char HP_UP;
    extern const char CP_UP;
}

class Control {
private:
    static char prev_command;
public:
    static void set_keypress();
    static void reset_keypress();

    static char get_prev_command();
    static void reset_prev_command();

    static char get_move();
    static char get_menu_action_on_start();
    static char get_menu_action_in_game();

    static char get_ench();

    static std::string get_save_name();
};