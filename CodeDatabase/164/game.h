#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "control.h"
#include "print.h"
#include "view.h"
#include "hand.h"
#include "user_input.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "json.hpp"

#define SAVE_GAME_FILE "last_game.json"
#define LOG_FILE "log.txt"
using json = nlohmann::json;

enum class end_type{
    user_end,
    victory,
    defeat,
    crash
};

struct ExitException : public std::exception {
    const char* what() const noexcept override;
};

template<typename InputT = User_input, typename ViewT = Game_view<Console_renderer>>
class Game{
    protected:
    unsigned int game_lvl;
    unsigned int hard_lvl;
    unsigned int win_points;
    Field field;
    Hand hand;

    InputT user_input;
    ViewT view;
    Control control;
    Logger logger;

    public:

    Game();

    void start_game();

    void main_loop();

    protected:
    void init_game();

    void load_game();

    void save_game();

    void level_up();

    void end_game(end_type type)

    void help();

    void Hero_turn();

    void Ally_turn();

    void Enemy_turn();

    void Base_turn();

    void Tower_turn()
};

#endif