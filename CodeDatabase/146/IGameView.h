#pragma once
#include <string>

class IGameView {
public:
    virtual ~IGameView() = default;

    virtual void show_message(const std::string& msg) = 0;
    virtual void on_level_init(int level) = 0;
    virtual void on_game_over(bool win) = 0;

    virtual int ask_upgrade_choice() = 0;
    virtual int ask_game_over_action() = 0;
};
