#pragma once
#include <ncurses.h>
#include "../manager/Manager.h"

class HelpDisplay{
public:
    void display_choose_spell();
    void display_spell_was_choosed(char c, Manager manager);
    void display_spell_was_used(bool was);
    void display_save_game(const char* ans);
    void display_load_game(const char* ans);
    void print_win();
    void print_big_game_over();
};




